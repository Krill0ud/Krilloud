const dllLib = require('../apiDLLLib').dllLib
const varLib = require('../variable/apiJSONLib').lib
const audioLib = require('../audiopool/apiJSONLib').lib
const KLPair = require('./structs').KLPair
var ref = require('ref-napi')

const exportLib = {}

exportLib.KAddNode = (reteName, tagName, position, varName) => {
  try {
    let data = {}
    console.log('begin of add node')
    const nodeClass = MapReteToTypeClass[reteName].class
    const klPair = new KLPair()
    klPair.first = position[0]
    klPair.second = position[1]
    // console.log('values to add node ', nodeClass, tagName, klPair)
    var result = dllLib.AddNode(nodeClass, tagName, klPair)
    if (reteName === 'Variable') {
      const varId = dllLib.GetVariablePosition(varName)
      dllLib.SetVariableSelection(tagName, result, varId)
    }
    data = {
      newId: result
    }
    const type = dllLib.GetNodeType(tagName, result)
    if (type === NodeTypeMap.indexOf('Filter')) {
      const parameters = getFilterRange(tagName, result)
      Object.assign(data, parameters)
    }
    return data
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KRemoveNode = (nodeId, tagName) => {
  try {
    var result = dllLib.RemoveNode(nodeId, tagName)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KGetNodesIDList = (tagName) => {
  try {
    var result = []
    var length = ref.alloc(ref.types.int, 0)
    var list = dllLib.GetNodesIDList(tagName, length)
    for (var i = 0; i < length.deref(); i++) {
      result.push(list[i])
    }
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KGetNodeData = (tagName, id) => {
  var result = {}
  result.name = getReteTypeOf(tagName, id)
  result.id = id
  var pairPosition = dllLib.GetNodePosition(tagName, id)
  result.position = [pairPosition.first, pairPosition.second]
  Object.assign(result, getConnections(tagName, id))
  result.data = getDataOf(tagName, result)
  return result
}

const NodeTypeMap = ['Normal', 'Variable', 'Selector', 'Output', 'AudioContainer', 'Filter', 'Mixer']
const ClassTypeMap = ['Error', 'AudioContanier', 'Macro', 'BandPass', 'BassBoost', 'BQR', 'DCRemover', 'Delay', 'Echo', 'Flanger', 'HighPass', 'LoFi', 'LowPass', 'Pitch', 'Robotizer', 'WaveShaper', 'IngameVariable', 'Input', 'Output', 'Randomizer', 'Selector', 'Freeverb', 'Mixer']
const MapReteToTypeClass = {
  Output: { type: 3, class: 18 },
  AudioContainer: { type: 4, class: 1 },
  Selector: { type: 2, class: 20 },
  Mixer: { type: 6, class: 22 },
  Variable: { type: 1, class: 16 },
  BassBoost: { type: 5, class: 4 },
  DCRemover: { type: 5, class: 6 },
  Echo: { type: 5, class: 8 },
  Flanger: { type: 5, class: 9 },
  LoFi: { type: 5, class: 11 },
  Pitch: { type: 5, class: 13 },
  Robotizer: { type: 5, class: 14 },
  WaveShaper: { type: 5, class: 15 }
}

function getReteTypeOf (tagName, nodeId) {
  const type = dllLib.GetNodeType(tagName, nodeId)
  if (type === NodeTypeMap.indexOf('Filter') || type === NodeTypeMap.indexOf('Normal')) {
    const nodeClass = dllLib.GetNodeClass(tagName, nodeId)
    return ClassTypeMap[nodeClass]
  }
  const nodeType = NodeTypeMap[type]
  if (!nodeType) {
    // console.log('Type of Node not found')
    return false
  }
  return nodeType
}

exportLib.KGetNodeTypeAndClass = (tagName, id) => {
  console.log('type', dllLib.GetNodeType(tagName, id))
  console.log('class', dllLib.GetNodeClass(tagName, id))
  console.log('position', dllLib.GetNodeSocketPosition(tagName, id, 3))
}

function getDataOf (tagName, node) {
  let data = {}
  const type = dllLib.GetNodeType(tagName, node.id)
  if (type === NodeTypeMap.indexOf('Variable')) {
    const variable = varLib.KGetNodeAssociatedKVar(tagName, node.id)
    const varId = dllLib.GetVariablePosition(variable.name)
    data = {
      name: variable.name,
      id: varId
    }
    return data
  } else if (type === NodeTypeMap.indexOf('Filter')) {
    const result = getFilterRange(tagName, node.id)
    return result
    // console.log('datafilter', data)
  } else if (type === NodeTypeMap.indexOf('Selector')) {
    const countSocket = dllLib.GetNodeSocketCount(tagName, node.id)
    data.nextSocketId = countSocket
  } else if (type === NodeTypeMap.indexOf('Mixer')) {
    const countSocket = dllLib.GetNodeSocketCount(tagName, node.id)
    data.nextSocketId = countSocket

    var lengthM = ref.alloc(ref.types.int, 0)
    var mixers = dllLib.GetMixerCurrentVolumes(tagName, node.id, lengthM)
    var resultM = []
    for (var j = 0; j < lengthM.deref(); j++) {
      resultM.push(mixers[j])
    }
    // console.log('mixers', resultM)
    data.volumes = resultM
  } else if (type === NodeTypeMap.indexOf('AudioContainer')) {
    data.name = dllLib.GetNodeName(tagName, node.id)
    console.log('node name', data.name)
    var audioList = audioLib.KGetContainerdIDs(tagName, node.id)
    data.audioList = audioList || []
    data.mode = audioLib.KGetAudioSourceSelectionMode(tagName, node.id)
    data.loop = audioLib.KGetAudioContainerLoopable(tagName, node.id)
    // console.log('el loop en el getdataof', data.loop)
  }
  return data
}

function getFilterRange (tagName, nodeId) {
  const data = {}
  var length = ref.alloc(ref.types.int, 0)
  var filterParameters = dllLib.GetNodeFilterParameters(tagName, nodeId, length)
  var result = []
  for (var i = 0; i < length.deref(); i++) {
    result.push(filterParameters[i])
  }
  result.forEach(item => {
    data[camelize(item.name)] = {
      value: item.value,
      id: result.indexOf(item),
      minValue: item.min_value,
      maxValue: item.max_value
    }
  })
  return data
}

function camelize (str) {
  return str.replace(/(?:^\w|[A-Z]|\b\w|\s+)/g, function (match, index) {
    if (+match === 0) return '' // or if (/\s+/.test(match)) for white spaces
    return index === 0 ? match.toLowerCase() : match.toUpperCase()
  })
}

exportLib.KGetNodeType = (tagName, nodeId) => {
  const type = dllLib.GetNodeType(tagName, nodeId)
  const nodeName = dllLib.GetNodeName(tagName, nodeId)
  var nodeData = {
    name: nodeName,
    type: type
  }
  return nodeData
}

exportLib.KGetFilterParameters = (tagName, nodeId) => {
  var length = ref.alloc(ref.types.int, 0)
  var list = dllLib.GetNodeFilterParameters(tagName, nodeId, length)
  var result = []
  for (var i = 0; i < length.deref(); i++) {
    // console.log('recorroro filters', list[i])
    result.push(list[i])
  }
  return result
}

exportLib.KGetMixerCurrentVolumes = (tagName, nodeId) => {
  var length = ref.alloc(ref.types.int, 0)
  var list = dllLib.GetMixerCurrentVolumes(tagName, nodeId, length)
  var result = []
  for (var i = 0; i < length.deref(); i++) {
    // console.log('recorroro filters', list[i])
    result.push(list[i])
  }
  return result
}

exportLib.KSetMixerSocketCurrentVolume = (tagname, nodeId, parameterId, value) => {
  dllLib.SetMixerSocketCurrentVolume(tagname, nodeId, parameterId, value)
}

exportLib.KDeleteMixerSocket = (tagName, nodeId) => {
  const countSocket = dllLib.GetNodeSocketCount(tagName, nodeId)
  dllLib.DeleteNodeSocket(tagName, nodeId, countSocket - 2)
}

exportLib.KAddMixerSocket = (tagName, nodeId) => {
  const countSocket = dllLib.GetNodeSocketCount(tagName, nodeId)
  const socketId = countSocket - ((countSocket - 1) / 2)
  exportLib.KCreateNodeSocket(tagName, nodeId, socketId, socketId)
}

const SocketType = ['Input', 'Output']
/* function getConnectionsTypeOf (type, nodeSockets) {
  const result = {}
  nodeSockets.forEach(item => {
    const connections = []
    nodeSockets.links.forEach(link => {
      const element = {}
      element.node = link.connected_node_id
      element.data = {}
      element[type === SocketType.indexOf('Input') ? 'input' : 'output'] = link.connected_node_socket_id
      connections.push(element)
    })
    result[item.socket_id] = {
      connections: connections
    }
  })
  return result
} */

exportLib.KGetConnections = (tagName, id) => {
  return getConnections(tagName, id)
}

function getConnections (tagName, nodeId) {
  const inputs = {}
  const outputs = {}
  const countSocket = dllLib.GetNodeSocketCount(tagName, nodeId)
  // console.log('count socket', countSocket)
  // const inputSocket = dllLib.GetNodeInputsCount(tagName, nodeId)
  // console.log('count input', inputSocket)
  // const outputSocket = dllLib.GetNodeOutputsCount(tagName, nodeId)
  // console.log('count output', outputSocket)
  for (var x = 0; x < countSocket; x++) {
    const elem = {}
    const type = dllLib.GetNodeSocketType(tagName, nodeId, x)
    // console.log('socket id', x, ' type ', type, ' data type', dllLib.GetNodeSocketDataType(tagName, nodeId, x))
    elem.type = type

    const connections = []
    const countLinks = dllLib.GetNodeSocketLinksCount(tagName, nodeId, x)
    // console.log('count links socket', countLinks)
    for (var y = 0; y < countLinks; y++) {
      const linkedId = dllLib.GetLinkedID(tagName, nodeId, x, y)
      const linkedIdSocket = dllLib.GetLinkedIDSocket(tagName, nodeId, x, y)
      const linkElem = {
        node: linkedId
      }
      // console.log('link Elem', linkElem)
      if (type === SocketType.indexOf('Input')) {
        linkElem.output = linkedIdSocket.toString()
      } else {
        linkElem.input = linkedIdSocket.toString()
      }
      linkElem.data = {}
      connections.push(linkElem)
    }

    // if (connections.length > 0) {
    // console.log('Conexiones nodo socket', x, connections)
    if (type === SocketType.indexOf('Input')) {
      inputs[x] = { connections: connections }
    } else {
      outputs[x] = { connections: connections }
    }
    // }
  }
  return { inputs: inputs, outputs: outputs }
}

exportLib.KRemoveNodeLinks = (tagName, nodeId, socketId) => {
  return dllLib.RemoveNodeLinks(tagName, nodeId, parseInt(socketId))
}

exportLib.KCreateNodeLink = (tagName, nodeId, socketId, nodeIdToLink, socketIdToLink) => {
  return dllLib.CreateNodeLink(tagName, nodeId, parseInt(socketId), nodeIdToLink, parseInt(socketIdToLink))
}

exportLib.KDeleteNodeSocket = (tagName, nodeId, socketId) => {
  return dllLib.DeleteNodeSocket(tagName, nodeId, socketId)
}

exportLib.KCreateNodeSocket = (tagName, nodeId, position, socketId, componentName) => {
  const type = 0
  const dataType = 0
  const isDeleteable = true
  const multipleLinks = true
  /**
   * Krilloud::Nodes::NodeSockets::CreateNodeSocket(myOwner->GetAssociatedTagName(), myID, 50 + (16 * (float)inputsCount * 2), socketCount - ((socketCount-1)/2), Input, Normal, true, false);
   */
  console.log('create node socket ', tagName, nodeId, position, socketId, type, dataType, isDeleteable, multipleLinks)
  console.log('antes de crear count', dllLib.GetNodeSocketCount(tagName, nodeId))
  console.log('result', dllLib.CreateNodeSocket(tagName, nodeId, parseFloat(position), socketId, type, dataType, isDeleteable, multipleLinks))
  console.log('count', dllLib.GetNodeSocketCount(tagName, nodeId))
  /* if (componentName === 'Selector') {
  } else if (componentName === 'Mixer') {

  } */
}

exportLib.KSetNodeFilterParameter = (tagname, nodeId, parameterId, value) => {
  dllLib.SetNodeFilterParameter(tagname, nodeId, parameterId, value)
}

exportLib.KSetNodePosition = (tagname, nodeId, position) => {
  const klPair = new KLPair()
  klPair.first = position[0]
  klPair.second = position[1]
  dllLib.SetNodePosition(tagname, nodeId, klPair)
}

exportLib.KSetNodeName = (tagname, nodeId, name) => {
  console.log(tagname, nodeId, name)
  try {
    dllLib.SetNodeName(tagname, nodeId, name)
  } catch (e) {
    console.log(e)
  }
}

module.exports.lib = exportLib
