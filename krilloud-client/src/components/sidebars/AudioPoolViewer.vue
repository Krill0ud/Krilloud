<template>
    <q-card class="tag-section audio-container-side non-selectable" square>
      <q-card-section class="sidebar sidebar-audioPool row" >
        <div class="sidebar-title col-8">
          <q-form @submit="renameNode">
          <q-input class="q-pa-sm non-selectable"
            dense
            v-model="audioContainerName"
            @blur="renameNode"/>
          </q-form>
        </div>
        <div class="text-title col-2" style="margin-top: 7px">
          <q-btn
            flat
            round
            icon="icon-icon-add"
            class="q-mr-xs"
            size="sm"
            @click="openAddAudioModal()"
          />
        </div>
        <div class="text-title col-2" style="margin-top: 7px">
          <q-btn
            flat
            round
            icon="icon-icon-close-1"
            class="q-mr-xs"
            size="sm"
            @click="closeAudioPoolViewer()"
          />
        </div>
        </q-card-section>
      <q-card-actions class="q-pa-sm text-modal">
          <q-list v-if="currentAudioResources.length > 0">
              <q-item dense style="width: 285px" v-for="item in currentAudioResources" :key="item.key">
                  <q-item-section >
                    <q-item-label class="row items-center channel">
                      <div class="col-6 channel text-left">{{item.fileName}}</div>
                      <q-badge class="slider-value col-4" color="white" text-color="black">{{item.volume}}
                    </q-badge>
                    </q-item-label>

                  <q-item-label class="row justify-between">
                  <div class="audio-container-side col-10">
                    <q-slider class="audio-container-side"
                        v-model="item.volume"
                        :min="0.0"
                        :max="1.0"
                        :step="0.1"
                        label
                        @input="setVolumenAudio(item.volume, item.id)"
                      >
                      </q-slider>
                    </div>
                    <div class="text-right ">
                    <q-btn color="white" class="q-tree-btn" size="10px" icon="icon-icon--delete" flat @click="deleteSound(item)"></q-btn>

                  </div>
                    </q-item-label>

                  </q-item-section>
                  </q-item>
              </q-list>
          </q-card-actions>
    </q-card>
</template>

<script>

export default {
  name: 'AudioPoolViewer',
  components: {

  },
  props: {},
  mounted () {
  },
  data: function () {
    return {
      slider: 1
    }
  },
  computed: {
    currentAudioResources () {
      return this.$store.state.audioPool.currentAudioResources.audioResources
    },
    nodeId () {
      return this.$store.state.audioPool.currentAudioResources.nodeId
    },
    currentTag () {
      return this.$store.state.tag.currentTag
    },
    showAddAudioModal () {
      return this.$store.state.audioPool.showAddAudioModal
    },
    audioContainerName: {
      get () {
        return this.$store.state.audioPool.currentAudioResources.name
      },
      set (v) {
        this.$store.state.audioPool.currentAudioResources.name = v
      }
    }
  },
  /* watch: {
    '$store.state.audioPool.currentAudioResources.audioResources': function (val) {
      this.currentAudioResources = val
    }

  }, */
  updated () {
    console.log('updated')
  },
  methods: {
    renameNode () {
      if (this.audioContainerName === '') {
        this.audioContainerName = 'AUDIO CONTAINER'
      }
      this.$store.dispatch('main/renameNode', { name: this.audioContainerName, nodeId: this.nodeId })
      const node = this.$store.state.rete.editor.nodes
      const nodeId = this.nodeId
      var nodeFound = node.find(item => item.data.newId === nodeId || (!item.data.newId && item.id === nodeId))
      nodeFound.data.name = this.audioContainerName
    },
    setVolumenAudio (v, soundId) {
      this.$store.dispatch('audioPool/setAudioVolume', { tagName: this.currentTag.name, nodeId: this.nodeId, soundId: soundId, value: v })
      const node = this.$store.state.rete.editor.nodes
      const nodeId = this.$store.state.audioPool.currentAudioResources.nodeId
      var nodeFound = node.find(item => item.data.newId === nodeId || (!item.data.newId && item.id === nodeId))
      var soundData = nodeFound.data.audioList.find(item => item.id === soundId)
      soundData.volume = v
    },
    closeAudioPoolViewer () {
      this.$store.commit('audioPool/setshowAudioPoolViewer', false)
    },
    openAddAudioModal () {
      this.$store.commit('audioPool/setshowAddAudioModal', true)
      // this.$store.commit('audioPool/setshowAudioPoolViewer', false)
    },
    deleteSound (item) {
      this.$store.dispatch('audioPool/removeSound', { tagName: this.currentTag.name, nodeId: this.nodeId, soundId: item.id })
      const node = this.$store.state.rete.editor.nodes
      const nodeId = this.$store.state.audioPool.currentAudioResources.nodeId
      var nodeFound = node.find(item => item.data.newId === nodeId || (!item.data.newId && item.id === nodeId))
      var index = nodeFound.data.audioList.findIndex((n) => n.id === item.id)
      nodeFound.data.audioList.splice(index, 1)
    }

  }
}
</script>
