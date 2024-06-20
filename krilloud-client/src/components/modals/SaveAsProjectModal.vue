<template>
  <q-dialog :value="showSaveAsProjectModal" @input="$emit('update', $event)" persistent>
      <q-card class="bg-modal" style="width:450px;height:auto">
          <q-card-section class="row items-center dialog-header">
            <div class="col-1 text-h6 dialog-header">
              <q-icon dense flat>
                <img src="~assets/icons/Krilloud_icon.png" />
              </q-icon>
            </div>
            <div class="col-10 text-h6 dialog-header">Save project</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3" flat  dense v-close-popup/>
          </q-card-section>
          <q-form @submit="saveCurrentProjectAs">
              <q-card-section class="text-modal">
                <div class="q-pb-md">Save as: </div>
                <q-input v-model="projectName" borderless color="white" bg-color="white" dense autofocus class="q-pa-xs" :rules='[validate]'/>
                </q-card-section>
              <q-card-actions class="q-pb-md" align="center">
                <q-btn unelevated rounded  class="my-button-sidebar" label="ACCEPT" type="submit"/>
                <q-btn unelevated rounded  class="my-button-sidebar" label="CANCEL" @click="cancel()" v-close-popup />
              </q-card-actions>
            </q-form>
      </q-card>
    </q-dialog>
</template>

<script>

export default {
  props: { showSaveAsProjectModal: Boolean },
  data () {
    return {
      alsoSave: false,
      projectName: ''
    }
  },
  computed: {
    currentProject () {
      return this.$store.state.project.project
    }
  },
  methods: {
    saveCurrentProjectAs () {
      this.$store.dispatch('project/saveAs', this.projectName)
      this.projectName = ''
      this.$store.state.project.showSaveAsProjectModal = false
    },
    cancel () {
      this.$emit('cancel')
    },
    validate (value) {
      if (!value) {
        return 'Required field'
      }
    }
  }
}
</script>
