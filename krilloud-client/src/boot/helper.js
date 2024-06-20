import VueClipboard from 'vue-clipboard2'

export default async ({ Vue }) => {
  Vue.use(VueClipboard)
  const helper = {
    formatNumber: function (value, decimals) {
      return parseFloat(value).toFixed(decimals)
    },
    pasteFromClipboard: async function () {
      const editor = Vue.prototype.$store.state.rete.editor
      const selectedList = await navigator.clipboard.readText()
      const nodeList = JSON.parse(selectedList)
      nodeList.forEach(async node => {
        if (node.name === 'Output') {
          return false
        } else {
          const { name, position: [x, y], data } = node
          const component = editor.components.get(name)
          const newNode = await component.createNode(data)
          newNode.position = [x + 10, y + 10]

          editor.addNode(newNode)
        }
      })
    },
    copyToClipboard: function () {
      Vue.prototype.$copyText(JSON.stringify(Vue.prototype.$store.state.rete.editor.selected.list))
    },
    getAudioChildren: function (array) {
      const getChildren = (result, object) => {
        if (object.isLeaf) {
          result.push(object)
          return result
        }
        if (Array.isArray(object.children)) {
          const children = object.children.reduce(getChildren, [])
          if (children.length) result.push({ ...object, children })
        }
        return result
      }

      return array.reduce(getChildren, [])
    },
    getLeafNodes: async function (nodes, result) {
      if (!result) {
        result = []
      }
      for (var i = 0, length = nodes.length; i < length; i++) {
        if (nodes[i].isLeaf && nodes[i].id >= 0) {
          result.push(nodes[i])
        } else {
          if (nodes[i].children) {
            result = await this.getLeafNodes(nodes[i].children, result)
          }
        }
      }
      return result
    },
    deleteSelection: function () {
      const selectedlist = Vue.prototype.$store.state.rete.editor.selected.list
      selectedlist.forEach(node => {
        if (node.name === 'Output') {
          return false
        } else {
          Vue.prototype.$store.state.rete.editor.removeNode(node)
        }
      })
      Vue.prototype.$store.state.rete.editor.selected.list = []
    },
    clone: function () {
      this.copyToClipboard()
      this.pasteFromClipboard()
    },
    autoConnect: function (inputs) {
      const currentPath = Vue.prototype.$store.state.rete.currentPathConnection
      if (!currentPath) {
        return
      }
      const socektType = currentPath.socket.name
      const connectSocket = inputs.find(item => item.socket.name === socektType && item.connections.length === 0)
      if (connectSocket && connectSocket.node.id !== currentPath.node.id) {
        Vue.prototype.$store.state.rete.editor.connect(currentPath, connectSocket)
        Vue.prototype.$store.commit('rete/setCurrentPathConnection', null)
      }
    },
    callNodeSelected: function (node) {
      if (Vue.prototype.$store.state.rete.currentPathConnection) {
        Vue.prototype.$store.state.rete.editor.trigger('nodeselect', node)
      }
    },
    socketConnectedClass (connection, type) {
      if (connection.length > 0) {
        if (type === 'audio') {
          return 'socket-connected-audio'
        } else {
          return 'socket-connected-variable'
        }
      }
    }
  }

  Vue.prototype.$helper = helper
}
