import Action from '../action'

// The saved connection may have been removed and recreated, so make sure we are working with the correct reference
/* function findNewConnection (oldConnection) {
  const { input, output } = oldConnection

  return output.connections.find(c => c.input === input)
} */

class ConnectionActionHelper {
  constructor (editor, connection) {
    this.editor = editor
    this.connection = connection
  }

  add () {
    const nodes = this.editor.nodes
    const nodeInput = nodes.find(item => item.id === this.connection.input.node.id)
    const input = nodeInput.inputs.get(this.connection.input.key)
    const nodeOutput = nodes.find(item => item.id === this.connection.output.node.id)
    const output = nodeOutput.outputs.get(this.connection.output.key)
    this.editor.connect(output, input)
  }

  remove () {
    const nodes = this.editor.nodes
    const nodeInput = nodes.find(item => item.id === this.connection.input.node.id)
    const input = nodeInput.inputs.get(this.connection.input.key)
    const nodeOutput = nodes.find(item => item.id === this.connection.output.node.id)
    const output = nodeOutput.outputs.get(this.connection.output.key)
    const conn = output.connections.find(c => c.input === input)
    this.editor.removeConnection(conn)
  }
}

export class AddConnectionAction extends Action {
  constructor (editor, connection) {
    super()
    this.helper = new ConnectionActionHelper(editor, connection)
  }

  undo () { this.helper.remove() }

  redo () { this.helper.add() }
}

export class RemoveConnectionAction extends Action {
  constructor (editor, connection) {
    super()
    this.helper = new ConnectionActionHelper(editor, connection)
  }

  undo () { this.helper.add() }

  redo () { this.helper.remove() }
}
