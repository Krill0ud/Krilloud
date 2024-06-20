<template>
  <div class="q-gutter-sm non-selectable">
    <div class="hidden" v-shortkey="['ctrl', 'n']" @shortkey="askBeforeSave(newProject)"></div>
    <div class="hidden" v-shortkey="['ctrl', 'o']" @shortkey="askBeforeSave(openProject)"></div>
    <div class="hidden" v-shortkey="['ctrl', 's']" @shortkey="saveProject()"></div>
    <div class="hidden" v-shortkey="['ctrl', 'p']" @shortkey="optionsProject()"></div>
    <div class="hidden" v-shortkey="['ctrl', 'shift', 's']" @shortkey="askBeforeSave(saveProjectAs)"></div>
    <div class="hidden" v-shortkey="['ctrl', 'm']" @shortkey="channels()"></div>
    <div class="hidden" v-shortkey="['ctrl', 'e']" @shortkey="exportProject()"></div>
    <div class="hidden" v-shortkey="['ctrl', 'c']" @shortkey="copy"></div>
    <div class="hidden" v-shortkey="['ctrl', 'v']" @shortkey="paste"></div>
    <div class="hidden" v-shortkey="['ctrl', 'd']" @shortkey="clone"></div>
    <div class="hidden" v-shortkey="['del']" @shortkey="deleteSelection"></div>
    <q-bar class="bg-base text-bar">
      <div class="menuBar non-selectable" @mouseover="overMenu('menuFile')">File
        <q-menu anchor="bottom end" self="top right" content-class="menu-bar" v-model="menuFile" ref="menuFile" style="width: 200px">
          <q-list dense>
              <q-item clickable v-close-popup>
                <q-item-section @click="askBeforeSave(newProject)"
                  >New Project
                  </q-item-section>
                 <q-item-section avatar style="font-size: 12px">Ctrl + N</q-item-section>
                </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="askBeforeSave(openProject)">Open
                  </q-item-section>
                  <q-item-section avatar style="font-size: 12px">Ctrl + O</q-item-section>
                </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="askBeforeSave(closeProject)">Close
                  </q-item-section>
                </q-item>
            <q-separator />
              <q-item clickable v-close-popup>
                <q-item-section @click="saveProject()">Save
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Ctrl + S</q-item-section>
              </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="askBeforeSave(saveProjectAs)">Save As
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Ctrl+shift+S</q-item-section>
              </q-item>
          </q-list>
        </q-menu>
      </div>
      <div class="menuBar non-selectable" @mouseover="overMenu('menuEdit')">Edit
        <q-menu anchor="bottom end" self="top right" content-class="menu-bar" v-model="menuEdit" ref="menuEdit">
        <q-list dense style="min-width: 100px">
              <q-item clickable v-close-popup>
                <q-item-section @click="copy" class="row items-baseline justify-around">
                  Copy
                  </q-item-section>
                  <q-item-section avatar style="font-size: 12px">Ctrl + C</q-item-section>
                </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="paste">Paste
                  </q-item-section>
                  <q-item-section avatar style="font-size: 12px">Ctrl + V</q-item-section>
                </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="clone">Clone
                  </q-item-section>
                  <q-item-section avatar style="font-size: 12px">Ctrl + D</q-item-section>
                </q-item>
            <q-separator />
            <q-item clickable v-close-popup>
                <q-item-section @click="undo">Undo
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Ctrl + Z</q-item-section>
              </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="redo">Redo
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Ctrl + Y</q-item-section>
              </q-item>
              <q-item clickable v-close-popup>
                <q-item-section @click="deleteSelection">Delete
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Delete</q-item-section>
              </q-item>
          </q-list>
        </q-menu>
      </div>
      <div class="menuBar non-selectable" @mouseover="overMenu('menuProject')" style="width: 60px">Project
          <q-menu anchor="bottom end" self="top middle" content-class="projectMenuBar" v-model="menuProject" ref="menuProject">
          <q-list dense class="">
            <q-item clickable v-close-popup>
              <q-item-section @click="optionsProject()">Options
              </q-item-section>
              <q-item-section avatar style="font-size: 12px">Ctrl+P</q-item-section>
            </q-item>
            <q-item clickable v-close-popup>
              <q-item-section @click="channels()">Channels
              </q-item-section>
              <q-item-section avatar style="font-size: 12px">Ctrl+M</q-item-section>
            </q-item>
            <q-separator />
              <q-item clickable v-close-popup>
                <q-item-section @click="exportProject()">Export
                </q-item-section>
                <q-item-section avatar style="font-size: 12px">Ctrl+E</q-item-section>
              </q-item>
          </q-list>
        </q-menu>
      </div>
      <div></div>
      <div class="menuBar non-selectable realTime" @click="openClient()" style="width: 100px">Real Time
         <q-icon v-if="realTimeState" size="15px" name="circle" color="positive"/>
      </div>
      <q-space />
      <div class="name-bar">{{projectName}}</div>
      <q-space />
      <div>{{versionNumber}}</div>
    </q-bar>
    <ChannelsModal :showChannelsModal="showChannelsModal" @update="showChannelsModal = $event"/>

    <q-dialog v-model="closeSave">
      <q-card class="bg-modal" style="width:450px;height:auto">
        <q-card-section class="q-pa-lg text-modal">
          <div class="col-10 text-h6 text-center">Do you want to save before continuing?</div>
        </q-card-section>

        <q-card-actions align="center" class="q-pb-lg">
          <q-btn unelevated rounded  class="my-button-sidebar" label="Yes" @click="saveBefore(true)" v-close-popup />
          <q-btn unelevated rounded  class="my-button-sidebar" label="No" @click="saveBefore(false)" v-close-popup />
        </q-card-actions>
      </q-card>
    </q-dialog>
    <SaveAsProjectModal :showSaveAsProjectModal="showSaveAsProjectModal" @update="showSaveAsProjectModal = $event" />
    <RealTimeModal :showRealTimeModal="showRealTimeModal" @update="showRealTimeModal = $event" />

  </div>
