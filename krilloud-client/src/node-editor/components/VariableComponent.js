import Rete from 'rete'
import * as Socket from '../sockets.js'
import CustomVariableComponent from './VariableComponent.vue'

export class VariableComponent extends Rete.Component {
  constructor () {
    super('Variable')
    this.data.render = 'vue'
    this.data.component = CustomVariableComponent
    this.data.props = {}
    this.data.path = 2
  }

  builder (node) {
    var out = new Rete.Output('0', 'Value', Socket.variable, false)

    return node
      .addOutput(out)
  }

  worker (node, inputs, outputs) {

  }
}
