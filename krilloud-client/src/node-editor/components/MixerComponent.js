import Rete from 'rete'
import * as Socket from '../sockets.js'
import CustomMixerComponent from './MixerComponent.vue'

export class MixerComponent extends Rete.Component {
  constructor () {
    super('Mixer')
    this.data.render = 'vue'
    this.data.component = CustomMixerComponent
    this.data.props = {}
    this.data.path = 1
  }

  builder (node) {
    var inp1 = new Rete.Input('1', 'variable', Socket.variable, false)
    var inp2 = new Rete.Input('2', 'audio', Socket.audio, false)
    var out = new Rete.Output('0', 'Output', Socket.audio, false)
    var inp3 = new Rete.Input('3', 'variable', Socket.variable, false)
    var inp4 = new Rete.Input('4', 'audio', Socket.audio, false)

    if (!node.data.nextSocketId) {
      node.data.nextSocketId = 5
    }
    if (!node.data.volumes) {
      node.data.volumes = [1, 1]
    }

    const result = node
      .addInput(inp1)
      .addInput(inp2)
      .addInput(inp3)
      .addInput(inp4)
      .addOutput(out)

    if (node.data.nextSocketId > 4) {
      for (var x = 5; x < node.data.nextSocketId; x++) {
        if (x % 2 === 0) {
          result.addInput(new Rete.Input(String(x), 'audio', Socket.audio, false))
        } else {
          result.addInput(new Rete.Input(String(x), 'variable', Socket.variable, false))
        }
      }
    }
    return result
  }

  worker (node, inputs, outputs) {

  }
}
