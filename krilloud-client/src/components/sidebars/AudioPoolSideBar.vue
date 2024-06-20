<template>
  <q-card class="tag-section non-selectable" square>
    <q-card-section class="sidebar row items-baseline justify-around q-pr-sm my-sticky">
      <div class="sidebar-title col-5">
        <div align="left">AUDIO POOL</div>
      </div>
      <div class="text-title col-7" align="right">
        <q-btn flat round icon="cached" class="q-tree-title" size="md" @click="refreshPoolTree()"/>
        <q-btn flat round icon="icon-folder-closed" class="q-tree-btn" size="md" @click="initCreateSubfolder(activeItems[0])"/>
        <q-btn flat round icon="icon-icon-add-small" class="q-tree-btn" size="md" @click="initAddFiles(activeItems[0])"/>
      </div>
      </q-card-section>
      <q-card-section v-if="!treeData" class="text-center content-center text-tag tag-section">
          <p>You have no audios</p>
      </q-card-section>
      <br/>
      <br/>
      <q-card-section class="tex-tag">
      <div v-if="treeData">
        <q-input
          ref="filter"
          @focus="setModeEditing()"
          @blur="setNoEditing()"
          v-model="filterText"
          color="white"
          bg-color="white"
          label-color= "primary"
          filled
          @input="filterTree"
          dense
          class="q-pb-md">
          <template v-slot:append>
            <q-icon v-if="filterText !== ''" name="clear" class="cursor-pointer" @click="resetFilter" />
            </template>
            <template v-slot:append>
            <q-icon name="search"/>
          </template>
        </q-input>
    <vue-tree-list
      class="tag-section"
      @click="onClick"
      @change-name="onChangeName"
      @delete-node="onDel"
      @add-node="onAddNode"
      @drop="drop"
      @drop-before="dropBefore"
      @drop-after="dropAfter"
      :model="treeData"
      default-tree-node-name="New Folder"
      default-leaf-node-name="New File"
      v-bind:default-expanded="true"
    >
      <template v-slot:leafNameDisplay="slotProps" draggable="true"  @dragstart="startDrag($event, slotProps.model)">
        <div draggable="true"  @dragstart="startDrag($event, slotProps.model)">
          <q-menu v-if="slotProps.model.isLeaf"
            touch-position
            context-menu
          >
          <q-list dense style="min-width: 150px">
            <q-item clickable v-close-popup @click="onDel(slotProps.model, false)">
              <q-item-section class="q-pa-xs">Delete</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="onRenameAudio(slotProps.model)">
              <q-item-section class="q-pa-xs">Rename</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="showInExplorer(slotProps.model)">
              <q-item-section class="q-pa-xs">Show In Explorer</q-item-section>
            </q-item>
          </q-list>
          </q-menu>
          <q-menu v-if="!slotProps.model.isLeaf"
            touch-position
            context-menu
          >
          <q-list dense style="min-width: 150px">
            <q-item clickable v-close-popup @click="initAddFiles(slotProps.model)">
              <q-item-section class="q-pa-xs">Add files</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="initCreateSubfolder(slotProps.model)">
              <q-item-section class="q-pa-xs">Create Subfolder</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="onDel(slotProps.model, true)" v-if="!slotProps.model.root" >
              <q-item-section class="q-pa-xs">Delete subfolder</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="showInExplorer(slotProps.model)">
              <q-item-section class="q-pa-xs">Show In Explorer</q-item-section>
            </q-item>
          </q-list>
          </q-menu>
          <q-item  clickable v-ripple :active="activeItems.findIndex(item => item.id === slotProps.model.id) > -1" active-class="bg-grey-13" @click="addActiveItem(slotProps.model, $event)" draggable="true">
              <q-item-section avatar class="q-pr-sm"  draggable="true" style="display: inline-flex">
                <q-icon size="12px" class="q-pl-xs" name="folder" color="blue-grey-7" v-if="!slotProps.model.isLeaf"/>
              </q-item-section>
              <q-item-section style="overflow-wrap: anywhere;" draggable="true">
                {{ slotProps.model.name }}
              </q-item-section>
              <q-item-section side>
                <q-btn class="q-tree-btn-launch" icon="launch" size="1px" flat @click="launchAudio(slotProps.model)"></q-btn>
              </q-item-section>
          </q-item>
        </div>
      </template>
      <q-icon name="" slot="addLeafNodeIcon" color="blue-grey-7"/>
      <q-icon name="create_new_folder" slot="addTreeNodeIcon" color="blue-grey-7"/>
      <q-icon name="mode_edit" slot="editNodeIcon" color="blue-grey-7"/>
      <q-icon name="icon-icon--delete" slot="delNodeIcon" color="blue-grey-7"/>
      <q-icon name="" slot="leafNodeIcon" color="blue-grey-7"/>
      <q-icon name="" slot="treeNodeIcon" color="blue-grey-7"/>
    </vue-tree-list>
  </div>
      </q-card-section>
    <DeleteElementModal :showDeleteElementModal="showDeleteElementModal" @update="showDeleteElementModal = $event" :hideMessage="hideMessage" :audioDelete="true" :variableDelete="false" :audioName="audioName" :message="message" @delete="deleteAudio()" @cancel="closeModal()" @close="closeModal()"/>
    <RenameAudioModal :showRenameAudioModal="showRenameAudioModal" @update="showRenameAudioModal = $event"  :audio="audio" @onChangeName="onChangeName" @cancel="showRenameAudioModal=false" @cancelModal="closeRenameModal()" @close="closeRenameModal()"/>
    <CreateSubfolderModal :showCreateSubfolderModal="showCreateSubfolderModal"  @update="showCreateSubfolderModal = $event" :audio="audio" @onCreateSubfolder="onCreateSubfolder" @cancel="closeCreateFolderModal()" @close="closeCreateFolderModal()"/>
  </q-card>
