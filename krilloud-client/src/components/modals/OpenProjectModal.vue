<template>
      <q-dialog ref='open-project' :value="showOpenProjectModal" @input="$emit('update', $event)" persistent>
        <q-card class="bg-modal non-selectable" style="width:450px;height:auto" v-if="showOpenProjectModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header">
              <q-img src="~assets/img/Krilloud_LOGO.png" style="width:300px;height:auto"/>
            </div>
            <div >
              <q-btn class="col-1" icon="close" color="blue-grey-3" flat  dense v-close-popup v-if="showCloseIcon"/>
            </div>

          </q-card-section>
          <q-card-section class="q-pa-lg text-modal">
            <q-list separator>
              <q-item-section  separator class="q-pb-sm title-recentProject">RECENT PROJECTS</q-item-section>
              <q-separator/>
              <q-card-section class="q-pt-lg text-center" style="height: 250px" v-if="recentProjects.length < 1"></q-card-section>
              <div v-else>
              <q-item class="recentProject" v-for="index in 5" :key="index" clickable @click="openRecent(recentProjects[index-1])">
                <q-item-section>
                  {{recentProjects[index-1]?recentProjects[index-1].project.name:''}}
                </q-item-section>
                <q-item-section style="font-size: 11px" side v-if="(recentProjects[index-1] && recentProjects[index-1].date) === date">Today</q-item-section>
                <q-item-section style="font-size: 11px" side v-else-if="(recentProjects[index-1] && recentProjects[index-1].date) === yesterdayDate">Yesterday</q-item-section>
                <q-item-section style="font-size: 11px" side v-else>{{recentProjects[index-1]?recentProjects[index-1].date:''}}</q-item-section>
              </q-item>
              </div>
            </q-list>
          </q-card-section>

          <q-card-actions align="center" class="q-pb-lg">
            <q-btn v-close-popup  unelevated rounded  label="NEW PROJECT" class="my-button-sidebar" @click="newProjectModal()"/>
            <q-btn unelevated rounded  label="BROWSE" class="my-button-sidebar" @click="openProject" />
            <q-input ref="myProjectDirectory" style="display:none" v-model="projectFile"
              @input="projectFileSelected" type="file" label="Standard" accept=".kpj"></q-input>
          </q-card-actions>
          </q-card>
      </q-dialog>
</template>

<script>
export default {
  props: { project: Object, showOpenProjectModal: Boolean },
  data () {
    return {
      projectFile: null
    }
  },
  computed: {
    date () {
      const today = new Date()
      return today.toLocaleDateString()
    },
    yesterdayDate () {
      const today = new Date()
      const yesterday = new Date()
      yesterday.setDate(today.getDate() - 1)
      return yesterday.toLocaleDateString()
    },
    recentProjects () {
      return this.$store.state.project.recentProjects
    },
    showCloseIcon () {
      return (this.$store.state.project.project && this.$store.state.project.project.name)
    }
  },
  methods: {
    openProject () {
      this.$refs.myProjectDirectory.$el.click()
    },
    async openRecent (project) {
      if (project && project.project.name) {
        await this.$store.dispatch('project/getOpenedProject', project.project.path)
        this.$store.dispatch('main/relaunch', project.project.path)
      }
    },
    async projectFileSelected () {
      await this.$store.dispatch('project/getOpenedProject', this.projectFile[0].path)
      this.projectFile = null
      this.$store.dispatch('main/relaunch', null)
      // this.$refs['open-project'].hide()
    },
    newProjectModal () {
      this.$emit('click')
    }
  }

}
</script>
