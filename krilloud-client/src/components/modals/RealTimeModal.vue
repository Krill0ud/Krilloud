<template>
  <q-dialog
    :value="showRealTimeModal"
    @input="$emit('update', $event)"
    persistent
  >
    <q-card
      class="bg-modal non-selectable"
      style="width:398px;height:auto"
      v-if="showRealTimeModal"
    >
      <q-card-section class="row items-center dialog-header q-pa-xs">
        <div class="col-11 text-h6 dialog-header">REAL TIME</div>
        <q-btn
          class="col-1"
          icon="close"
          color="blue-grey-3"
          v-close-popup
          flat
          dense
          @click="close()"
        />
      </q-card-section>

      <q-card-section class="q-pl-lg q-pr-lg q-pt-lg q-pb-sm text-modal">
        <div class="row">
          <span class="col-6 q-pb-md" style="font-weight: bold"
            >Conecction</span
          >
          <span
            class="col-6 q-pb-md"
            style="text-align: end; color: #f0134e; font-weight: bold"
            v-if="!realTimeState"
            >No active connection</span
          >
          <span
            class="col-6 q-pb-md"
            style="text-align: end; color: #80c088; font-weight: bold"
            v-if="realTimeState"
            >Connected</span
          >
        </div>
        <div class="q-pb-xs">Connection port:</div>
        <q-input
          v-model="port"
          borderless
          color="white"
          bg-color="white"
          dense
          autofocus
        />
        <div class="q-pt-md text-red">{{ message }}</div>
      </q-card-section>
      <q-card-actions align="center" class="q-pb-lg">
        <q-btn
          v-if="!realTimeState"
          unelevated
          rounded
          label="CONNECT"
          class="my-button-sidebar"
          style="font-weight: bold"
          @click="connect()"
        />
        <q-btn
          v-if="realTimeState"
          unelevated
          rounded
          label="DISCONNECT"
          class="my-button-sidebar"
          style="font-weight: bold"
          @click="disconnect()"
        />
      </q-card-actions>
      <q-separator color="white" size="0.15rem" />
      <q-card-section class="q-pa-lg text-modal">
        <div>
          <span class="col-6 q-mb-md" style="font-weight: bold"
            >In game-objects</span
          >
          <q-list class="q-pt-sm scroll2" style="max-height: 100px">
            <q-item
              dense
              v-for="item in realtimeObjectList"
              :key="item.key"
              class="q-ma-xs"
              >{{ item.id }}</q-item
            >
          </q-list>
        </div>
      </q-card-section>
      <q-separator color="white" size="0.15rem" />
      <q-card-section class="q-pa-lg text-modal">
        <div>
          <span class="col-6 q-pb-md" style="font-weight: bold"
            >Playing tags</span
          >
          <q-list class="q-pt-sm scroll2" style="max-height: 100px">
            <q-item
              dense
              v-for="item in realtimeTagsList"
              :key="item.key"
              class="q-ma-xs"
              >{{ item.tagname }}</q-item
            >
          </q-list>
        </div>
      </q-card-section>
    </q-card>
  </q-dialog>
</template>

<script>
import {} from 'vuex'
export default {
  props: { showRealTimeModal: Boolean },
  data () {
    return {
      port: 25,
      connected: false,
      message: '',
      objects: [],
      tagsPlaying: []
    }
  },
  watch: {},
  computed: {
    realTimeState () {
      return this.$store.state.main.realTimeState
    },
    realtimeObjectList () {
      return this.$store.state.main.realtimeObjectList
    },
    realtimeTagsList () {
      return this.$store.state.main.realtimeTagsList
    }
  },
  methods: {
    close () {
      this.$store.commit('main/setIsModal', false)
      this.message = ''
    },
    async connect () {
      await this.$store.dispatch('main/connect', {
        port: this.port.toString()
      })
      const activeConnection = await this.stateConnection()
      if (activeConnection) {
        this.connected = true
        this.message = ''
        this.objects = []
        this.$store.commit('main/setRealTimeState', true)
        const objectList = await this.$store.dispatch(
          'main/getIngameObjectList'
        )
        this.$store.commit('main/setRealtimeObjectList', objectList)
        this.objects = this.realtimeObjectList

        const playingTagList = await this.$store.dispatch(
          'main/getPlayingTagsList'
        )
        this.$store.commit('main/setRealtimeTagsList', playingTagList)
        this.tagsPlaying = this.realtimeTagsList
      } else {
        this.connected = false
        this.message = 'First start server in unity, with the same port'
        this.$store.commit('main/setRealTimeState', false)
      }
      this.$store.commit('main/setIsModal', false)
    },
    async disconnect () {
      await this.$store.dispatch('main/disconnect')
      this.$store.commit('main/setRealTimeState', false)
      this.$store.commit('main/setRealtimeObjectList', [])
      this.objects = []
      this.$store.commit('main/setRealtimeTagsList', [])
      this.tagsPlaying = []

      this.stateConnection()
      this.connected = false
      this.$store.commit('main/setIsModal', false)
    },
    async stateConnection () {
      const connectionActive = await this.$store.dispatch(
        'main/isConectionActive'
      )
      return connectionActive
    },
    async connectionStablished () {
      const connectionStablished = await this.$store.dispatch(
        'main/connectionStablished'
      )
      return connectionStablished
    },
    async isServerActive () {
      const serverActive = await this.$store.dispatch('main/isActiveServer')
      return serverActive
    }
  }
}
</script>