</template>

<script>
import { VueTreeList, Tree, TreeNode } from 'vue-tree-list'
import DeleteElementModal from '../modals/DeleteElementModal'
import RenameAudioModal from '../modals/RenameAudioModal'
import {
  mapGetters
} from 'vuex'
import CreateSubfolderModal from '../modals/CreateSubfolderModal'

export default {
  name: 'AudioPoolSideBar',
  props: {

  },
  components: {
    VueTreeList,
    DeleteElementModal,
    RenameAudioModal,
    CreateSubfolderModal
  },
  mounted () {
    if (!this.treeData && this.$store.state.audioPool.audios) {
      this.treeData = new Tree(this.$store.state.audioPool.audios)
    }
    var img = new Image()
    img.src = require('../../assets/img/audio-connector.png')
    this.dragImg = img
  },
  data: function () {
    return {
      showAddAudioModal: false,
      showRenameAudioModal: false,
      showDeleteElementModal: false,
      showCreateSubfolderModal: false,
      treeData: null, // new Tree(this.audios)
      treeOriginal: null,
      hideMessage: false,
      audio: {},
      isfolder: false,
      message: [],
      filterText: '',
      activeItems: [],
      activeRef: null,
      ctrlKey: false,
      dragImg: null,
      audioName: ''
    }
  },
  computed:
  {
    folderPathProject () {
      return this.$store.state.project.project.folderProject
    },
    ...mapGetters('project', ['isOpenedProject'])
  },
  watch: {
    '$store.state.audioPool.audios': function (val) {
      this.treeData = new Tree(val)
      this.treeOriginal = new Tree(val)
      // this.activeItem = this.treeOriginal.children[0]
      // this.activeItems = []
      // .activeItems.push(this.treeOriginal.children[0])
    }
  },
  methods: {
    addActiveItem (param, event) {
      this.ctrlKey = event.ctrlKey
      const activeItem = this.activeItems.findIndex(item => item.id === param.id)
      if (activeItem > -1) {
        this.activeItems.splice(activeItem, 1)
      } else if (!event.ctrlKey) {
        this.activeItems = []
      } else {
        this.activeItems.push(param)
      }
    },
    setModeEditing () {
      this.$store.commit('main/setModeEditing', true)
    },
    setNoEditing () {
      this.$store.commit('main/setModeEditing', false)
    },
    onDel (node, folder) {
      this.isFolder = folder
      this.showDeleteElementModal = true
      this.audio = node
      if (this.activeItems.length > 0) {
        this.audioName = 'all the selected files'
      } else {
        this.audioName = this.audio.name
      }
    },
    async deleteAudio () {
      if (this.activeItems.length > 0) {
        this.activeItems.forEach(async item => {
          if (item.isLeaf) {
            this.$store.dispatch('audioPool/removeFile', item.fullPath)
            await item.remove()
          } else {
            this.$store.dispatch('audioPool/removeDirectory', item.fullPath)
            await item.remove()
          }
        })
        this.activeItems = []
        this.showDeleteElementModal = false
      } else {
        if (!this.isFolder) {
          this.$store.dispatch('audioPool/removeFile', this.audio.fullPath)
          await this.audio.remove()
          this.showDeleteElementModal = false
        } else {
          this.$store.dispatch('audioPool/removeDirectory', this.audio.fullPath)
          await this.audio.remove()
          this.showDeleteElementModal = false
        }
      }
    },
    onRenameAudio (params) {
      this.audio = params
      this.showRenameAudioModal = true
      this.$store.commit('main/setIsModal', true)
    },
    showInExplorer (params) {
      let folderPath = params ? params.fullPath : this.treeData[0].fullPathName
      folderPath = folderPath.substring(0, folderPath.lastIndexOf('/'))
      this.$store.dispatch('audioPool/showInExplorer', folderPath)
    },
    initAddFiles (params) {
      const { dialog } = require('electron').remote
      const folderPath = params ? params.fullPath : this.treeData.children[0].fullPath
      const _this = this
      dialog.showOpenDialog({
        properties: ['openFile', 'multiSelections'],
        filters: [
          { name: 'Audio Files', extensions: ['wav', 'mp3', 'ogg', 'flac'] }]
      }).then((response) => {
        if (!response.canceled && folderPath) {
          _this.$store.dispatch('audioPool/addFiles', { destFolder: folderPath, filePaths: response.filePaths })
          // _this.$store.dispatch('audioPool/addDirectories', { target: folderPath, source: response.filePaths[0] })
        } else if (!response.canceled && !folderPath) {
          _this.$store.dispatch('audioPool/addFiles', { destFolder: this.folderPathProject + '/Sound Files', filePaths: response.filePaths })
        } else {
          console.log('no file selected')
        }
      })
    },
    initCreateSubfolder (params) {
      this.audio = params
      if (!params) {
        this.audio = this.treeData.children[0]
      }
      this.showCreateSubfolderModal = true
      this.$store.commit('main/setIsModal', true)
    },
    onCreateSubfolder (params) {
      var reg = /[^a-z0-9]/gi
      params = params.replace(reg, '')
      const path = this.$store.state.project.project.folderProject
      if (!this.audio.fullPath) {
        this.$store.dispatch('audioPool/createDirectory', path + '/Sound Files' + '/' + params)
        this.showCreateSubfolderModal = false
        this.$store.commit('main/setIsModal', false)
        this.audio = {}
      } else {
        const destFolder = this.audio.isLeaf ? this.audio.substring(0, this.audio.lastIndexOf('/')) : this.audio.fullPath
        const path = destFolder + '/' + params
        this.$store.dispatch('audioPool/createDirectory', path)
        this.showCreateSubfolderModal = false
        this.$store.commit('main/setIsModal', false)
        this.audio = {}
      }
    },
    onChangeName (newName) {
      if (this.audio.isLeaf) {
        this.$store.dispatch('audioPool/renameFile', { audio: this.audio, newName: newName })
        this.audio = {}
        this.$store.commit('main/setIsModal', false)
      }
    },
    async launchAudio (audio) {
      this.$store.commit('main/setTemporalElementDrag', { element: audio, type: 'audio' })
      this.$q.notify({
        type: 'positive',
        icon: 'launch',
        message: 'Click where you want to add the audio'
      })
      // await this.$store.dispatch('rete/loadAudioContainer', { audios: [audioResource] })
    },
    onAddNode (params) {
      // eslint-disable-next-line no-console
    },

    onClick (params) {
      // eslint-disable-next-line no-console
      // console.log('click', params)
    },

    drop: function ({ node, src, target }) {
      // eslint-disable-next-line no-console
      let targetPath = target.fullPath
      if (target.isLeaf) {
        targetPath = targetPath.substring(0, targetPath.lastIndexOf('/'))
      }
      this.activeItems.forEach(async item => {
        if (item.isLeaf) {
          if (item.fullPath !== (targetPath + '/' + item.name)) {
            await this.$store.dispatch('audioPool/moveFile', { source: item.fullPath, output: (targetPath + '/' + item.name) })
          }
        } else {
          if (item.fullPath !== (targetPath + '/' + item.name)) {
            await this.$store.dispatch('audioPool/moveDirectory', { source: item.fullPath, output: targetPath })
          }
        }
        // this.activeItems = []
      })
    },

    dropBefore: function ({ node, src, target }) {
      // eslint-disable-next-line no-console
    },

    dropAfter: function ({ node, src, target }) {
      // eslint-disable-next-line no-console
    },
    addNode () {
      var node = new TreeNode({ name: 'New Node', isLeaf: false })
      if (!this.data.children) this.data.children = []
      this.data.addChildren(node)
    },
    startDrag: function (event, audio) {
      event.dataTransfer.dropEffect = 'move'
      event.dataTransfer.effectAllowed = 'move'
      event.dataTransfer.setDragImage(this.dragImg, 20, 20)
      if (this.ctrlKey) {
        if (this.activeItems.findIndex(item => item.id === audio.id) === -1) {
          this.activeItems.push(audio)
        }
      } else {
        this.activeItems = []
        this.activeItems.push(audio)
      }
      event.dataTransfer.setData('audio', this.activeItems)
    },
    closeModal () {
      this.showDeleteElementModal = false
      this.message = []
    },
    closeRenameModal () {
      this.$store.commit('main/setIsModal', false)
    },
    closeCreateFolderModal () {
      this.showCreateSubfolderModal = false
      this.$store.commit('main/setIsModal', false)
    },
    filterTree () {
      const result = this.filter(this.treeOriginal.children, this.filterText)
      this.treeData.children = result
    },
    filter: function (array, text) {
      const tex = text.toLowerCase()
      const getChildren = (result, object) => {
        if (object.name.toLowerCase().indexOf(tex) >= 0) {
          result.push(object)
          return result
        }
        if (Array.isArray(object.children)) {
          const children = object.children.reduce(getChildren, [])
          if (children.length) result.push({ ...object, children })
        }
        return result
      }

      return array.reduce(getChildren, [])
    },
    async refreshPoolTree () {
      if (this.isOpenedProject) {
        this.$q.loading.show()
        console.log('cambio')
        await this.$store.dispatch('audioPool/init')
        this.$q.loading.hide()
      }
    }
  }
}
</script>
<style  lang="stylus" scoped>
.q-btn {
  ::v-deep {
    .q-btn__wrapper {
      padding: 4px 5px !important;
    }

  }
}
.q-item {
  min-height: 25px !important;
  padding: unset !important;
}
.q-item__section--avatar {
    min-width: unset !important;
}
</style>
