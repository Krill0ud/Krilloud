import 'regenerator-runtime'
import Rete from 'rete'
import { AudioComponent } from './components/AudioComponent'
import { SelectorComponent } from './components/SelectorComponent'
import { VariableComponent } from './components/VariableComponent'
import { MixerComponent } from './components/MixerComponent'
import { OutputComponent } from './components/OutputComponent.js'
import { EchoComponent } from './components/filters/EchoComponent.js'
import { PitchComponent } from './components/filters/PitchComponent.js'
import { LoFiComponent } from './components/filters/LoFiComponent.js'
import { FlangerComponent } from './components/filters/FlangerComponent.js'
import { DcRemoverComponent } from './components/filters/DcRemoverComponent.js'
import { RobotizerComponent } from './components/filters/RobotizerComponent.js'
import { WaveshaperComponent } from './components/filters/WaveshaperComponent.js'
import { BassBoostComponent } from './components/filters/BassBoostComponent.js'
import ContextMenuPlugin from 'rete-context-menu-plugin'
import ConnectionPlugin from 'rete-connection-plugin'
import AreaPlugin from 'rete-area-plugin'
import VueRenderPlugin from 'rete-vue-render-plugin'
import ModulePlugin from 'rete-module-plugin'
import { Notify } from 'quasar'
import HistoryPlugin from './plugins/custom-history-plugin'
import Vue from 'vue'

