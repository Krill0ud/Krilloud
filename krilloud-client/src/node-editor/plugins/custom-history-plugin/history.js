import Vue from 'vue'

export default class History {
  constructor () {
    this.active = false
    this.map = []
  }

  add (action) {
    if (this.active) return
    this._tagMap().produced.push(action)
    this._tagMap().reserved = []
  }

  _tagMap () {
    const tagName = Vue.prototype.$store.state.tag.currentTag.name
    if (!this.map[tagName]) {
      this.map[tagName] = { produced: [], reserved: [] }
    }
    return this.map[tagName]
  }

  get last () {
    return this._tagMap().produced[this._tagMap().produced.length - 1]
  }

  async _do (from, to, type) {
    const action = from.pop()

    if (!action) return

    this.active = true
    await action[type]()
    await Vue.prototype.$store.state.rete.editor.trigger('process')
    to.push(action)
    this.active = false
  }

  undo () {
    this._do(this._tagMap().produced, this._tagMap().reserved, 'undo')
  }

  clear () {
    this.active = false
    // this.produced = []
    // this.reserved = []
  }

  redo () {
    this._do(this._tagMap().reserved, this._tagMap().produced, 'redo')
  }
}
