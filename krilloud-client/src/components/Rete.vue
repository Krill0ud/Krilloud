<template>
  <div id="rete" v-show="visible" ref="rete" class='node-editor'></div>
</template>

<script>
import init from '../node-editor'

export default {
  data () {
    return {
      visible: true
    }
  },
  methods: {
  },
  mounted () {
    init(this.$refs.rete, this.$store)
  }
}
</script>
