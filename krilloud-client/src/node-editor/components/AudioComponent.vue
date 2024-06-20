<template lang="html">
  <div class="node-wrap">
  <div class="node" :class="[selected(), 'audio-container', getNodeClass()] | kebab" :id="this.node.data.newId || (!this.node.data.newId && this.node.id)">
    <div class="row node-header" >
        <div class="node-title q-pt-sm col-10"> {{ (audioContainerName.length > 15 ? (audioContainerName.substring(0, 14) + '..') : audioContainerName) }}</div>
        <div class="q col-1 q-pl-md">
        <q-btn padding="xs" color="white" class="q-tree-btn q-mr-xs" icon="icon-icon-edit-c" flat @click="showAudioPoolViewer()" ></q-btn>
        </div>
        <div class="q col-1">
        <!-- Outputs-->
        <template v-for="output in outputs()">
          <div class="output" :class="$helper.socketConnectedClass(output.connections, output.socket.name)" :key="output.key">
              <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
          </div>
        </template>
        </div>
      </div>
        <!-- Controls-->
  <div class="row radio-options">
    <div class="q-pt-sm col-4">Play order: </div>
    <div class="q-pa-s col-8">
      <q-option-group
      v-model="mode"
      :options="playOrderOptions"
      color="blue-grey"
      style="height: auto"
      size="22px"

    ></q-option-group>
      </div>
    </div>
    <div class="row loop-option items-baseline q-pb-xs">
    <div class="col-4 text-left">Loop</div>
    <div class="col-1">
        <q-checkbox keep-color left-label size="14px" color="grey-5" v-model="loop"></q-checkbox>
      </div>
    </div>
    <div class="row contents-expansion">
    <div class="q col-12">
    <q-expansion-item dense>
      <template v-slot:header>
          <q-item-section>
            Contents
          </q-item-section>
          <q-item-section>
            <q-btn style="margin-rigth: 20px" size="xs" color="white" class="q-tree-btn" icon="icon-icon-add-inverse" flat @click="addAudioModal()"></q-btn>
          </q-item-section>
          <q-item-section side>
          </q-item-section>
        </template>
     <q-card class="collapse-scroll">
          <q-list class="scroll2" style="max-height: 100px">
          <q-item stlye="width: 92%" v-for="item in audios" :key="item.key">{{item.fileName}}</q-item>
          </q-list>
        </q-card>
      </q-expansion-item>
  </div>
  </div>
</div>
</div>
</template>

<script>
import VueRenderPlugin from 'rete-vue-render-plugin'

export default {
  mixins: [VueRenderPlugin.mixin],
  components: {
    Socket: VueRenderPlugin.Socket
  },
  computed: {
    playOrderOptions () {
      return this.$store.state.main.playOrderOptions
    },
    audios () {
      return this.node.data.audioList
    },
    currentAudioResources () {
      return this.$store.state.audioPool.currentAudioResources
    },
    audioContainerName () {
      return this.node.data.name
    },
    showAddAudioModal: {
      get () {
        return this.$store.state.audioPool.showAddAudioModal
      },
      set (v) {
        this.$store.commit('audioPool/setshowAddAudioModal', v)
      }
    },
    mode: {
      get () {
        return this.node.data.mode
      },
      set (v) {
        this.$store.dispatch('audioPool/setSelectionMode', { nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id), mode: v })
        this.node.data.mode = v
      }
    },
    loop: {
      get () {
        return this.node.data.loop
      },
      set (v) {
        this.$store.dispatch('audioPool/setAudioContainerLoopable', { nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id), loop: v })
        this.node.data.loop = v
      }
    }
  },
  data: function () {
    return {
      shape: 'line'
    }
  },
  methods: {
    setClass (connection) {
      return this.$helper.soc
    },
    getNodeClass () {
      return 'default-node-color'
    },
    addAudioModal () {
      this.$store.commit('audioPool/setCurrentAudioResources', { nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id), audioResources: this.audios })
      this.$store.commit('audioPool/setshowAddAudioModal', true)
    },
    showAudioPoolViewer () {
      this.$store.commit('audioPool/setCurrentAudioResources', { nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id), audioResources: this.audios, name: this.audioContainerName })
      this.$store.commit('audioPool/setshowAudioPoolViewer', true)
      this.$store.state.rete.editor.selected.list = []
    }
  }
}
</script>
