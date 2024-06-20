<template lang="html">
  <div class="node-wrap">
  <div class="node" :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="row node-header">
        <div class="q col-2">
        <!-- Inputs-->
          <div class="col-1 input" :class="$helper.socketConnectedClass(inputVariable.connections, inputVariable.socket.name)">
            <Socket v-socket:input="inputVariable" type="input" :socket="inputVariable.socket"></Socket>
        </div>
        </div>
        <div class="node-title q-pt-sm col-8">{{node.name}}</div>
      <!-- Outputs-->
        <div class="q col-2">
          <template v-for="output in outputs()">
          <div class="output" :key="output.key" :class="$helper.socketConnectedClass(output.connections, output.socket.name)">
                <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
            </div>
            </template>
        </div>
      </div>
      <!-- Controls-->
      <div class="row q-pt-sm text-selector">
          <div class="col-5 row">
            <template v-for="input in inputsAudio">
             <div class="col-6 input" :key="input.key" :class="$helper.socketConnectedClass(input.connections, input.socket.name)">
            <Socket v-socket:input="input" type="input" :socket="input.socket"></Socket>
        </div>
            {{input.key - 2}}
        </template>
          </div>
          <div class="col-3"><q-btn flat round icon="icon-icon-add-inverse" size="7px" @click="addSelector" /></div>
          <div class="col-3"><q-btn flat round color="#31484c" icon="remove"  size="7px" @click="removeSelector" class="q-tree-btn" /></div>
      </div>
  </div>
</div>
</template>

<script>
import VueRenderPlugin from 'rete-vue-render-plugin'
import * as Socket from '../sockets.js'
import Rete from 'rete'
export default {
  mixins: [VueRenderPlugin.mixin],
  data: function () {
    return {
      numberLabel: 0
    }
  },
  components: {
    Socket: VueRenderPlugin.Socket
  },
  methods: {
    getNodeClass () {
      return 'default-node-color'
    },
    async addSelector () {
      var inp1 = new Rete.Input(String(this.numOfAudioInputs), 'Value', Socket.audio)
      // this.numOfAudioInputs++
      this.node.addInput(inp1)
      const payload = {
        nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id),
        socketId: inp1.key,
        componentName: this.node.name
      }
      await this.$store.dispatch('main/addSocket', payload)
      await this.$store.state.rete.editor.trigger('process')
      this.numOfAudioInputs++
      console.log(this.$store.state.rete.editor)
    },
    async removeSelector () {
      const input = this.inputs()[this.inputs().length - 1]
      const inputkey = input.key
      if (inputkey === '3') {
        return null
      } else {
        console.log(input)
        input.connections.slice().map(this.$store.state.rete.editor.removeConnection.bind(this.$store.state.rete.editor))
        const payload = {
          nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id),
          socketId: input.key
        }
        await this.$store.dispatch('main/removeSocket', payload)
        this.node.inputs.delete(input.key)
        input.node = null
        await this.$store.state.rete.editor.trigger('process')
        // await this.$store.state.main.engine.process(this.$store.state.main.editor.toJSON())
        this.numOfAudioInputs--
      }
    }
  },
  computed: {
    inputVariable () {
      return this.inputs().find(element => element.socket.name === 'variable')
    },
    inputsAudio () {
      console.log(this.numOfAudioInputs)
      return this.inputs().filter(element => element.socket.name === 'audio')
    },
    numOfAudioInputs: {
      get () {
        return this.node.data.nextSocketId
      },
      set (v) {
        this.node.data.nextSocketId = v
      }
    }
  }
}
</script>
