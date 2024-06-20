<template>
      <q-dialog :value="showDeleteElementModal" @input="$emit('update', $event)" persistent >
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showDeleteElementModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header q-pl-xl" v-if="tagDelete">DELETE TAG</div>
            <div class="col-11 text-h6 dialog-header q-pl-xl" v-else>{{ audioDelete ? 'DELETE AUDIO FILE': 'DELETE VARIABLE'}}</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3"  @click="close()" flat dense v-close-popup/>
          </q-card-section>

          <q-card-section align="center" class="text-modal q-pa-md" v-if="audioDelete">
            <div style="line-height: 2rem">
              <p>You are about to permanently delete <br>
              <span class="text-black elementName" style="font-weight: bolder">{{audioName}}</span> <br> from your computer</p>
            </div>
            </q-card-section>
            <q-card-section align="center" class="text-modal q-pa-md" v-if="folderDelete">
            <div style="line-height: 2rem">
              <p>You are about to permanently delete <br>
              <span class="text-black elementName" style="font-weight: bolder">{{subfolder}}</span> <br> from your computer?</p>
            </div>
            </q-card-section>
            <q-card-section align="center" class="text-modal q-pa-md" v-if="variableDelete">
            <div style="line-height: 2rem">
              <p>You are about to permanently delete <br>
              <span class="text-black elementName" style="font-weight: bolder">{{variableName}}</span> <br> from your Project?</p>
            </div>
          </q-card-section>
          <q-card-section align="center" class="text-modal q-pa-md" v-if="tagDelete">
            <div style="line-height: 2rem">
              <p>Delete <br>
              <span class="text-black elementName" style="font-weight: bolder">{{tagName}}</span> <br> are you sure?</p>
            </div>
            </q-card-section>
          <q-card-actions align="center" class="q-pa-xs">
            <q-btn   unelevated rounded  label="DELETE" class="my-button-sidebar" @click="deleteElement()" />
            <q-btn v-close-popup  unelevated rounded  label="CANCEL" class="my-button-sidebar" @click="cancel()" />
          </q-card-actions>
          <q-card-actions align="center" class="q-pa-xs">
          <ErrorValidate :message="message" style="width:400px" />
          </q-card-actions>
        </q-card>

      </q-dialog>
</template>

<script>

import {

} from 'vuex'
import ErrorValidate from '../commons/ErrorValidate.vue'
export default {
  props: {
    showDeleteElementModal: Boolean,
    hideMessage: Boolean,
    message: Array,
    audioName: String,
    audioDelete: Boolean,
    variableDelete: Boolean,
    variableName: String,
    tagDelete: Boolean,
    tagName: String,
    subfolder: String,
    folderDelete: Boolean
  },
  components: {
    ErrorValidate
  },
  data () {
    return {

    }
  },
  computed: {

  },
  methods: {
    deleteElement () {
      this.$emit('delete')
    },
    cancel () {
      this.$emit('cancel')
    },
    close () {
      this.$emit('close')
    }

  }

}
</script>