export default async function (container, store) {
  var components = [
    new AudioComponent(),
    new SelectorComponent(),
    new VariableComponent(),
    new MixerComponent(),
    new OutputComponent(),
    new EchoComponent(),
    new PitchComponent(),
    new LoFiComponent(),
    new FlangerComponent(),
    new DcRemoverComponent(),
    new RobotizerComponent(),
    new WaveshaperComponent(),
    new BassBoostComponent()
  ]

  var editor = new Rete.NodeEditor('krill@0.1.0', container)
  editor.use(ConnectionPlugin)
  editor.use(VueRenderPlugin)
  editor.use(HistoryPlugin, { keyboard: true })
  editor.use(ContextMenuPlugin, {
    searchBar: false,
    delay: 4000,
    allocate (component) {
      if (component.data.path === 1) return ['Add Module']
      // if (component.data.path === 2) return ['Add Variable']
      if (component.data.path === 3) return ['Add Module', 'Filter']
      if (component.data.path === 4) return null // output
      // if (!component.data.path) return ['Add Variable', 'New']
    },
    rename (component) {
      return component.name
    },
    items: {
      'Remove Selection': (event) => {
        const nodeList = editor.selected.list
        nodeList.forEach(node => {
          if (node.name !== 'Output') {
            editor.removeNode(node)
          }
        })
        if (editor.selected.list.length === 0) {
          Notify.create({
            type: 'warning',
            message: 'There is no selected nodes.'
          })
        }
      }
    },
    nodeItems: {
      Clone: false,
      Delete: false,
      'Remove Node': (event) => {
        editor.removeNode(event.node)
      },
      'Clone Node': (event) => {
        Vue.prototype.$helper.clone()
      }

    }

  })
  editor.use(AreaPlugin)

  var engine = new Rete.Engine('krill@0.1.0')
  await store.dispatch('tag/init')
  const tags = store.state.tag.tags
  editor.use(ModulePlugin, { engine, tags })

  components.map(c => {
    editor.register(c)
    engine.register(c)
  })

  editor.on(
    'process nodecreated noderemoved connectioncreated connectionremoved',
    async () => {
      await engine.abort()
      await engine.process(editor.toJSON())
    }
  )

  editor.on(
    'nodecreated',
    async (node) => {
      if (!store.state.tag.isLoadingTag) {
        let varName = null
        if (node.name === 'Variable') {
          const variable = store.state.variable.variables.find(item => item.id === node.data.id)
          varName = variable.name
        }
        const payload = {
          position: node.position,
          type: node.name,
          varName: varName // for variable creation purpose
        }
        const result = await store.dispatch('main/createNode', payload)
        Object.assign(node.data, result)

        if (node.data.audioList && node.data.audioList[0]) {
          node.data.audioList.forEach(item => {
            store.dispatch('audioPool/addSound', { tagName: store.state.tag.currentTag.name, nodeId: node.data.newId || (!node.newId && node.id), soundId: item.id, fullPathName: (item.fullPathName || item.fullPath), volume: 1 })
          })
        }
      }
    }
  )

  editor.on(
    'noderemoved',
    async (node) => {
      if (!store.state.tag.isLoadingTag) {
        store.dispatch('main/removeNode', node.data.newId || (!node.data.newId && node.id))
      }
    }
  )

  editor.on(
    'connectioncreated',
    async (connection) => {
      if (!store.state.tag.isLoadingTag) {
        const payload = {
          nodeId: connection.output.node.data.newId || (!connection.output.node.data.newId && connection.output.node.id),
          socketId: connection.output.key,
          nodeIdToLink: connection.input.node.data.newId || (!connection.input.node.data.newId && connection.input.node.id),
          socketIdToLink: connection.input.key
        }
        Vue.prototype.$store.commit('rete/setCurrentPathConnection', null)
        store.dispatch('main/createConnection', payload)
      }
    }
  )

  editor.on(
    'connectioncreate',
    async (params) => {
      if (params.input.node.name === 'Selector' && !store.state.tag.isLoadingTag) {
        try {
          const inputsAudios = Array.from(params.input.node.inputs.values())
          const firstAvailableInput = inputsAudios.find(item => item.socket.name === 'audio' && item.connections.length === 0)
          var connection = params.output.connectTo(firstAvailableInput)
          editor.view.addConnection(connection)
          editor.trigger('connectioncreated', connection)
        } catch (e) {
          editor.trigger('warn', e)
        }
        return false
      }
    }
  )

  editor.on(
    'connectionremoved',
    async (connection) => {
      if (!store.state.tag.isLoadingTag) {
        const payload = {
          nodeId: connection.output.node.data.newId || (!connection.output.node.data.newId && connection.output.node.id),
          socketId: connection.output.key,
          nodeIdToLink: connection.input.node.data.newId || (!connection.input.node.data.newId && connection.input.node.id),
          socketIdToLink: connection.input.key
        }
        store.dispatch('main/removeConnection', payload)
      }
    }
  )

  editor.on('multiselectnode', (nodes) => {
    store.commit('audioPool/setshowAudioPoolViewer', false)
  })

  editor.on('zoom', ({ source, translate, zoom }) => {
    if (zoom < 0.25 || zoom > 1) {
      return false
    }
    return source !== 'dblclick'
  })

  editor.on('showcontextmenu', ({ e, node }) => { // e - MouseEvent, node - Node instance or null
    if (node && node.name === 'Output') { return false }
  })

  editor.on('mousemove', ({ x, y }) => { // e - MouseEvent, node - Node instance or null
    store.commit('rete/setPosition', { x: x, y: y })
  })

  editor.on('nodetranslated', ({ node }) => { // e - MouseEvent, node - Node instance or null
    store.dispatch('main/updateNodePosition', { nodeId: node.data.newId || (!node.data.newId && node.id), position: node.position })
  })

  editor.on('connectionpick', io /* Input or Output */ => {
    const ioElement = io
    ioElement.fromPick = true // connection drop is firing after connection pick without purpose, and we use this flag to avoid it, and use it the event to clean the connection path
    store.commit('rete/setCurrentPathConnection', ioElement)
  })

  editor.on('connectiondrop', io /* Input or Output */ => {
    if (io.fromPick) {
      if (io.node.outputs.get(io.key)) { // when a connection begin from the destiny socket
        io.node.outputs.get(io.key).fromPick = false
      }
    } else {
      store.commit('rete/setCurrentPathConnection', null)
    }
  })

  editor.on('nodeselect', node /* Input or Output */ => {
    if (store.state.rete.currentPathConnection) {
      const inputs = Array.from(node.inputs.values())
      Vue.prototype.$helper.autoConnect(inputs)
    }
    if (store.state.main.temporalElementDrag.type === 'audio' && node.name === 'AudioContainer') {
      const audio = store.state.main.temporalElementDrag
      const tagName = store.state.tag.currentTag.name
      const nodeId = node.data.newId || (!node.data.newId && node.id)
      const audioExist = node.data.audioList.find(item => item.id === audio.id)
      if (audio.isLeaf && !audioExist) {
        node.data.audioList.push({
          id: audio.id,
          fullPathName: (audio.fullPathName || audio.fullPath),
          fileName: audio.name,
          volume: 1
        })
        store.dispatch('audioPool/addSound', { tagName: tagName, nodeId: nodeId, soundId: audio.id, fullPathName: (audio.fullPathName || audio.fullPath), volume: 1 })
        store.commit('main/setTemporalElementDrag', { element: {}, type: null })
      }
      if (!audio.isLeaf) {
        new Promise((resolve) => {
          resolve(Vue.prototype.$helper.getLeafNodes(audio.children))
        }).then(async (result) => {
          result.forEach(audio => {
            const audioExist = node.data.audioList.find(item => item.id === audio.id)
            if (!audioExist) {
              node.data.audioList.push({
                id: audio.id,
                fullPathName: (audio.fullPathName || audio.fullPath),
                fileName: audio.name,
                volume: 1
              })
              store.dispatch('audioPool/addSound', { tagName: tagName, nodeId: nodeId, soundId: audio.id, fullPathName: (audio.fullPathName || audio.fullPath), volume: 1 })
            }
          })
          store.commit('main/setTemporalElementDrag', { element: {}, type: null })
        })
      }
      Notify.create({
        type: 'positive',
        icon: 'check_circle',
        message: 'The selected audios, were added to the selected audio container'
      })
    }
  })

  editor.on('click', (event, container) => {
    store.commit('rete/setCurrentPathConnection', null)
    const temporalElement = store.state.main.temporalElementDrag
    if (temporalElement.type) {
      if (temporalElement.type === 'variable') {
        const variable = store.state.variable.variables.find(item => item.name === temporalElement.name)
        store.dispatch('rete/loadVariable', { id: variable.id, name: temporalElement.name, x: event.e.offsetX, y: event.e.offsetY })
        store.commit('main/setTemporalElementDrag', { element: {}, type: null })
      }
      if (temporalElement.type === 'audio') {
        const audio = JSON.parse(temporalElement)
        var audioResources = []
        if (audio.isLeaf) {
          audioResources.push({
            id: audio.id, fullPathName: audio.fullPath, fileName: audio.name, volume: 1
          })
          store.dispatch('rete/loadAudioContainer', { audios: audioResources, x: event.e.offsetX, y: event.e.offsetY })
          store.commit('main/setTemporalElementDrag', { element: {}, type: null })
        } else {
          new Promise((resolve) => {
            resolve(Vue.prototype.$helper.getLeafNodes(audio.children))
          }).then(async (result) => {
            const audios = []
            result.forEach(item => {
              audios.push({
                id: item.id, fullPathName: item.fullPath, fileName: item.name, volume: 1
              })
            })
            await store.dispatch('rete/loadAudioContainer', { audios: audios, x: event.e.offsetX, y: event.e.offsetY })
            store.commit('main/setTemporalElementDrag', { element: {}, type: null })
          })
        }
      }
      Notify.create({
        type: 'positive',
        icon: 'check_circle',
        message: 'Node created'
      })
    }
  })

  editor.view.resize()
  AreaPlugin.zoomAt(editor)

  setTimeout(() => {
    editor.trigger('process')
  }
  , 1000)

  store.commit('rete/setEditor', editor)
  store.commit('rete/setEngine', engine)

  // editor.trigger('undo')
  // editor.trigger('redo')
  // if (store.state.main.tags.length === 0) {
  // await store.dispatch('rete/loadTag')
  // }
}
