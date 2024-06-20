import Rete from 'rete'
import * as Socket from '../sockets.js'
import CustomSelectorComponent from './SelectorComponent.vue'

export class SelectorComponent extends Rete.Component {
  constructor () {
    super('Selector')
    this.data.render = 'vue'
    this.data.component = CustomSelectorComponent
    this.data.props = {}
    this.data.path = 1
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Selector', Socket.variable, false)
    var inp2 = new Rete.Input('2', 'Audio', Socket.audio, false)
    var inp3 = new Rete.Input('3', 'Audio', Socket.audio, false)
    var out = new Rete.Output('1', 'Output', Socket.audio, false)

    if (!node.data.nextSocketId) {
      node.data.nextSocketId = 4
    }

    const result = node
      .addInput(inp1)
      .addInput(inp2)
      .addInput(inp3)
      .addOutput(out)

    if (node.data.nextSocketId > 4) {
      for (var x = 4; x < node.data.nextSocketId; x++) {
        var input = new Rete.Input(String(x), 'Audio', Socket.audio, false)
        result.addInput(input)
      }
    }
    return result
  }

  worker (node, inputs, outputs) {
  }
}
