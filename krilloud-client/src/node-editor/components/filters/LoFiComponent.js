import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomLoFiComponent from './LoFiComponent.vue'

export class LoFiComponent extends Rete.Component {
  constructor () {
    super('LoFi')
    this.data.render = 'vue'
    this.data.component = CustomLoFiComponent
    this.data.props = {}
    this.data.path = 3
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)
    var out = new Rete.Output('1', 'Value', Socket.audio, false)
    var inp2 = new Rete.Input('2', 'Value', Socket.variable, false)
    var inp3 = new Rete.Input('3', 'Value', Socket.variable, false)
    var inp4 = new Rete.Input('4', 'Value', Socket.variable, false)

    if (Object.keys(node.data).length === 0) {
      node.data = {
        wet: { id: 0, value: 0.000, minValue: 0.000, maxValue: 1.000 },
        sampleRate: { id: 1, value: 0.000, minValue: 100.000, maxValue: 22000.000 },
        bitDepth: { id: 2, value: 3.001, minValue: 0.500, maxValue: 16.000 }
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
