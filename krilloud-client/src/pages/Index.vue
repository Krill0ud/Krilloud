<template>
  <q-page class="flex flex-center non-selectable">
    <div
      style="width: 100vw;
      height: 100vh;"
       :class="cursorMode"  @drop='onDrop($event)' @dragover.prevent @dragenter.prevent>
      <div class='absolute-top-right q-mt-md q-mr-md col column items-center'>
     <div class="hidden" v-shortkey="['space']" @shortkey="(playOrStop==='stop'?stop():play())" v-if="!modalOn"></div>
     <!--  <div class="hidden" v-shortkey="['space']" @shortkey="!modalOn?(playOrStop==='stop'?stop():play()): false"></div> -->
        <div :class="playOrStop" @click="(playOrStop==='stop'?stop():play())"
            ></div>
         <q-toggle v-model="retrigger" color="green"/>
         <div class='retrigger'>{{ $t('retrigger') }}</div>
      </div>
      <div class='absolute-top-left row q-pt-sm q-pl-sm'>
        <q-btn flat round icon="icon-icon-zoom-in" @click="zoomIn" size="20px" class="sidebar-icon"/>
        <q-btn flat round icon="icon-icon-zoom-out" @click="zoomOut" size="20px" class="sidebar-icon zoom-out-margin" />
      </div>
      <Rete/>
    </div>
    <OpenProjectModal :showOpenProjectModal="showOpenProjectModal" @update="showOpenProjectModal = $event" @click="newProject" />
    <NewProjectModal :showNewProjectModal="showNewProjectModal" @update="showNewProjectModal = $event"/>
    <AddAudioModal :showAddAudioModal="showAddAudioModal" @update="showAddAudioModal = $event" />
    <OptionsProjectModal :showOptionsProjectModal="showOptionsProjectModal" @update="showOptionsProjectModal = $event" />
    <ExportProjectModal :showExportProjectModal="showExportProjectModal" @update="showExportProjectModal = $event" />

  </q-page>
</template>

<script>
import Rete from '../components/Rete'
import OpenProjectModal from '../components/modals/OpenProjectModal'
import NewProjectModal from '../components/modals/NewProjectModal'
import AddAudioModal from '../components/modals/AddAudioModal'
import OptionsProjectModal from '../components/modals/OptionsProjectModal'
import ExportProjectModal from '../components/modals/ExportProjectModal'

