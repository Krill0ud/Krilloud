import Rete from 'rete'
import * as Socket from '../sockets.js'
import CustomAudioComponent from './AudioComponent.vue'

export class AudioComponent extends Rete.Component {
  constructor () {
    super('AudioContainer')
    this.data.render = 'vue'
    this.data.component = CustomAudioComponent
    this.data.props = {}
    this.data.path = 1
  }

  builder (node) {
    var out = new Rete.Output('0', 'Value', Socket.audio, false)

    if (!node.data.name) {
      node.data.name = 'AUDIO CONTAINER'
    }

    if (!node.data.audioList) {
      node.data.audioList = []
      node.data.mode = 0
      node.data.loop = false
    }

    return node
      .addOutput(out)
  }

  worker (node, inputs, outputs) {

  }
}
