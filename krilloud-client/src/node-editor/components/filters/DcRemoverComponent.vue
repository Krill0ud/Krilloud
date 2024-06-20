<template lang="html">
  <div class="node-wrap">
  <div class="node filter" :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="row node-header">
<!-- Inputs-->
      <div class="q col-1" :class="$helper.socketConnectedClass(inputAudio.connections, inputAudio.socket.name)">
            <Socket v-socket:input="inputAudio" type="input" :socket="inputAudio.socket"></Socket>
        </div>
        <div class="node-title q-pt-sm col-10">{{ node.name }}</div>
        <div class="col-1">
<!-- Outputs-->
<template v-for="output in outputs()">
        <div class="output" :key="output.key" :class="$helper.socketConnectedClass(output.connections, output.socket.name)">
              <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
          </div>
          </template>
        </div>
      </div>
<!-- Controls-->
<div class="q-mb-md">
<div class="row q-pt-xs q-pt-sm" @pointerdown.stop="" @click="$helper.callNodeSelected(node)">
    <div class="q col-1" :class="$helper.socketConnectedClass(inputVariable.connections, inputVariable.socket.name)">
              <Socket v-socket:input="inputVariable" type="input" :socket="inputVariable.socket"></Socket>
          </div>
    <div class="slider-title-mixer row col-10">
      <div class="q-pt-xs slider-title col-8">Wet</div>
          <q-badge class="slider-value col-4" color="white" text-color="black">
               {{ isVisible() ? wet:''}}
            </q-badge>
          <q-slider
              v-model="wet"
              :min="parameters.wet.minValue"
              :max="parameters.wet.maxValue"
              :step="0.001"
              :class="isVisible()?'':'hidden'"
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
    isVisible () {
      const variable = this.inputVariable
      return variable && variable.connections.length === 0
    }
  },
  computed: {
    parameters () {
      return this.node.data
    },
    inputVariable () {
      return this.inputs().find(element => element.socket.name === 'variable')
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
    }
  }
}
</script>
