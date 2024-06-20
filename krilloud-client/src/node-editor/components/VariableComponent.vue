<template lang="html">
  <div class="node-wrap">
  <div class="node" :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="row node-header">
        <div class="node-title q-pt-sm col-11">{{variableName}}</div>
        <div class="q col-1">
        <!-- Outputs-->
         <template v-for="output in outputs()">
        <div class="output outVariable" :key="output.key" :class="$helper.socketConnectedClass(output.connections, output.socket.name)">
              <Socket v-socket:output="output" type="output" :socket="output.socket"></Socket>
          </div>
           </template>
        </div>
      </div>
        <!-- Controls-->
    <div class="row q-pa-md" @pointerdown.stop="">
    <div class="slider-title col-8">Simulated value:</div>
      <q-badge class="slider-value col-4" color="white" text-color="black">
        {{ simulated }}
      </q-badge>
      <q-slider
        v-model="simulated"
        :min="parseFloat(variableInstance.minRange)"
        :max="parseFloat(variableInstance.maxRange)"
        :step="onlyIntegerOption"
      ></q-slider>
    <div class="row justify-between checkbox-title q-pt-sm">
    <div class="q-pt-xs col-10">Only integers:</div>
    <div align="right" class="col-2"><q-checkbox keep-color left-label size="14px" color="grey-5" v-model="onlyIntegers"></q-checkbox></div>
    <div class="q-pt-xs col-10">Set on play only</div>
    <div align="right" class="col-2"><q-checkbox keep-color left-label size="14px" color="grey-5" v-model="onPlayOnly"></q-checkbox></div>
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
  computed: {
    onlyIntegerOption () {
      if (this.onlyIntegers) {
        return 1
      } else {
        return 0.100
      }
    },
    variableInstance () {
      return this.$store.state.variable.variables.find(item => item.id === this.node.data.id)
    },
    variableNode () {
      return this.node.data
    },
    variableName: {
      get () {
        return this.variableInstance.name
      },
      set (v) {
        this.$store.dispatch('variable/setAttribute', { name: this.variableName, value: v, attribute: 'name' })
        this.variableInstance.name = v
      }
    },
    onlyIntegers: {
      get () {
        return this.variableInstance.onlyIntegers
      },
      set (v) {
        this.variableInstance.onlyIntegers = v
      }
    },
    onPlayOnly: {
      get () {
        return this.variableInstance.onPlayOnly
      },
      set (v) {
        this.$store.dispatch('variable/setAttribute', { name: this.variableName, value: v, attribute: 'on-play' })
        this.variableInstance.onPlayOnly = v
      }
    },
    simulated: {
      get () {
        return this.variableInstance.simulated
      },
      set (v) {
        this.$store.dispatch('variable/setAttribute', { name: this.variableName, value: v, attribute: 'value' })
        this.variableInstance.simulated = v
      }
    }
  },
  watch: {
  },
  mounted () {
  },
  methods: {
    getNodeClass () {
      return 'default-node-color'
    }
  }
}
</script>
