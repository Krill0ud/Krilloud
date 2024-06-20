import AreaPlugin from 'rete-area-plugin'

export async function loadTag ({ commit, dispatch, rootState, getters, state }, id) {
  commit('tag/setIsLoadingTag', true, { root: true })
  if (id === null) { // initial scenario
    const current = rootState.tag.currentTag
    if (!current.data) {
      const treeData = await dispatch('main/loadTree', current.name, { root: true })
      if (!treeData) {
        current.data = getters.initialData
      } else {
        current.data = treeData
      }
    }
    state.editor.plugins.get('history').dontAddActions = true
    await state.editor.fromJSON(current.data)
    state.editor.plugins.get('history').dontAddActions = false
    await state.editor.trigger('process')
    commit('tag/setIsLoadingTag', false, { root: true })
  } else {
    const tagData = rootState.tag.tags.find(item => item.id === id)
    if (!tagData.data) {
      const treeData = await dispatch('main/loadTree', tagData.name, { root: true })
      if (!treeData) {
        tagData.data = getters.initialData
      } else {
        tagData.data = treeData
      }
    }
    const current = rootState.tag.currentTag
    console.log(state.editor.plugins.get('history'))
    current.data = state.editor.toJSON()
    await commit('tag/setTagInfo', current, { root: true })
    await commit('tag/setCurrentTag', tagData, { root: true })
    state.editor.plugins.get('history').dontAddActions = true
    await state.editor.fromJSON(rootState.tag.currentTag.data)
    state.editor.plugins.get('history').dontAddActions = false
    await state.editor.trigger('process')
    console.log('after process call', state.isLoadingTag)
    await commit('tag/setIsLoadingTag', false, { root: true })
    console.log('isLoadingTag updated', state.isLoadingTag)
  }
  state.editor.view.resize()
  const { area } = state.editor.view // read from Vue component data;
  area.zoom(0.76)
  await AreaPlugin.zoomAt(state.editor)
}

export async function loadVariable ({ state, rootState }, params) {
  const components = state.editor.components
  const componentsArray = Array.from(components.values())
  const variableComponent = componentsArray[2]
  const variableNode = await variableComponent.createNode({ name: params.name, id: params.id })

  // console.log('rete', state.editor.view.area.mouse.x, state.editor.view.area.mouse.y,
  // state.editor.view.area.container.clientHeight, state.editor.view.area.container.clientWidth, state.editor.view.area.transform)

  // console.log('position', params.x, params.y)
  const k = state.editor.view.area.transform.k
  const posX = params.x ? params.x : (state.editor.view.area.container.clientWidth / 2)
  const posY = params.y ? params.y : (state.editor.view.area.container.clientHeight / 2)
  variableNode.position = [(posX - state.editor.view.area.transform.x) / k,
    (posY - state.editor.view.area.transform.y) / k]

  state.editor.addNode(variableNode)
}

export async function loadAudioContainer ({ state, rootState }, params) {
  const components = state.editor.components
  const componentsArray = Array.from(components.values())
  const audioComponent = componentsArray[0]
  const audioNode = await audioComponent.createNode({ audioList: params.audios, mode: 0, loop: false })

  // console.log('rete', state.editor.view.area.mouse.x, state.editor.view.area.mouse.y,
  // state.editor.view.area.container.clientHeight, state.editor.view.area.container.clientWidth, state.editor.view.area.transform)

  // console.log('position', params.x, params.y)
  const k = state.editor.view.area.transform.k
  const posX = params.x ? params.x : (state.editor.view.area.container.clientWidth / 2)
  const posY = params.y ? params.y : (state.editor.view.area.container.clientHeight / 2)
  audioNode.position = [(posX - state.editor.view.area.transform.x) / k,
    (posY - state.editor.view.area.transform.y) / k]

  state.editor.addNode(audioNode)
  return audioNode.id
}

export async function getVariablesContextMenu ({ state, rootState }) {
  const result = {}
  rootState.variable.variables.forEach((item, index) => {
    result[item.name] = function () {
      loadVariable(state, { name: item.name, id: index, x: state.editor.view.area.mouse.x, y: state.editor.view.area.mouse.y })
    }
  })
  return result
}
/*
export async function removeVariable ({ state, rootState }, id) {
  recorre los tags
  por cada tag
  new Editor
  le carga el json del tag
  buscar esa variable en el editor, la borra
  ¿salvar data de nuevo en el tag editor.toJSON()?
}
*/
