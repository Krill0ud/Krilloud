<template>
      <q-dialog :value="showCreateSubfolderModal" @input="$emit('update', $event)" >
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showCreateSubfolderModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header">CREATE SUBFOLDER</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3"  @click="close()" flat dense v-close-popup/>
          </q-card-section>
            <q-form @submit="onCreateSubfolder">
              <q-card-section align="center" class="text-modal q-pa-md">
            <q-input v-model="folderName" class="col-6" autofocus type="text" :rules='[validate]'></q-input>
          </q-card-section>
          <q-card-section align="center" class="text-modal q-pa-md">
            <div style="line-height: 2rem">
              <p>You are about to create a subfolder of <br>
              <span class="text-black" style="overflow-wrap: anywhere; display: inline-flex">{{audio.isLeaf? audio.fullPath.substring(0, audio.fullPath.lastIndexOf('/')) : audio.fullPath}}</span></p>
            </div>
          </q-card-section>
          <q-card-actions align="center" class="q-pa-md">
            <q-btn unelevated rounded  label="CREATE" class="my-button-sidebar" type="submit" />
            <q-btn v-close-popup  unelevated rounded  label="CANCEL" class="my-button-sidebar" @click="cancel()" />
          </q-card-actions>
            </q-form>

        </q-card>
      </q-dialog>
</template>

<script>

import {

} from 'vuex'
export default {
  props: { showCreateSubfolderModal: Boolean, audio: Object },
  data () {
    return {
      folderName: ''
    }
  },
  computed: {

  },
  methods: {
    onCreateSubfolder () {
      this.$emit('onCreateSubfolder', this.folderName)
      this.folderName = ''
    },
    cancel () {
      this.$emit('cancel')
    },
    close () {
      this.$emit('close')
    },
    validate (value) {
      if (!value) {
        return 'Required field'
      }
    }
  }

}
</script>