</template>

<script>
import ChannelsModal from '../components/modals/ChannelsModal'
import SaveAsProjectModal from '../components/modals/SaveAsProjectModal'
import RealTimeModal from '../components/modals/RealTimeModal'

import { debounce } from 'quasar'

export default {
  name: 'MainBar',
  components: { ChannelsModal, SaveAsProjectModal, RealTimeModal },
  props: {
    title: {
      type: String,
      required: true
    }
  },
  data: function () {
    return {
      showRealTimeModal: false,
      showChannelsModal: false,
      showOptionsProjectModal: false,
      temporalProject: null,
      menu: false,
      menuFile: false,
      menuEdit: false,
      menuProject: false,
      menuRealTime: false,
      menuOver: false,
      closeSave: false,
      fcCloseSave: null
    }
  },
  computed: {
    versionNumber () {
      return this.$store.state.main.versionNumber
    },
    project () {
      return this.$store.state.project.project
    },
    projectName () {
      return this.$store.state.project.project.name
    },
    showSaveAsProjectModal: {
      get () {
        return this.$store.state.project.showSaveAsProjectModal
      },
      set (v) {
        this.$store.commit('project/setShowSaveAsProjectModal', v)
      }
    },
    realTimeState () {
      return this.$store.state.main.realTimeState
    }

  },
  methods: {
    overMenu (reference) {
      if (this.menuFile || this.menuProject || this.menuRealTime || this.menuEdit) {
        this.menuFile = this.menuProject = this.menuRealTime = this.menuEdit = false
        this.$refs[reference].show()
      }
    },
    askBeforeSave (fc) {
      this.closeSave = true
      this.fcCloseSave = fc
    },
    async saveBefore (decision) {
      if (decision) {
        await this.saveProject()
      }
      this.fcCloseSave()
    },
    newProject () {
      this.$store.commit('project/setShowNewProjectModal', true)
    },
    openProject () {
      this.$store.commit('project/setShowOpenProjectModal', true)
    },
    closeProject () {
      this.$store.dispatch('project/close', this.project)
    },
    saveProject () {
      this.$store.dispatch('project/save', this.project)
      this.$q.notify({
        type: 'positive',
        message: 'The project was saved'
      })
    },
    saveProjectAs () {
      this.showSaveAsProjectModal = true
    },
    channels () {
      this.showChannelsModal = true
    },
    optionsProject () {
      this.$store.commit('project/showOptionsProjectModal', true)
    },
    mouseOverEvt (event) {
      /* */
    },
    checkMenu () {
      if (this.menuOver) {
        this.menu = true
      } else {
        this.menu = false
      }
    },
    debounceFunc () {
      debounce(this.checkMenu(), 300)
    },
    exportProject () {
      this.$store.commit('project/showExportProjectModal', true)
    },
    async openClient () {
      if (this.realTimeState) {
        const objectList = await this.$store.dispatch('main/getIngameObjectList')
        this.$store.commit('main/setRealtimeObjectList', objectList)
        const playingTagList = await this.$store.dispatch('main/getPlayingTagsList')
        this.$store.commit('main/setRealtimeTagsList', playingTagList)
      } else {
        this.$store.commit('main/setRealtimeObjectList', [])
        this.$store.commit('main/setRealtimeTagsList', [])
      }
      this.showRealTimeModal = true
      this.$store.commit('main/setIsModal', true)
    },
    copy () {
      this.$helper.copyToClipboard()
      this.$q.notify({
        type: 'positive',
        message: 'Selected nodes copied',
        icon: 'content_copy'
      })
    },
    paste () {
      this.$helper.pasteFromClipboard()
    },
    deleteSelection () {
      this.$helper.deleteSelection()
    },
    clone () {
      this.$helper.clone()
    },
    undo () {
      this.$store.state.rete.editor.trigger('undo')
    },
    redo () {
      this.$store.state.rete.editor.trigger('redo')
    }
  },
  watch: {
    menuOver (val) {
      this.debounceFunc()
    }
  }
}
</script>
