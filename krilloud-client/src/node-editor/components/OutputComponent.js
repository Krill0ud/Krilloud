import Rete from 'rete'
import * as Socket from '../sockets.js'
import CustomOutputComponent from './OutputComponent.vue'

export class OutputComponent extends Rete.Component {
  constructor () {
    super('Output')
    this.data.render = 'vue'
    this.data.component = CustomOutputComponent
    this.data.path = 4
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)

    return node
      .addInput(inp1)
  }

  worker (node, inputs, outputs) {

  }
}
