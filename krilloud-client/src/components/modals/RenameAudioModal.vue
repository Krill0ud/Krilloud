<template>
      <q-dialog :value="showRenameAudioModal" @input="$emit('update', $event)" @show="audioName=''">
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showRenameAudioModal">
          <q-form @submit="onChangeName">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header">RENAME AUDIO FILE</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3"  @click="close()" flat dense v-close-popup/>
          </q-card-section>
          <q-card-section align="center" class="text-modal q-pt-lg">
            <q-input square class="col-6" bg-color="white" v-model="audioName" dense autofocus />
          </q-card-section>
          <q-card-section align="center" class="text-modal q-pa-sm" style="color:red">
            <div style="line-height: 2rem">
              <p>You are about to permanently rename <br>
              <span class="text-black" style="overflow-wrap: anywhere">{{audio.fullPath}}</span> <br> from your computer?</p>
            </div>
          </q-card-section>
          <q-card-actions align="center" class="q-pa-md">
            <q-btn v-close-popup  unelevated rounded  label="RENAME" class="my-button-sidebar" type="submit" />
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
  props: { showRenameAudioModal: Boolean, audio: Object },
  data () {
    return {
      audioName: ''
    }
  },
  mounted () {
    this.audioName = ''
  },
  updated () {
  },
  computed: {

  },
  methods: {
    onChangeName () {
      this.$emit('onChangeName', this.audioName)
    },
    cancel () {
      this.$emit('cancelModal')
    },
    close () {
      this.$emit('close')
    }

  }

}
</script>
