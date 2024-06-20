import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomRobotizerComponent from './RobotizerComponent.vue'

export class RobotizerComponent extends Rete.Component {
  constructor () {
    super('Robotizer')
    this.data.render = 'vue'
    this.data.component = CustomRobotizerComponent
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
        wet: { id: 0, value: 1.000, minValue: 0.000, maxValue: 1.000 },
        frequency: { id: 1, value: 30.000, minValue: 0.100, maxValue: 100.000 },
        waveform: { id: 2, value: 0.000, minValue: 0.000, maxValue: 6.000 }
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
