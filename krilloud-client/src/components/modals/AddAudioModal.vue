<template>
      <q-dialog :value="showAddAudioModal" @input="$emit('update', $event)" persistent >
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showAddAudioModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header">ADD AUDIO</div>
            <q-btn icon="close" color="blue-grey-3" flat  dense v-close-popup @click="closeModal()"/>
          </q-card-section>

          <q-card-section class="q-pa-lg text-modal">

            <q-tree class="col-12 col-sm-6"
              ref="tree"
              :nodes="audios"
              node-key="id"
              tick-strategy="leaf-filtered"
              :ticked.sync="ticked"
              default-expand-all
              color="grey-5"
              size="xs"
            />
            <!-- <div v-for="tick in ticked" :key="`ticked-${tick}`">
              {{ tick }}
            </div> -->

          </q-card-section>
            <q-card-section align="center">
           <div><q-btn unelevated rounded  label="SAVE" class="my-button-sidebar text-modal" dense v-close-popup @click="addAudio" /></div>
           </q-card-section>
            </q-card>
      </q-dialog>
</template>

<script>

export default {
  props: { showAddAudioModal: Boolean },
  data () {
    return {
      ticked: []

    }
  },
  updated () {
    if (this.$refs.tree) {
      this.$refs.tree.expandAll()
    }
  },
  computed: {
    audios () {
      return this.$store.state.audioPool.audios
    },
    currentAudioResources () {
      return this.$store.state.audioPool.currentAudioResources.audioResources
    },
    indexOfAudios () {
      return this.$store.state.audioPool.indexOfAudios
    },
    currentTag () {
      return this.$store.state.tag.currentTag
    },
    nodeId () {
      return this.$store.state.audioPool.currentAudioResources.nodeId
    }
  },
  watch: {
    '$store.state.audioPool.currentAudioResources.audioResources': function (val) {
      if (val.length > 0) {
        this.ticked = []
        val.forEach(element => {
          this.ticked.push(element.id)
        })
      } else {
        this.ticked = []
      }
    }

  },
  methods: {
    addAudio () {
      var audioResources = []
      var audiosAdd = []
      var audiosRemove = []
      var finalAuddioResource = []

      for (var x = 0; x < this.currentAudioResources.length; x++) {
        audioResources.push(this.currentAudioResources[x].id)
      }
      this.ticked.forEach(item => {
        if (!audioResources.find(resource => resource === item)) {
          audiosAdd.push(item)

          const audioToAdd = this.indexOfAudios.find(toAdd => toAdd.id === item)
          this.$store.dispatch('audioPool/addSound', { tagName: this.currentTag.name, nodeId: this.nodeId, soundId: item, fullPathName: audioToAdd.fullPath, volume: 1 })
          finalAuddioResource.push({
            id: item,
            fullPathName: audioToAdd.fullPath,
            fileName: audioToAdd.name,
            volume: 1
          })
        }
      })

      audioResources.forEach(item => {
        if (!this.ticked.find(tickedElem => tickedElem === item)) {
          this.$store.dispatch('audioPool/removeSound', { tagName: this.currentTag.name, nodeId: this.nodeId, soundId: item })
          audiosRemove.push(item)
        }
      })

      // finalAuddioResource = audiosAdd
      this.currentAudioResources.forEach(item => {
        if (!audiosAdd.find(addItem => addItem === item.id) && !audiosRemove.find(removeItem => removeItem === item.id)) {
          finalAuddioResource.push(item)
        }
      })
      const nodes = this.$store.state.rete.editor.nodes
      const nodeId = this.$store.state.audioPool.currentAudioResources.nodeId
      nodes.find(item => item.data.newId === nodeId || (!item.data.newId && item.id === nodeId)).data.audioList = finalAuddioResource

      if (this.$store.state.audioPool.showAudioPoolViewer) {
        this.$store.commit('audioPool/setCurrentAudioResources', { nodeId: this.$store.state.audioPool.currentAudioResources.nodeId, audioResources: finalAuddioResource, name: this.$store.state.audioPool.currentAudioResources.name })
      }

      // }
    },
    closeModal () {

    },
    setCurrentAudios () {
      var finalAuddioResource = []
      this.ticked.forEach(item => {
        const audioToAdd = this.indexOfAudios.find(toAdd => toAdd.id === item)
        this.$store.dispatch('audioPool/addSound', { tagName: this.currentTag.name, nodeId: this.nodeId, soundId: item, fullPathName: audioToAdd.fullPath, volume: 1 })
        finalAuddioResource.push({
          id: item,
          fullPathName: audioToAdd.fullPath,
          fileName: audioToAdd.name,
          volume: 1
        })
      })
      const nodes = this.$store.state.rete.editor.nodes
      const nodeId = this.$store.state.audioPool.currentAudioResources.nodeId
      nodes.find(item => item.id === nodeId).data.audioList = finalAuddioResource
    }

  }
}
</script>
