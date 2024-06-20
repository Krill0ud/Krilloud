<template lang="html">
  <div class="node-triangle">
  <div :class="[selected(), node.name, getNodeClass()] | kebab">
    <div class="node-header">
          <!-- Inputs-->
          <template v-for="input in inputs()">
            <div class="input bg-output"  :key="input.key" :class="$helper.socketConnectedClass(input.connections, input.socket.name)">
                  <Socket v-socket:input="input" type="input" :socket="input.socket"></Socket>
            </div>
            </template>
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
  methods: {
    getNodeClass () {
      return 'default-node-color'
    }
  }
}
</script>

<style scoped>
.bg-output {
  background: url('../../assets/img/Output.png') no-repeat;
  width: 71px;
  height: 83px;
  padding-top: 30px;
  padding-left: 10px;
}

</style>
