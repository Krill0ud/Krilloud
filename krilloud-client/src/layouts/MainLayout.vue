<template>
  <q-layout view="hHh lpR fFf">
    <q-header>
      <main-bar title='Krilloud'/>
    </q-header>

  <keep-alive>
  <q-drawer class="q-drawer-sidebar" show-if-above  side="left" behavior="desktop" bordered :width="300">
    <audio-pool-viewer v-if="showAudioPoolViewer"></audio-pool-viewer>
    <q-splitter v-else
      v-model="splitterModel"
      style="height: 92.5%"
      horizontal
      before-class="scroll"
    >
      <template v-slot:before>
          <tag-side-bar/>
      </template>

      <template v-slot:after>
        <q-splitter
          v-model="insideModel"
          horizontal
          before-class="scroll"
          after-class="scroll"
        >
          <template v-slot:before>
            <variable-side-bar/>
          </template>

          <template v-slot:after>
            <audio-pool-side-bar/>
          </template>
          </q-splitter>
        </template>
      </q-splitter>
    </q-drawer>
  </keep-alive>
    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script>
import MainBar from 'components/MainBar.vue'
import TagSideBar from 'components/sidebars/TagSideBar.vue'
import VariableSideBar from 'components/sidebars/VariableSideBar.vue'
import AudioPoolSideBar from 'components/sidebars/AudioPoolSideBar.vue'
import AudioPoolViewer from 'components/sidebars/AudioPoolViewer'

export default {
  name: 'MainLayout',
  components: { MainBar, TagSideBar, VariableSideBar, AudioPoolSideBar, AudioPoolViewer },
  data () {
    return {
      leftDrawerOpen: true,
      splitterModel: 30, // start at 50%
      insideModel: 50

    }
  },
  async mounted () {
    const project = await this.$store.dispatch('project/getOpenedProject')
    if (project && project.name) {
      this.$store.dispatch('project/open', `${project.folderProject}/${project.name}.kpj`)
    } else {
      this.$store.commit('project/setShowOpenProjectModal', true)
    }
  },
  computed: {
    showAudioPoolViewer: {
      get () {
        return this.$store.state.audioPool.showAudioPoolViewer
      },
      set (v) {
        this.$store.commit('audioPool/setshowAudioPoolViewer', v)
      }
    }
  }
}
</script>
