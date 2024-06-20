import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomFlangerComponent from './FlangerComponent.vue'

export class FlangerComponent extends Rete.Component {
  constructor () {
    super('Flanger')
    this.data.render = 'vue'
    this.data.component = CustomFlangerComponent
    this.data.props = {}
    this.data.path = 3
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)
    var inp2 = new Rete.Input('2', 'Value', Socket.variable, false)
    var inp3 = new Rete.Input('3', 'Value', Socket.variable, false)
    var inp4 = new Rete.Input('4', 'Value', Socket.variable, false)
    var out = new Rete.Output('1', 'Value', Socket.audio, false)

    if (Object.keys(node.data).length === 0) {
      node.data = {
        wet: { id: 0, value: 0.000, minValue: 0.000, maxValue: 1.000 },
        delay: { id: 1, value: 0.005, minValue: 0.001, maxValue: 0.100 },
        frequency: { id: 2, value: 10.000, minValue: 0.001, maxValue: 100.000 }
      }
    }
    return node
      .addInput(inp1)
      .addInput(inp2)
      .addInput(inp3)
      .addInput(inp4)
      .addOutput(out)
  }

  worker (node, inputs, outputs) {
  }
}
