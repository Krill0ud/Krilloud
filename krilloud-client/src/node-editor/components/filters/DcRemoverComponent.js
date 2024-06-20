import Rete from 'rete'
import * as Socket from 'src/node-editor/sockets.js'
import CustomDcRemoverComponent from './DcRemoverComponent.vue'

export class DcRemoverComponent extends Rete.Component {
  constructor () {
    super('DCRemover')
    this.data.render = 'vue'
    this.data.component = CustomDcRemoverComponent
    this.data.props = {}
    this.data.path = 3
  }

  builder (node) {
    var inp1 = new Rete.Input('0', 'Value', Socket.audio, false)
    var inp2 = new Rete.Input('2', 'Value', Socket.variable, false)
    var out = new Rete.Output('1', 'Value', Socket.audio, false)

    if (Object.keys(node.data).length === 0) {
      node.data = {
        wet: { id: 0, value: 0.000, minValue: 0, maxValue: 1 }
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
