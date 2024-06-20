import Action from '../plugins/custom-history-plugin/action'
import Vue from 'vue'

export default class SetMixerVolumeAction extends Action {
  constructor (prev, next, set) {
    super()
    this.prev = prev
    this.next = next
  }

  undo () {
    Vue.prototype.$store.dispatch('main/setMixerVolume', this.prev)
    this.draw(this.prev)
  }

  redo () {
    Vue.prototype.$store.dispatch('main/setMixerVolume', this.next)
    this.draw(this.next)
  }

  draw (data) {
    const node = Vue.prototype.$store.state.rete.editor.nodes
    var nodeFound = node.find(item => item.data.newId === data.idNode || (!item.data.newId && item.id === data.idNode))
    nodeFound.data.volumes[data.index] = data.value
    nodeFound.update()
  }
}
