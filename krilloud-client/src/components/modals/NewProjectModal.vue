<template>
      <q-dialog ref="refNewProjectModal" :value="showNewProjectModal" @input="$emit('update', $event)" persistent>
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showNewProjectModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-1 text-h6 dialog-header">
              <q-icon dense flat>
                <img src="~assets/icons/Krilloud_icon.png" />
              </q-icon>
            </div>
            <div class="col-10 text-h6 dialog-header">New Project</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3"  @click="close()" flat dense v-close-popup/>
          </q-card-section>

          <q-card-section class="q-pa-lg text-modal">
            <div>Project Name</div>
            <q-input v-model="project.name" borderless color="white" bg-color="white" dense />
          </q-card-section>
          <q-card-section class="q-pa-lg text-modal">
            <div>Krilloud Project Folder</div>

          <div class="row q-pt-lg items-baseline justify-around">
              <div  class="q-col-1">
              <q-btn flat round icon="icon-folder-closed" size="md"  @click="getProjectFile" class='q-mr-md q-tree-btn'></q-btn>
              </div>
              <div class="q-col-10" style="width: 80%">
              <div class="self-center q-pt-md" style="overflow-wrap: anywhere">{{project.folderProject?project.folderProject:'No folder selected'}}</div>
              </div>
            </div>
          </q-card-section>
          <q-card-section class="q-pa-lg text-modal">
            <div>Game Project folder</div>
            <div class="row q-pt-lg items-baseline justify-around">
              <div  class="q-col-1">
              <q-btn flat round icon="icon-folder-closed" size="md" @click="getGameFile" class='q-mr-md q-tree-btn'></q-btn>
              </div>
              <div class="q-col-10" style="width: 80%">
                <div class="self-center q-pt-md" style="overflow-wrap: anywhere">{{project.folderGame?project.folderGame:'No folder selected'}}</div>
              </div>
            </div>
            <ErrorValidate :message="message" />
          </q-card-section>

          <q-card-actions align="center" class="q-pb-lg">
            <q-btn unelevated rounded  label="CREATE PROJECT" class="my-button-sidebar" @click="createProject" />

          </q-card-actions>
        </q-card>
      </q-dialog>
</template>

<script>
import {
  mapGetters
} from 'vuex'
import ErrorValidate from '../commons/ErrorValidate.vue'
export default {
  props: { showNewProjectModal: Boolean },
  components: {
    ErrorValidate
  },
  data () {
    return {
      project: {
        folderGame: '',
        folderProject: '',
        name: ''
      },
      gameFile: '',
      projectFile: '',
      message: []
    }
  },
  watch: {
    showNewProjectModal (val) {
      if (val) {
        this.project = {
          folderGame: '',
          folderProject: '',
          name: ''
        }
      }
    }
  },
  activated () {
    this.project = {
      folderGame: '',
      folderProject: '',
      name: ''
    }
  },
  mounted () {

  },
  computed: {
    ...mapGetters('project', ['isOpenedProject'])
  },
  methods: {
    checkProject () {
      if (!this.isOpenedProject) {
        this.$store.commit('project/setShowOpenProjectModal', true)
      }
    },
    async createProject () {
      this.validateFolder()
      if (this.message.length === 0) {
        await this.$store.dispatch('project/saveNewProject', this.project)
        this.$refs.refNewProjectModal.hide()
      } else {
        return this.message
      }
    },
    getGameFile () {
      const { dialog } = require('electron').remote
      const projectObj = this.project
      dialog.showOpenDialog({ properties: ['openDirectory'] }).then(function (response) {
        if (!response.canceled) {
          projectObj.folderGame = response.filePaths[0]
        } else {
          // console.log('no file selected')
        }
      })
    },
    getProjectFile () {
      const { dialog } = require('electron').remote
      const projectObj = this.project
      dialog.showOpenDialog({ properties: ['openDirectory'] }).then(function (response) {
        if (!response.canceled) {
          projectObj.folderProject = response.filePaths[0]
        } else {
          // console.log('no file selected')
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
    },
    validateFolder () {
      this.message = []
      if (!this.project.folderGame) {
        var folderGameError = 'You need to specify a Game Project Folder'
        this.message.push(folderGameError)
      }
      if (!this.project.folderProject) {
        var folderProjectError = 'You need to specify a Krilloud Project Folder'
        this.message.push(folderProjectError)
      }
      if (!this.project.name) {
        var projectNameError = 'You need to specify a Project Name'
        this.message.push(projectNameError)
      }
      return this.message
    },
    close () {
      this.message = []
      if (!this.isOpenedProject) {
        this.$store.commit('project/setShowOpenProjectModal', true)
      }
    }

  }
}
</script>
