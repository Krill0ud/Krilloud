<template>
    <div :class="tagMode">
      <div class="row items-baseline justify-between">
    </div>
    <div class="row items-baseline q-pt-sm q-pl-xs">
      <div class="col-5 ">Channel:</div>
      <div class="col-7">
     <q-select borderless v-model="channel" :options="channelOptions" dense map-options emit-value  options-dense popup-content-class="menu-options" options-selected-class="text-grey-14" class="selectColor"></q-select>
    </div>
          <div class="col-4 q-pt-md 1-pb-xs">
          <q-checkbox dense keep-color right-label size="xs" color="grey-7" v-model="audio3D" label="3D audio"></q-checkbox>
      </div>
      <div class="col-8 q-pt-md">
        <q-select v-if="audio3D"  borderless v-model="attenuation" :options="attenuationOptions"  map-options emit-value dense stack-label options-dense popup-content-class="menu-options" options-selected-class="text-grey-14" class="selectColor"></q-select>
        <div v-if="audio3D && attenuation" class="row items-baseline justify start q-pl-xs">
          <div class="col-5 q-pt-xs">Min distance:</div>
          <div class="col-7 q-pt-xs">
           <q-input
            @click.stop
            filled
            type="number"
            step="any"
            v-model="minDistance"
            standout="bg-white text-white"
            input-class="text-left"
          />
            </div>
          <div class="col-5 q-pt-xs">Max distance</div>
          <div class="col-7 q-pt-xs">
           <q-input
            @click.stop
            filled
            type="number"
            step="any"
            v-model="maxDistance"
            standout="bg-white text-white"
          /></div>
          <div class="col-5 q-pt-xs">Rolloff factor</div>
          <div class="col-7 q-pt-xs">
            <q-input
            @click.stop
            filled
            type="number"
            step="any"
            v-model="rolloff"
            standout="bg-white text-white"
          /></div>
          <div class="col-12" align="rigth left">
            <q-checkbox align="rigth left" keep-color rigth-label size="xs" color="grey-5" v-model="isdoppler"  label="Use Doppler Effect"></q-checkbox>
          </div>
          <div v-if="isdoppler" class="col-6" align="left">Doppler factor
          </div>
            <div v-if="isdoppler" class="col-6">
            <q-input
            @click.stop
            filled
            type="number"
            step="any"
            v-model="doppler"
            standout="bg-white text-white"
          /></div>
          <div class="col-12">
          <q-checkbox  keep-color rigth-label size="xs" color="grey-5" v-model="delay" label="Enable distance delay"></q-checkbox>
          </div>
          </div>
    </div>
      <div class="col-12 q-pt-xs">
        <q-checkbox dense keep-color right-label size="xs" color="grey-7" v-model="loop" label="Loop"></q-checkbox>
      </div>
    </div>
      </div>
</template>

<script>

export default {
  name: 'Tag',
  props:
    [
      'tag',
      'createMode',
      'tagMode'],
  data: function () {
    return {
    }
  },
  computed: {
    attenuationOptions () {
      return this.$store.state.main.attenuationOptions
    },
    channelOptions () {
      return this.$store.state.channel.channels
    },
    audio3D: {
      get () {
        return this.tag.audio3D
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'audio3D', tagName: this.name })
        }
        this.tag.audio3D = v
      }
    },
    name: {
      get () {
        return this.tag.name
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'name', tagName: this.name })
        }
        this.tag.name = v
      }
    },
    attenuation: {
      get () {
        return this.tag.attenuation
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'attenuation', tagName: this.name })
        }
        this.tag.attenuation = v
      }
    },
    channel: {
      get () {
        return this.tag.channel
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'channel', tagName: this.name })
        }
        this.tag.channel = v
      }
    },
    minDistance: {
      get () {
        return this.tag.minDistance
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: { minDistance: v, maxDistance: this.maxDistance, tagName: this.name }, attribute: 'min-max-distance' })
        }
        this.tag.minDistance = v
      }
    },
    maxDistance: {
      get () {
        return this.tag.maxDistance
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: { minDistance: this.minDistance, maxDistance: v, tagName: this.name }, attribute: 'min-max-distance' })
        }
        this.tag.maxDistance = v
      }
    },
    rolloff: {
      get () {
        return this.tag.rolloff
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'rolloff', tagName: this.name })
        }
        this.tag.rolloff = v
      }
    },
    isdoppler: {
      get () {
        return this.tag.isdoppler
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'isdoppler', tagName: this.name })
        }
        this.tag.isdoppler = v
      }
    },
    doppler: {
      get () {
        return this.tag.doppler
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'doppler', tagName: this.name })
        }
        this.tag.doppler = v
      }
    },
    delay: {
      get () {
        return this.tag.delay
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'delay', tagName: this.name })
        }
        this.tag.delay = v
      }
    },
    loop: {
      get () {
        return this.tag.loop
      },
      set (v) {
        if (!this.createMode) {
          this.$store.dispatch('tag/setAttribute', { value: v, attribute: 'loop', tagName: this.name })
        }
        this.tag.loop = v
      }
    }
  }
}
</script>
