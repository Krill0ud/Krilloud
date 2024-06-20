<template>
      <q-dialog :value="showChannelsModal" @input="$emit('update', $event)">
        <q-card class="bg-modal" style="width:450px;height:auto" v-if="showChannelsModal">
          <q-card-section class="row items-center dialog-header">
            <div class="col-11 text-h6 dialog-header non-selectable">Audio Channels</div>
            <q-btn class="col-1" icon="close" color="blue-grey-3" flat  dense v-close-popup/>
          </q-card-section>

          <q-card-actions class="q-pa-lg text-modal">
              <q-list style="width: 420px" class="channel">
                <q-item dense v-for="(channel,index) in channels" :key="channel.key" style="width: 380px">
                  <q-item-section class="q-mb-sm">
                    <q-item-label class="row justify-between no-wrap channel">
                      <div class="col-9">
                        <q-input v-model="channel.name" @blur="changeName(channel)" @click="shortCutPlayOff()" borderless dense :readonly="index === 0"
                          input-class="channel-label" />
                      </div>
                      <div class="col-2 channel">
                        <q-input :value="$helper.formatNumber(channel.volume, 3)" readonly borderless dense input-class="text-right channel-label"/>
                      </div>
                      <div class="col-1"></div>
                    </q-item-label>

                    <q-item-label class="row no-wrap items-baseline justify-around" style="margin-top: -3px;">
                      <div class="col-11 column">
                        <q-slider class="channel"
                          @input="changeVolume(channel)"
                          v-model="channel.volume"
                          :min="0.000"
                          :max="1.000"
                          :step="0.100"
                          label
                        />
                      </div>
                        <div class="col-1 column">
                          <q-btn padding="xs" color="white" class="q-tree-btn" size="10px" icon="icon-icon--delete" flat  @click="deleteChannel(channel)" v-if="index > 0"></q-btn>
                        </div>
                    </q-item-label>
                  </q-item-section>
                  </q-item>
              </q-list>
          </q-card-actions>
            <q-card-section align="center">
              <div><q-btn unelevated rounded  label="Add channel" class="my-button-sidebar" @click="addChannel()" /></div>
            </q-card-section>
            </q-card>
      </q-dialog>
</template>

<script>
import channel from 'src/store/channel'

export default {
  props: { project: Object, showChannelsModal: Boolean },
  data () {
    return {

    }
  },
  computed: {
    channelLabel () {
      return console.log(channel)
    },
    channels () {
      return this.$store.state.channel.channels
    },
    currentChannel () {
      return this.$store.state.channel.currentChannel
    }
  },
  methods: {
    async addChannel () {
      const newChannelInstance = this.getDefaultChannel()
      if (this.channels.findIndex(item => item.name === newChannelInstance.name) > -1) {
        newChannelInstance.name = (newChannelInstance.name) + '.1 '
        newChannelInstance.label = (newChannelInstance.label) + '.1 '
      }
      await this.$store.dispatch('channel/save', newChannelInstance)
    },
    deleteChannel (channel) {
      this.$store.dispatch('channel/deleteChannel', { index: channel.value, channelId: channel.id })
    },
    getDefaultChannel () {
      return this.$store.getters['channel/channelInitialData']
    },
    changeVolume (channel) {
      this.$store.dispatch('channel/setVolume', channel)
      this.$forceUpdate()
    },
    async changeName (channel) {
      if (this.channels.findIndex(item => item.name === channel.label) > -1) {
        this.$q.notify({
          type: 'negative',
          icon: 'launch',
          message: 'that name is already being used'
        })
      }
      await this.$store.dispatch('channel/changeChannelName', channel)
      this.$store.commit('channel/setCurrentChannel', channel)
      this.$store.commit('channel/setChannelLabel', channel.name)
      this.$store.commit('main/setIsModal', false)
    },
    shortCutPlayOff () {
      this.$store.commit('main/setIsModal', true)
    }
  }
}
</script>
