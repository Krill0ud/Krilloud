<template>
  <q-dialog :value="showExportProjectModal" @input="$emit('update', $event)" persistent>
      <q-card class="scroll bg-modal" style="width:auto;height:auto">
          <q-card-section class="row items-center dialog-header">
            <div class="col-1 text-h6 dialog-header">
              <q-icon dense flat>
                <img src="~assets/icons/Krilloud_icon.png" />
              </q-icon>
            </div>
            <div class="col-10 text-h6 dialog-header">Export project</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3" flat  dense v-close-popup/>
          </q-card-section>
          <q-card-section class="q-pa-lg text-modal">
            <div class="q-pl-sm">Export project to: </div>
            <div class="q-pt-sm q-pl-sm text-red"> {{currentProject.folderGame}}</div>
              <q-checkbox class="q-pt-lg" keep-color rigth-label size="md" color="grey-5" v-model="alsoSave"  label="Save project in the process"></q-checkbox>
          </q-card-section>
        <q-card-actions align="center">
          <q-btn unelevated rounded  class="my-button-sidebar" label="ACCEPT" @click="exportProject(true)" v-close-popup />
          <q-btn unelevated rounded  class="my-button-sidebar" label="CANCEL" @click="cancel(false)" v-close-popup />
        </q-card-actions>
      </q-card>
    </q-dialog>
</template>

<script>

export default {
  props: { showExportProjectModal: Boolean },
  data () {
    return {
      alsoSave: true
    }
  },
  computed: {
    currentProject () {
      return this.$store.state.project.project
    },
    project () {
      var project = Object.assign({}, this.currentProject)
      return project
    }
  },
  methods: {
    exportProject () {
      this.$store.dispatch('project/exportProject', this.alsoSave)
    },
    cancel () {
      this.$store.commit('project/showExportProjectModal', false)
    }
  }
}
</script>
