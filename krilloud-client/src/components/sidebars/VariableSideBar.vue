<template>
  <q-card class="tag-section non-selectable" square>
    <q-card-section class="sidebar row items-baseline justify-around q-pr-sm my-sticky">
      <div class="sidebar-title col-4">
        <div align="left">VARIABLES</div>
      </div>
      <div class="text-title col-8" align="right">
        <!-- <q-btn flat round icon="icon-folder-closed" size="md" class="q-tree-btn"/> -->
        <q-btn flat round icon="icon-icon-add-small" size="md" class="q-tree-btn" @click="newVariable" />
      </div>
      </q-card-section>
      <br/>
      <br/>
      <q-card-section v-if="variables.length === 0" class="text-center text-tag tag-section">
        <div class="q-pt-lg">
          <p>You have no variables</p>
          <q-btn @click="newVariable" unelevated rounded  label="CREATE VARIABLE" class="my-button-sidebar" />
          </div>
      </q-card-section>
      <q-card-section class="tex-tag inputSearch" style="padding: 0px" v-else >
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
            <q-icon name="search"/>
          </template>
        </q-input>
        <q-tree
          :nodes="variables"
          node-key="name"
          text-color="primary"
          :filter="filter"
          :filter-method="myFilterMethod"
          default-expand-all
          class="text-tag q-item-drag header-expansion"
        >
          <template dense v-slot:header-generic="prop" draggable="true"  @dragstart.native="startDrag($event, prop.node)">
            <q-item class="full-width" dense  draggable="true"  @dragstart.native="startDrag($event, prop.node)" active-class="bg-grey-13">
                <q-item-section class="">{{prop.node.name}}</q-item-section>
                <q-item-section class="q-pr-xs items-end">
                  <div class="row no-wrap">
                    <q-btn padding="xs" color="white" class="q-tree-btn" size="5px" icon="icon-icon-edit" flat @click="editVariable(prop.node)"></q-btn>
                    <q-btn padding="xs" color="white" class="q-tree-btn" size="5px" icon="icon-icon--delete" flat @click="confirmDeleteVar(prop.node)"></q-btn>
                  </div>
                </q-item-section>
            </q-item>
          </template>
        </q-tree>
     </q-card-section>
     <q-dialog id="var-editor" v-model="showVariableEditor" persistent>
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showVariableEditor">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header">{{ this.editModeEditor ? 'Edit Variable': 'New Variable'}}</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3" @click="closeModal()" flat  dense v-close-popup/>
          </q-card-section>

          <q-card-section class="q-pa-lg text-modal">
            <q-form @submit="onSubmit">
            <div>Variable Name</div>
            <q-input ref="variableName" v-model="currentVariable.name" borderless color="white" bg-color="white" dense autofocus
            :rules='[validate]' />
            <div class="q-pt-lg" >Simulated value</div>
            <q-input
            v-model="currentVariable.simulated"
            type="number"
            step="any"
            @blur="currentVariable.simulated = $helper.formatNumber(currentVariable.simulated, 3)"
            filled
            borderless
            style="max-width: 400px; background: white" color="white" bg-color="white" dense />
            <div class="q-pt-md">
                <q-select dense square filled bg-color="white" v-model="currentVariable.varType" :options="$store.state.main.varTypes" emit-value
        map-options></q-select>
                <q-select dense square filled bg-color="white" class="text-tag q-pt-md" v-model="currentVariable.varContentType" :options="$store.state.main.varContentTypes" emit-value map-options ></q-select>
                  <div v-if="!currentVariable.varContentType" class="row q-pt-md items-baseline justify-around">
                    <div>From</div>
                  <q-input
                    v-model="currentVariable.minRange"
                    type="number"
                    filled
                    step="any"
                    @blur="currentVariable.minRange = $helper.formatNumber(currentVariable.minRange, 3)"
                    style="max-width: 90px; background: white" color="white" bg-color="white" dense />
                    <div>To</div>
                    <q-input
                    v-model="currentVariable.maxRange"
                    type="number"
                    filled
                    step="any"
                    @blur="currentVariable.maxRange = $helper.formatNumber(currentVariable.maxRange, 3)"
                    style="max-width: 90px; background: white" color="white" bg-color="white" dense />
                    </div>
                <q-select dense square filled bg-color="white" class="q-pt-md text-tag" borderless v-model="currentVariable.varSourceType" :options="varSourceTypes" emit-value
                  map-options></q-select>
              </div>
              <ErrorValidate :message="message"/>
              <div align="center" class="q-pt-md">
                <q-btn v-if="!this.editModeEditor" type="submit"  unelevated rounded  label="CREATE VARIABLE" class="my-button-sidebar"/>
                <q-btn v-else type="submit" unelevated rounded  label="EDIT VARIABLE" class="my-button-sidebar" />
              </div>
              </q-form>
          </q-card-section>
        </q-card>
      </q-dialog>
      <DeleteElementModal :showDeleteElementModal="showDeleteElementModal" @update="showDeleteElementModal = $event" :hideMessage="hideMessage" :audioDelete="false" :variableDelete="true" :variableName="variableName" :message="message" @delete="deleteVariable()" @cancel="closeModal()" @close="closeModal()"/>
    </q-card>
</template>

<script>
import ErrorValidate from '../commons/ErrorValidate.vue'
import DeleteElementModal from '../modals/DeleteElementModal'

