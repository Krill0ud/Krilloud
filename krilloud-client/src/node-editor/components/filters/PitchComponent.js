import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomPitchComponent from './PitchComponent.vue'

export class PitchComponent extends Rete.Component {
  constructor () {
    super('Pitch')
    this.data.render = 'vue'
    this.data.component = CustomPitchComponent
    this.data.props = {}
    this.data.path = 3
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)
    var inp2 = new Rete.Input('2', 'Value', Socket.variable, false)
    var out = new Rete.Output('1', 'Value', Socket.audio, false)

    if (Object.keys(node.data).length === 0) {
      node.data = {
        tune: { id: 0, value: 0.000, minValue: -24, maxValue: 24 }
      }
    }

    return node
      .addInput(inp1)
      .addInput(inp2)
      .addOutput(out)
  }

  worker (node, inputs, outputs) {

  }
}