export default {
  name: 'PageIndex',
  components: {
    Rete,
    OpenProjectModal,
    NewProjectModal,
    AddAudioModal,
    OptionsProjectModal,
    ExportProjectModal
  },
  afterMounted () {
  },
  computed: {
    cursorMode () {
      const temporalElement = this.$store.state.main.temporalElementDrag
      if (temporalElement.type === null) {
        return 'cursor-default'
      }
      return `cursor-copy-${temporalElement.type}`
    },
    modalOn () {
      return this.$store.getters['main/modalOn']
    },
    currentTag () {
      return this.$store.state.currentTag
    },
    getModalOn () {
      return this.$store.state.currentTag
    },
    position () {
      return this.$store.state.rete.position
    },
    isPlaying () {
      if (this.$store.state.tag.currentTag) {
        return this.$store.state.tag.currentTag.isPlaying
      } else {
        return null
      }
    },
    playOrStop () {
      if (this.isPlaying) {
        return 'stop'
      } else {
        return 'play'
      }
    },
    showExportProjectModal: {
      get () {
        return this.$store.state.project.showExportProjectModal
      },
      set (v) {
        this.$store.commit('project/showExportProjectModal', v)
      }
    },
    showOptionsProjectModal: {
      get () {
        return this.$store.state.project.showOptionsProjectModal
      },
      set (v) {
        this.$store.commit('project/showOptionsProjectModal', v)
      }
    },
    showOpenProjectModal: {
      get () {
        return this.$store.state.project.showOpenProjectModal
      },
      set (v) {
        this.$store.commit('project/setShowOpenProjectModal', v)
      }
    },
    showAddAudioModal: {
      get () {
        return this.$store.state.audioPool.showAddAudioModal
      },
      set (v) {
        this.$store.commit('audioPool/setshowAddAudioModal', v)
      }
    },
    showNewProjectModal: {
      get () {
        return this.$store.state.project.showNewProjectModal
      },
      set (v) {
        this.$store.commit('project/setShowNewProjectModal', v)
      }
    }
  },
  data () {
    return {
      currentTimeout: null,
      retrigger: false,
      isModal: false
    }
  },
  mounted () {
  },
  watch: {
    '$store.state.tag.currentTag.isPlaying': function (val) {
      if (!this.isPlaying && this.currentTimeout) {
        clearInterval(this.currentTimeout)
        this.currentTimeout = null
        if (this.retrigger) {
          this.play()
        }
      }
    }
  },
  methods: {
    async play () {
      var interval = 1500
      await this.$store.dispatch('tag/isPlaying')
      if (!this.isPlaying && !this.currentTimeout) {
        this.$store.dispatch('tag/play')
      }
      await this.$store.dispatch('tag/isPlaying')
      if (this.isPlaying) {
        this.currentTimeout = setInterval(() => {
          this.$store.dispatch('tag/isPlaying')
        }, interval)
      }
    },
    stop () {
      clearInterval(this.currentTimeout)
      this.$store.dispatch('tag/stop')
      this.currentTimeout = null
    },
    async onDrop (event) {
      const variable = event.dataTransfer.getData('variable')
      const audioParam = event.dataTransfer.getData('audio')
      if (variable) {
        const variableElement = this.$store.state.variable.variables.find(item => item.name === variable)
        this.$store.dispatch('rete/loadVariable', { id: variableElement.id, name: variable, x: event.offsetX, y: event.offsetY })
      } else if (audioParam) {
        if (event.srcElement.className !== 'node-editor') { // droping the audio over a node
          const nodes = this.$store.state.rete.editor.nodes
          const nodeAudios = nodes.filter(item => item.name === 'AudioContainer')
          let dropNodeId = event.path.find(item => (item.className && (typeof item.className === 'string' || item.className instanceof String) && item.className.indexOf('audio-container') > -1))
          if (dropNodeId) {
            dropNodeId = parseInt(dropNodeId.id)
            const nodeAudio = nodeAudios.find(a =>
              a.data.newId === dropNodeId || (!a.data.newId && (a.id === dropNodeId)
              ))
            const tagName = this.$store.state.tag.currentTag.name
            const nodeId = nodeAudio.data.newId || (!nodeAudio.data.newId && nodeAudio.id)

            const audios = JSON.parse(`[${audioParam}]`)
            audios.forEach(async audio => {
              const audioExist = nodeAudio.data.audioList.find(item => item.id === audio.id)
              if (audio.isLeaf && !audioExist) {
                nodeAudio.data.audioList.push({
                  id: audio.id,
                  fullPathName: (audio.fullPathName || audio.fullPath),
                  fileName: audio.name,
                  volume: 1
                })
                this.$store.dispatch('audioPool/addSound', { tagName: tagName, nodeId: nodeId, soundId: audio.id, fullPathName: (audio.fullPathName || audio.fullPath), volume: 1 })
              }
              if (!audio.isLeaf) {
                new Promise((resolve) => {
                  resolve(this.$helper.getLeafNodes(audio.children))
                }).then(async (result) => {
                  result.forEach(audioItem => {
                    const audioExist = nodeAudio.data.audioList.find(item => item.id === audioItem.id)
                    if (!audioExist) {
                      nodeAudio.data.audioList.push({
                        id: audioItem.id,
                        fullPathName: (audioItem.fullPathName || audioItem.fullPath),
                        fileName: audioItem.name,
                        volume: 1
                      })
                      this.$store.dispatch('audioPool/addSound', { tagName: tagName, nodeId: nodeId, soundId: audioItem.id, fullPathName: (audioItem.fullPathName || audioItem.fullPath), volume: 1 })
                    }
                  })
                })
              }
            })
            this.$q.notify({
              type: 'positive',
              icon: 'check_circle',
              message: 'The selected audios, were added to the selected audio container'
            })
          }
        } else {
          var audioResources = []
          const audios = JSON.parse(`[${audioParam}]`)
          audios.forEach(async audio => {
            if (audio.isLeaf) {
              if (audioResources.findIndex(item => item.id === audio.id) === -1) {
                audioResources.push({
                  id: audio.id, fullPathName: audio.fullPath, fileName: audio.name, volume: 1
                })
              }
            } else {
              await new Promise((resolve) => {
                resolve(this.$helper.getLeafNodes(audio.children))
              }).then(async (result) => {
                result.forEach(item => {
                  if (audioResources.findIndex(item => item.id === audio.id) === -1) {
                    audioResources.push({
                      id: item.id, fullPathName: item.fullPath, fileName: item.name, volume: 1
                    })
                  }
                })
                // await this.$store.dispatch('rete/loadAudioContainer', { audios: audios, x: event.offsetX, y: event.offsetY })
              })
            }
          })
          await this.$store.dispatch('rete/loadAudioContainer', { audios: audioResources, x: event.offsetX, y: event.offsetY })
        }
      }
    },
    zoomIn () {
      const { area } = this.$store.state.rete.editor.view // read from Vue component data;
      area.zoom(area.transform.k + 0.1)
    },
    zoomOut () {
      const { area } = this.$store.state.rete.editor.view // read from Vue component data;
      area.zoom(area.transform.k - 0.1)
    },
    newProject () {
      this.$store.commit('project/setShowNewProjectModal', true)
    },
    getDefaultProject () {
      return this.$store.getters['project/projectInitialData']
    }
  }

}
</script>
<style lang="stylus" scoped>
.q-toggle {
  ::v-deep {
    .q-toggle__thumb {
      transform: scale(0.7, 0.7);
      color: white !important;
    }
  }
}
</style>
