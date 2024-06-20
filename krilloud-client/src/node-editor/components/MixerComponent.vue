<template lang="html">
  <div class="node-wrap">
  <div class="node" :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="row node-header">
        <div class="node-title q-pt-sm col-9 justify-between">{{node.name}}
        </div>
        <div class="q-pl-lg col-1" v-if="false"><q-toggle v-model="allowVariables" ></q-toggle></div>
        <div class="q-pl-lg row">
          <q-btn flat round icon="icon-icon-add" size="7px" @click="addInputs" />
          <q-btn flat round class="q-ml-xs remove-icon" color="mid-text" icon="remove" size="7px" @click="removeInputs" />
        </div>
        <div class="q col-1">
        <!-- Outputs-->
        <template v-for="output in outputs()">
        <div class="output"  :key="output.key" :class="$helper.socketConnectedClass(output.connections, output.socket.name)">
              <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
          </div>
          </template>
       </div>
      </div>
      <!-- Controls-->
      <div @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
          <div class="row q-pt-xs q-pr-sm" v-for="(element) in inputsAudio" :key="element.key">
            <div class="col-1 input" :class="$helper.socketConnectedClass(element.connections, element.socket.name)">
              <Socket v-socket:input="element" type="input" :socket="element.socket"></Socket>
            </div>
            <div class="slider-title-mixer row col-10">
            <div class="q-pt-xs slider-title col-8">CustomName</div>
            <q-badge class="slider-value col-4" color="white" text-color="black">
              {{ isVisible(element.key) ? parseFloat(node.data.volumes[(element.key/2)-1]).toFixed(3):'' }}
            </q-badge>
            <q-slider
              @input="setVolume((element.key/2)-1, $event)"
              :value="node.data.volumes[(element.key/2)-1]"
              :class="isVisible(element.key)?'':'hidden'"
              :min="0.0"
              :max="1.0"
              :step="0.1"
            ></q-slider>
            </div>
            <div class="col-12 input" :class="$helper.socketConnectedClass(getInputVariable(element.key).connections, 'variable')" :style="visibilityChange">
              <Socket v-socket:input="getInputVariable(element.key)" type="input" :socket="getInputVariable(element.key).socket"></Socket>
            </div>
          </div>
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
  components: {
    Socket: VueRenderPlugin.Socket
  },
  data () {
    return {
      nextSocketId: null,
      visibility: 'hidden',
      volumesPrev: null,
      value: true
    }
  },
  watch: {
  },
  methods: {
    getNodeClass () {
      return 'default-node-color'
    },
    async addInputs () {
      this.nextSocketId = this.node.data.nextSocketId
      console.log(this.nextSocketId)
      var inp2 = new Rete.Input(String(this.nextSocketId), 'variable', Socket.variable)
      this.node.addInput(inp2)
      var inp1 = new Rete.Input(String(++this.nextSocketId), 'audio', Socket.audio)
      this.node.addInput(inp1)
      await this.$store.state.rete.editor.trigger('process')
      // await this.$store.state.main.engine.process(this.$store.state.main.editor.toJSON())
      this.node.data.volumes.push(1)
      this.node.data.nextSocketId = ++this.nextSocketId
      console.log(this.nextSocketId)

      const payload = {
        nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id)
      }
      await this.$store.dispatch('main/createMixerSocket', payload)
      await this.$store.state.rete.editor.trigger('process')
    },
    async setVolume (index, value) {
      if (!this.node.data.prevVolumes) {
        this.node.data.prevVolumes = []
      }
      if (!this.node.data.prevVolumes[index]) {
        this.node.data.prevVolumes[index] = 1 // default value
      }
      this.node.data.volumes[index] = value
      await this.$store.dispatch('main/setMixerVolume', { idNode: this.node.data.newId || (!this.node.data.newId && this.node.id), index: index, volume: this.node.data.volumes[index], prevValue: this.node.data.prevVolumes[index] })
      if (this.node.data.prevVolumes[index] !== value) {
        this.node.data.prevVolumes[index] = value
      }
      this.$forceUpdate()
    },
    async removeInputs () {
      if (this.inputs().length > 4) {
        const input = this.inputs()[this.inputs().length - 1]
        const input2 = this.inputs()[this.inputs().length - 2]
        const inputkey = input.key
        if (inputkey === 1) {
          return null
        } else {
          console.log(input)
          input.connections.slice().map(this.$store.state.rete.editor.removeConnection.bind(this.$store.state.rete.editor))
          input2.connections.slice().map(this.$store.state.rete.editor.removeConnection.bind(this.$store.state.rete.editor))
          this.node.inputs.delete(input.key)
          this.node.inputs.delete(input2.key)
          input.node = null
          input2.node = null
          this.nextSocketId = this.nextSocketId - 2
          const payload = {
            nodeId: this.node.data.newId || (!this.node.data.newId && this.node.id)
          }
          await this.$store.dispatch('main/removeMixerSocket', payload)
          await this.$store.state.rete.editor.trigger('process')
        }
      }
    },
    getInputVariable (key) {
      return this.inputsVariable.find(item => item.key === String(key - 1))
    },
    async removeConnections () {
      const inputs = this.inputs()
      const variablesFilters = inputs.filter(element => element.name === 'variable')
      variablesFilters.forEach(item => {
        item.connections.slice().map(this.$store.state.rete.editor.removeConnection.bind(this.$store.state.rete.editor))
      })
      await this.$store.state.rete.editor.trigger('process')
    },
    isVisible (index) {
      const variable = this.getInputVariable(index)
      return variable && variable.connections.length === 0
    }
  },
  computed: {
    volumes () {
      return this.node.data.volumes
    },
    /* volumes: {
      get (index) {
        return this.node.data.volumes[index]
      },
      set (index, v) {
        this.$store.dispatch('main/setMixerVolume', { nodeId: this.node.data.newId || this.node.id, index: index, volume: this.node.data.volumes[index], prevValue: this.node.data.volumes[index] })
        this.node.data.volumes[index] = v
      }
    }, */
    currentValue (index) {
      var currentValue = Object.assign({}, this.node.data.volumes[index])
      return currentValue
    },
    inputsVariable () {
      console.log(this.nextSocketId)
      return this.inputs().filter(element => element.socket.name === 'variable')
    },
    inputsAudio () {
      console.log(this.nextSocketId)
      return this.inputs().filter(element => element.socket.name === 'audio')
    },
    visibilityChange () {
      if (!this.value) {
        return { visibility: 'hidden' }
      } else {
        return { visibility: 'inherit' }
      }
    },
    allowVariables: {
      get () {
        return this.value
      },
      set (val) {
        this.value = val
        if (!val) {
          this.removeConnections()
        }
      }
    }
  }
}
</script>
