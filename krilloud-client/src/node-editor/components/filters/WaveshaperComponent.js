import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomWaveshaperComponent from './WaveshaperComponent.vue'

export class WaveshaperComponent extends Rete.Component {
  constructor () {
    super('WaveShaper')
    this.data.render = 'vue'
    this.data.component = CustomWaveshaperComponent
    this.data.props = {}
    this.data.path = 3
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)
    var inp2 = new Rete.Input('2', 'Value', Socket.variable, false)
    var inp3 = new Rete.Input('3', 'Value', Socket.variable, false)
    var out = new Rete.Output('1', 'Value', Socket.audio, false)

    if (Object.keys(node.data).length === 0) {
      node.data = {
        wet: { id: 0, value: 0.000, minValue: 0.000, maxValue: 1.000 },
        amount: { id: 1, value: 0.000, minValue: -1.000, maxValue: 1.000 }
      }
    }
    return node
      .addInput(inp1)
      .addInput(inp2)
      .addInput(inp3)
      .addOutput(out)
  }

  worker (node, inputs, outputs) {

  }
}
