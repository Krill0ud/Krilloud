<template>
    <q-card class="tag-section non-selectable" square>
      <q-card-section class="sidebar row items-baseline justify-around q-pr-sm my-sticky">
        <div class="sidebar-title col-4">
        <div align="left">TAGS</div>
        </div>
        <div class="text-title col-8" align="right">
          <!-- <q-btn flat round icon="icon-folder-closed" class="q-tree-btn" size="md"/> -->
          <q-btn
            flat
            round
            icon="icon-icon-add-small"
            size="md"
            @click="newTag"
            class="q-tree-btn"
          />
        </div>
        </q-card-section>
        <br/>
        <br/>
        <q-card-section class="tex-tag inputSearch" style="padding: 0px">
          <q-input
            ref="filter"
            @focus="setModeEditing()"
            @blur="setNoEditing()"
            v-model="filter"
            color="white"
            bg-color="white"
            label-color= "primary"
            style="padding: 16px 16px 5px 16px"
            filled
            dense>
              <template v-slot:append>
                <q-icon v-if="filter !== ''" name="clear" class="cursor-pointer" @click="resetFilter" />
              </template>
              <template v-slot:append>
                <q-icon name="search"  />
              </template>
          </q-input>
          <q-tree
            :nodes="tags"
            node-key="name"
            color="primary"
            text-color="primary"
            :filter="filter"
            :filter-method="myFilterMethod"
            class="text-tag header-expansion q-mx-sm"
            @click.stop=""
          >
            <template dense class="text-tag full-width" v-slot:header-generic="prop">
                    <q-radio
                      v-model="tagSelected"
                      :val="prop.node.name"
                      color="primary"
                      @input="loadTag(prop.node.id)"
                      class="full-width"
                      >
                        <div class="q-pl-sm">{{prop.node.name}}</div>
                    </q-radio>
                    <div id="items-wrap-tree" class="q-pr-xs items-end">
                      <div class="row no-wrap">
                        <q-btn padding="xs" color="white" class="q-tree-btn" icon="icon-icon-clone" flat @click="cloneTag(prop.node)"></q-btn>
                        <q-btn padding="xs" color="white" class="q-tree-btn" icon="icon-icon-edit" flat @click="editTag(prop.node)"></q-btn>
                        <q-btn padding="xs" color="white" class="q-tree-btn" icon="icon-icon--delete" flat @click="deletagModal(prop.node)"></q-btn>
                      </div>
                    </div>
                    <!--
                    <q-input
                      :value="prop.node.name"
                      @change="renameTag(prop.node.name, $event)"
                      @click.stop=""
                      @keypress.stop=""
                      @focus="setModeEditing()"
                      @blur="setNoEditing()"
                      standout="bg-white text-white"
                      input-class="text-left"
                      borderless
                      dense
                      class="input-tag-name"

                    ><template v-slot:append>
                    <q-avatar>
                     <q-icon name="edit"  style="font-size: 12px;"/>
                    </q-avatar>
                  </template>
                </q-input>
                </template>-->
                <!--<q-card>
                  <q-card-section class="full-width q-pb-none q-pt-xs">

                    <div class="q-pl-sm">{{prop.node.name}}</div>
                    <div class="q-pr-xs items-end">
                      <div class="row no-wrap">
                        <q-btn padding="xs" color="white" class="q-tree-btn" size="5px" icon="icon-icon-edit" flat @click="editTag(prop.node)"></q-btn>
                        <q-btn padding="xs" color="white" class="q-tree-btn" size="5px" icon="icon-icon--delete" flat @click="deletagModal(prop.node)"></q-btn>
                      </div>
                    </div>

                     <Tag :tag="prop.node" :tagMode="tagModeSideBar"/>
                    <div class="text-right" style="background-color:white">
                      <q-btn padding="xs" color="white" class="q-tree-btn" size="10px" icon="icon-icon--delete" flat @click="deletagModal(prop.node)"  ></q-btn>
                    </div>
                  </q-card-section>
                </q-card>-->

            </template>
          </q-tree>
        </q-card-section>
        <q-dialog v-model="showTagEditor" persistent>
          <q-card class="bg-modal" style="width:450px;height:auto" v-if="showTagEditor">
            <q-card-section class="row items-center dialog-header">
              <div class="col-11 text-h6 dialog-header">{{modalEditMode ? 'Edit tag' : 'New tag'}}</div>
              <q-btn class="col-1" icon="close" color="primary" flat  dense v-close-popup @click="cancelTag()" />
            </q-card-section>

            <q-card-section class="q-pa-lg text-modal">
              <q-form @submit="saveTag">
              <div class="q-pb-xs text-modal-subtitle">Tag Name</div>
              <q-input color="white" bg-color="white" v-model="temporalTag.name"  dense autofocus :rules='[validate]' @change="renameTag(temporalTag.name, $event)"/>
              <div class="q-pt-lg">
                <Tag :tag="temporalTag" :createMode="!modalEditMode" :tagMode="tagModeModal"/>
                </div>
                <div align="center" class="q-pt-sm">
                  <q-btn type="submit" unelevated rounded  :label="modalEditMode ? 'DONE' : 'CREATE TAG'" class="my-button-sidebar" />
                </div>
                </q-form>
            </q-card-section>
          </q-card>
        </q-dialog>
      <DeleteElementModal :showDeleteElementModal="showDeleteElementModal" @update="showDeleteElementModal = $event" :hideMessage="hideMessage" :variableDelete="false" :tagDelete="true" :tagName="tagName" :message="message" @delete="deleteTag()" @close="closeModal()"/>
    </q-card>
