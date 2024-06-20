<template lang="html">
  <div class="node-wrap">
  <div class="node filter" :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="row node-header">
<!-- Inputs-->
      <div class="col-1" :class="$helper.socketConnectedClass(inputAudio.connections, inputAudio.socket.name)">
            <Socket v-socket:input="inputAudio" type="input" :socket="inputAudio.socket"></Socket>
        </div>
        <div class="col-10 node-title q-pt-sm">{{ node.name }}</div>
        <div class="col-1">
<!-- Outputs-->
      <template v-for="output in outputs()">
        <div class="col-1 output" :key="output.key" :class="$helper.socketConnectedClass(output.connections, output.socket.name)">
              <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
          </div>
          </template>
        </div>
      </div>
<!-- Controls-->
<div class="q-mb-md">
<div class="row q-pt-xs q-pt-sm" @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
    <div class="q col-1" :class="$helper.socketConnectedClass(inputsVariable[0].connections, inputsVariable[0].socket.name)">
              <Socket v-socket:input="inputsVariable[0]" type="input" :socket="inputsVariable[0].socket"></Socket>
          </div>
    <div class="slider-title-mixer row col-10">
      <div class="q-pt-xs slider-title col-8">Wet</div>
          <q-badge class="slider-value col-4" color="white" text-color="black">
              {{ isVisible(0) ? wet:''}}
            </q-badge>
          <q-slider
              v-model="wet"
              :min="parameters.wet.minValue"
              :max="parameters.wet.maxValue"
              :step="0.001"
              :class="isVisible(0)?'':'hidden'"
            ></q-slider>
        </div>
      </div>
  <div class="row q-pt-xs q-pt-sm" @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
    <div class="q col-1" :class="$helper.socketConnectedClass(inputsVariable[1].connections, inputsVariable[1].socket.name)">
              <Socket v-socket:input="inputsVariable[1]" type="input" :socket="inputsVariable[1].socket"></Socket>
          </div>
    <div class="slider-title-mixer row col-10">
      <div class="q-pt-xs slider-title col-8">Delay</div>
          <q-badge class="slider-value col-4" color="white" text-color="black">
              {{ isVisible(1) ? delay:''}}
            </q-badge>
          <q-slider
              v-model="delay"
              :min="parameters.delay.minValue"
              :max="parameters.delay.maxValue"
              :step="0.001"
              :class="isVisible(1)?'':'hidden'"
            ></q-slider>
        </div>
      </div>

  <div class="row q-pt-xs q-pt-sm" @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
    <div class="q col-1" :class="$helper.socketConnectedClass(inputsVariable[2].connections, inputsVariable[2].socket.name)">
              <Socket v-socket:input="inputsVariable[2]" type="input" :socket="inputsVariable[2].socket"></Socket>
          </div>
    <div class="slider-title-mixer row col-10">
      <div class="q-pt-xs slider-title col-8">Decay</div>
          <q-badge class="slider-value col-4" color="white" text-color="black">
              {{ isVisible(2) ? decay:''}}
            </q-badge>
          <q-slider
              v-model="decay"
              :min="parameters.decay.minValue"
              :max="parameters.decay.maxValue"
              :step="0.001"
              :class="isVisible(2)?'':'hidden'"
            ></q-slider>
        </div>
      </div>
  <div class="row q-pt-xs q-pt-sm" @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
    <div class="q col-1" :class="$helper.socketConnectedClass(inputsVariable[3].connections, inputsVariable[3].socket.name)">
              <Socket v-socket:input="inputsVariable[3]" type="input" :socket="inputsVariable[3].socket"></Socket>
          </div>
    <div class="slider-title-mixer row col-10">
      <div class="q-pt-xs slider-title col-8">Filter</div>
          <q-badge class="slider-value col-4" color="white" text-color="black">
               {{ isVisible(3) ? filter:''}}
            </q-badge>
          <q-slider
              v-model="filter"
              :min="parameters.filter.minValue"
              :max="parameters.filter.maxValue"
              :step="0.001"
              :class="isVisible(3)?'':'hidden'"
            ></q-slider>
        </div>
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
  data () {
    return {
    }
  },
  methods: {
    getNodeClass () {
      return 'default-node-color'
    },
    change (input) {
    },
    visibilityChange () {
      const input = this.inputsVariable[0].connections.length
      // var connection = this.inputsVariable[0].connections
      if (input > 0) {
        return { visibility: 'hidden' }
      } else {
        return { visibility: 'inherit' }
      }
    },
    isVisible (index) {
      const variable = this.inputsVariable[index]
      return variable && variable.connections.length === 0
    }
  },
  computed: {
    parameters () {
      return this.node.data
    },
    inputsVariable () {
      return this.inputs().filter(element => element.socket.name === 'variable')
    },
    inputAudio () {
      return this.inputs().find(element => element.socket.name === 'audio')
    },
    currentTagName () {
      return this.$store.state.tag.currentTag.name
    },
    datafilter () {
      return this.node.data
    },
    idNode () {
      return this.node.data.newId || (!this.node.data.newId && this.node.id)
    },
    wet: {
      get () {
        return parseFloat(this.datafilter.wet.value.toFixed(3))
      },
      set (v) {
        this.$store.dispatch('main/setFilterParameter', { id: 0, value: v, tagname: this.currentTagName, idNode: this.idNode, name: 'wet', prevValue: this.datafilter.wet.value })
        this.datafilter.wet.value = v
      }
    },
    delay: {
      get () {
        return parseFloat(this.datafilter.delay.value.toFixed(3))
      },
      set (v) {
        this.$store.dispatch('main/setFilterParameter', { id: 1, value: v, tagname: this.currentTagName, idNode: this.idNode, name: 'delay', prevValue: this.datafilter.delay.value })
        this.datafilter.delay.value = v
      }
    },
    decay: {
      get () {
        return parseFloat(this.datafilter.decay.value.toFixed(3))
      },
      set (v) {
        this.$store.dispatch('main/setFilterParameter', { id: 2, value: v, tagname: this.currentTagName, idNode: this.idNode, name: 'decay', prevValue: this.datafilter.decay.value })
        this.datafilter.decay.value = v
      }
    },
    filter: {
      get () {
        return parseFloat(this.datafilter.filter.value.toFixed(3))
      },
      set (v) {
        this.$store.dispatch('main/setFilterParameter', { id: 3, value: v, tagname: this.currentTagName, idNode: this.idNode, name: 'filter', prevValue: this.datafilter.filter.value })
        this.datafilter.filter.value = v
      }
    }

  }
}
</script>
