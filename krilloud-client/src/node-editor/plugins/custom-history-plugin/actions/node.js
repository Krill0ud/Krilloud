import Action from '../action'

class NodeAction extends Action {
  constructor (editor, node) {
    super()
    this.editor = editor
    this.node = node
  }
}

export class AddNodeAction extends NodeAction {
  undo () {
    const nodes = this.editor.nodes
    const nodeInstance = nodes.find(item => item.id === this.node.id)
    this.editor.removeNode(nodeInstance)
  }

  redo () {
    this.editor.addNode(this.node)
  }
}

export class RemoveNodeAction extends NodeAction {
  undo () {
    this.editor.addNode(this.node)
  }

  redo () {
    const nodes = this.editor.nodes
    const nodeInstance = nodes.find(item => item.id === this.node.id)
    this.editor.removeNode(nodeInstance)
  }
}

export class DragNodeAction extends NodeAction {
  constructor (editor, node, prev) {
    super(editor, node)

    this.prev = [...prev]
    this.new = [...node.position]
  }

  _translate (position) {
    const nodes = this.editor.nodes
    const nodeInstance = nodes.find(item => item.id === this.node.id)
    this.editor.view.nodes.get(nodeInstance).translate(...position)
  }

  undo () {
    this._translate(this.prev)
  }

  redo () {
    this._translate(this.new)
  }

  update (node) {
    this.new = [...node.position]
  }
}