</template>

<script>
import Tag from './tag/Tag.vue'
import DeleteElementModal from '../modals/DeleteElementModal'

export default {
  name: 'TagSideBar',
  components: {
    Tag,
    DeleteElementModal
  },
  props: {},
  mounted () {
    if (this.currentTag) { // cuando se cierra el audioPoolViewer tiene que volver a seleccionar el tag que estaba
      this.tagSelected = this.currentTag.name
    }
  },
  data: function () {
    return {
      tagSelected: null,
      showTagEditor: false,
      modalEditMode: false,
      origTemporalTagName: '',
      temporalTag: null,
      tagModeModal: 'tagModeModal',
      tagModeSideBar: 'tagModeSideBar',
      filter: '',
      showDeleteElementModal: false,
      tagName: '',
      hideMessage: false,
      message: []
    }
  },
  computed: {
    tags () {
      return this.$store.state.tag.tags
    },
    currentTag () {
      return this.$store.state.tag.currentTag
    }
  },
  watch: {
    currentTag () {
      this.tagSelected = this.currentTag.name
    }
  },
  methods: {
    newTag () {
      this.modalEditMode = false
      this.temporalTag = this.getDefaultTag()
      // this.$store.commit('main/setCurrentTag', newTagInstance)
      this.showTagEditor = true
      this.$store.commit('main/setIsModal', true)
    },
    editTag (tag) {
      this.modalEditMode = true
      this.temporalTag = tag
      this.origTemporalTagName = tag.name
      this.showTagEditor = true
      this.$store.commit('main/setIsModal', true)
    },
    cloneTag (tag) {
      this.$store.dispatch('tag/clone', { tagName: tag.name })
    },
    saveTag (tag) {
      if (!this.modalEditMode) {
        this.$store.dispatch('tag/save', this.temporalTag)
      }
      this.showTagEditor = false
      this.$store.commit('main/setIsModal', false)

      // this.tagSelected = this.temporalTag.name
    },
    cancelTag () {
      this.temporalTag = null
      this.showTagEditor = false
      this.$store.commit('main/setIsModal', false)
    },
    deletagModal (tag) {
      this.showDeleteElementModal = true
      this.tagName = tag.name
    },
    deleteTag (tag) {
      this.message = []
      if (this.tags.length === 1) {
        const error = "you can't delete this tag"
        this.message.push(error)
      } else {
        this.$store.dispatch('tag/deleteTag', this.tagName)
        this.showDeleteElementModal = false
      }
    },
    validate (value) {
      if (!value) {
        return 'Required field'
      } else {
        var tagName = this.$store.state.tag.tags.some(item => item.name.toUpperCase() === value.toUpperCase() && (this.modalEditMode ? item.id !== this.temporalTag.id : true))
        return !tagName || 'that name is already being used.'
      }
    },
    loadTag (id) {
      this.$store.dispatch('rete/loadTag', id)
    },
    getDefaultTag () {
      return this.$store.getters['tag/initialData']
    },
    myFilterMethod (node, filter) {
      const filt = filter.toLowerCase()
      return node.name && node.name.toLowerCase().indexOf(filt) > -1
    },
    resetFilter () {
      this.filter = ''
      this.$refs.filter.focus()
    },
    closeModal () {
      this.showDeleteElementModal = false
      this.message = []
    },
    async renameTag () {
      if (this.modalEditMode) {
        const newName = this.temporalTag.name
        const oldName = this.origTemporalTagName
        this.origTemporalTagName = newName
        await this.$store.dispatch('tag/renameTag', { oldName: oldName, newName: newName })
      }
    },
    setModeEditing () {
      this.$store.commit('main/setModeEditing', true)
    },
    setNoEditing () {
      this.$store.commit('main/setModeEditing', false)
    }

  }
}
</script>