export default {
  name: 'VariableSideBar',
  props: {
  },
  components: {
    ErrorValidate,
    DeleteElementModal
  },
  data: function () {
    return {
      showVariableEditor: false,
      showDeleteElementModal: false,
      variableSelected: false,
      editModeEditor: false,
      dragImg: null,
      filter: '',
      message: [

      ],
      hideMessage: false,
      variableName: '',
      active: true
      // variablesTree: this.$store.state.variable.variables
    }
  },
  computed:
  {
    variables () {
      return this.$store.state.variable.variables
    },
    currentVariable () {
      return this.$store.state.variable.currentVariable
    },
    varTypes () {
      return this.$store.state.main.varTypes
    },
    varSourceTypes () {
      return this.$store.state.main.varSourceTypes
    },
    varContentTypes () {
      return this.$store.state.main.varContentTypes
    }
  },
  watch: {
    currentVariable () {
      this.variableSelected = this.currentVariable
    }
  },
  mounted () {
    var img = new Image()
    img.src = require('../../assets/img/variable-connector@2x.png')
    this.dragImg = img
  },
  methods: {
    setModeEditing () {
      this.$store.commit('main/setModeEditing', true)
    },
    setNoEditing () {
      this.$store.commit('main/setModeEditing', false)
    },
    newVariable () {
      const newVariableInstance = this.getDefaultVariable()
      this.$store.commit('variable/setCurrentVariable', newVariableInstance)
      this.editModeEditor = false
      this.showVariableEditor = true
      this.$store.commit('main/setIsModal', true)
    },
    saveVariable () {
      this.valMinMax()
      if (this.message.length === 0) {
        this.$store.dispatch('variable/saveVariable', this.$store.state.variable.currentVariable)
        this.showVariableEditor = false
        this.$store.commit('main/setIsModal', false)
      } else {
        return this.message
      }
    },
    cancelVariable () {
      this.$store.commit('variable/setCurrentVariable', {})
      this.showTagEditor = false
      this.$store.commit('main/setIsModal', false)
    },
    editVariable (variable) {
      this.$store.commit('variable/setCurrentVariable', variable)
      this.editModeEditor = true
      this.showVariableEditor = true
      this.$store.commit('main/setIsModal', true)
    },
    saveEditVariable () {
      this.valMinMax()
      if (this.message.length === 0) {
        const index = this.variables.findIndex(item => item.id === this.currentVariable.id)
        this.currentVariable.index = index
        this.$store.dispatch('variable/editVariable', this.currentVariable)
        this.showVariableEditor = false
        this.$store.commit('main/setIsModal', false)
      } else {
        return this.message
      }
    },
    async deleteVariable () {
      const nodes = this.$store.state.rete.editor.nodes
      var nodesFound = await nodes.filter(item => item.name === 'Variable' && item.data.name === this.currentVariable.name)
      nodesFound.forEach(async variable => {
        await this.$store.state.rete.editor.removeNode(variable)
      })
      this.$store.dispatch('variable/deleteVariable', this.currentVariable)
      this.showDeleteElementModal = false
    },
    getDefaultVariable () {
      return this.$store.getters['variable/initialData']
    },
    startDrag: async function (event, variable) {
      // event.dataTransfer.effectAllowed = 'move'
      // event.dataTransfer.dropEffect = 'move'

      // event.dataTransfer.setDragImage(this.dragImg, 20, 20)
      event.dataTransfer.setData('variable', variable.name)
    },
    launchVar (variable) {
      this.$store.commit('main/setTemporalElementDrag', { element: variable, type: 'variable' })
      this.$q.notify({
        type: 'positive',
        icon: 'launch',
        message: 'Click where you want to add the variable'
      })
      /* const index = this.$store.state.variable.variables.findIndex(item => item.name === variable.name)
      this.$store.dispatch('rete/loadVariable', { index: index, name: variable.name }) */
    },
    confirmDeleteVar (variable) {
      this.showDeleteElementModal = true
      this.variableName = variable.name
      this.$store.commit('variable/setCurrentVariable', variable)
      // this.variableName = variable.name
    },
    onSubmit () {
      if (!this.editModeEditor) {
        this.saveVariable()
        // this.showVariableEditor = false
      } else if (this.editModeEditor) {
        this.saveEditVariable()
        // this.showVariableEditor = false
      }
    },
    validate (value) {
      if (!value) {
        return 'Required field'
      } else {
        var variableName = this.$store.state.variable.variables.some(item => item.name.toUpperCase() === value.toUpperCase() && item.id !== this.currentVariable.id)
        return !variableName || 'that name is already being used.'
      }
    },
    valMinMax () {
      this.message = []
      if (parseInt(this.currentVariable.minRange) > parseInt(this.currentVariable.maxRange)) {
        var valMinError = 'Min value should be less than Max value'
        this.message.push(valMinError)
      }
      if (parseInt(this.currentVariable.simulated) > parseInt(this.currentVariable.maxRange)) {
        var simulatedError = `Simulated value should be between ${this.currentVariable.minRange} and ${this.currentVariable.maxRange}`
        this.message.push(simulatedError)
      }
      return this.message
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
      this.$store.commit('main/setIsModal', false)
      this.message = []
    }
  }
}
</script>
