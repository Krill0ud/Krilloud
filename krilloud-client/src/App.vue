<template>
  <div id="q-app" @click="cleanTemporalElementDrag">
    <router-view />
  </div>
</template>
<script>

import {
  mapGetters
} from 'vuex'
export default {
  name: 'App',
  async beforeMount () {
  },
  async mounted () {
    // window.addEventListener('focus', this.refreshPoolTree)
    await this.$store.dispatch('main/getVersionNumber')
    if (this.$q.localStorage.getItem(`krill-recents-${this.versionNumber}`)) {
      const recents = JSON.parse(this.$q.localStorage.getItem(`krill-recents-${this.versionNumber}`))
      this.$store.commit('project/setRecentProjects', recents)
    }
    // window.onbeforeunload = (event) => {
    //  this.$store.dispatch('project/close', {})
    // }
  },
  methods: {
    cleanTemporalElementDrag () {
      this.$store.commit('main/setTemporalElementDrag', { element: {}, type: null })
    },
    refreshPoolTree () {
      if (this.isOpenedProject) {
        this.$store.dispatch('audioPool/init')
      }
    }
  },
  computed: {
    versionNumber () {
      return this.$store.state.main.versionNumber
    },
    ...mapGetters('project', ['isOpenedProject'])
  },
  created () {
    this.$q.iconMapFn = (iconName) => {
      if (iconName.startsWith('icon') === true) {
        return {
          cls: 'icon ' + iconName
        }
      }
    }
  }
}
</script>
