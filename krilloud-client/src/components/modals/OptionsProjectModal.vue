<template>
      <q-dialog :value="showOptionsProjectModal" @input="$emit('update', $event)" persistent>
        <q-card class="scroll bg-modal non-selectable" style="width:450px;height:auto" v-if="showOptionsProjectModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-1 text-h6 dialog-header">
              <q-icon dense flat>
                <img src="~assets/icons/Krilloud_icon.png" />
              </q-icon>
            </div>
            <div class="col-10 text-h6 dialog-header">Project options</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3" @click="close()" flat  dense v-close-popup/>
          </q-card-section>

          <q-card-section class="q-pa-lg text-modal">
            <div class="q-pb-xs">Project Name</div>
            <q-input square  standout bg-color="white" v-model="currentProject.name" dense disable />
          </q-card-section>
          <q-card-section class="q-pa-lg text-modal">
            <div>Export path</div>
            <div class="row q-pt-lg items-baseline justify-around">
              <div  class="q-col-1">
                <q-btn flat round icon="icon-folder-closed"  @click="getProjectFile" size="md" class="q-tree-btn" />
                </div>
              <div class="q-col-10" style="width: 80%">
                <div class="self-center" style="overflow-wrap: anywhere; display: inline-flex">{{project.folderGame?project.folderGame:this.currentProject.folderGame}}</div>
              </div>
            </div>
           <div class="q-mt-md">

            </div>
         </q-card-section>
          <q-card-actions align="center" class="q-pb-lg">
            <q-btn v-close-popup  unelevated rounded  label="SAVE" class="my-button-sidebar" @click="saveProject()" />
          </q-card-actions>
        </q-card>
      </q-dialog>
</template>

<script>

import {

} from 'vuex'
export default {
  props: { showOptionsProjectModal: Boolean },
  data () {
    return {
      newName: '',
      newProjectFolder: '',
      project: {
        folderGame: '',
        folderProject: '',
        name: ''
      }
    }
  },
  watch: {
    showNewProjectModal (val) {
      if (val) {
        this.project = {
          folderProject: '',
          folderGame: this.currentProject.folderGame,
          name: this.projectname
        }
      }
    }
  },
  computed: {
    currentProject () {
      return this.$store.state.project.project
    },
    projectname () {
      var project = Object.assign({}, this.currentProject)
      return project.name
    }
  },
  methods: {
    changeName (val) {
      this.newName = val
    },
    saveProject () {
      this.$store.dispatch('project/changeProjectName', this.newName)
      this.$store.dispatch('project/changeProjectPath', this.project.folderGame)
    },
    close () {

    },
    getProjectFile () {
      const { dialog } = require('electron').remote
      const projectObj = this.project
      dialog.showOpenDialog({ properties: ['openDirectory'] }).then(function (response) {
        if (!response.canceled) {
          projectObj.folderGame = response.filePaths[0]
        } else {
          console.log('no file selected')
        }
      })
    },
    onRejected (rejectedEntries) {
      // Notify plugin needs to be installed
      // https://quasar.dev/quasar-plugins/notify#Installation
      this.$q.notify({
        type: 'negative',
        message: `${rejectedEntries.length} file(s) did not pass validation constraints`
      })
    }
  }
}
</script>
