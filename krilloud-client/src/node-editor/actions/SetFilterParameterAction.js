import Action from '../plugins/custom-history-plugin/action'
import Vue from 'vue'

export default class SetFilterParameterAction extends Action {
  constructor (prev, next, set) {
    super()
    this.prev = prev
    this.next = next
  }

  undo () {
    Vue.prototype.$store.dispatch('main/setFilterParameter', this.prev)
    this.draw(this.prev)
  }

  redo () {
    Vue.prototype.$store.dispatch('main/setFilterParameter', this.next)
    this.draw(this.next)
  }

  draw (data) {
    const node = Vue.prototype.$store.state.rete.editor.nodes
    var nodeFound = node.find(item => item.data.newId === data.idNode || (!item.data.newId && item.id === data.idNode))
    nodeFound.data[data.name].value = data.value
  }
}

// inside a "change" method of your Control (called by user action)
// this.value - value before changing
// next - new value
// (v) => this.set(v) - change value of Field by undo/redo
// this.emitter.trigger('addhistory', new FieldChangeAction(this.value, next, (v) => this.set(v)))
