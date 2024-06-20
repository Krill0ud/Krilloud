
const KrilloudNode = require('./structs').KrilloudNode
const KLPair = require('./structs').KLPair
const FilterParameter = require('./structs').FilterParameter
var ref = require('ref-napi')
var ArrayType = require('ref-array-di')(ref)

var IntArray = ArrayType(ref.types.int)
var intType = new IntArray(256)
console.log(intType)

var FloatArray = ArrayType(ref.types.float)
var floatType = new FloatArray(256)
console.log(floatType)

var FilterParameterArray = ArrayType(FilterParameter)
var filterArray = new FilterParameterArray(256)
console.log(filterArray)

const libMap = {
  GetNodesIDList: [
    IntArray, [ref.types.CString, ref.refType(ref.types.int)]
  ],
  AddNode: [
    ref.types.int, [ref.types.int, ref.types.CString, KLPair]
  ],
  RemoveNode: [
    ref.types.bool, [ref.types.int, ref.types.CString]
  ],
  GetNodeData: [
    KrilloudNode, [ref.types.CString, ref.types.int]
  ],
  GetNodeName: [
    ref.types.CString, [ref.types.CString, ref.types.int]
  ],
  GetNodePosition: [
    KLPair, [ref.types.CString, ref.types.int]
  ],
  GetNodeType: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ],
  GetNodeSocketPosition: [
    ref.types.float, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetNodeClass: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ],
  GetNodeSocketCount: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ],
  GetNodeOutputsCount: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ],
  GetNodeInputsCount: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ],
  GetNodeSocketType: [
    ref.types.int, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetNodeSocketDataType: [
    ref.types.int, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetNodeSocketLinksCount: [
    ref.types.int, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetLinkedID: [
    ref.types.int, [ref.types.CString, ref.types.int, ref.types.int, ref.types.int]
  ],
  GetLinkedIDSocket: [
    ref.types.int, [ref.types.CString, ref.types.int, ref.types.int, ref.types.int]
  ],
  RemoveNodeLinks: [
    ref.types.bool, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  CreateNodeLink: [
    ref.types.bool, [ref.types.CString, ref.types.int, ref.types.size_t, ref.types.int, ref.types.size_t]
  ],
  DeleteNodeSocket: [
    ref.types.bool, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  CreateNodeSocket: [
    ref.types.bool, [ref.types.CString, ref.types.int, ref.types.float, ref.types.int, ref.types.int, ref.types.int, ref.types.bool, ref.types.bool]
  ],
  GetNodeFilterParameters: [
    FilterParameterArray, [ref.types.CString, ref.types.int, ref.refType(ref.types.int)]
  ],
  GetPoolTree: [
    ref.types.void, ['char ***', 'int *']
  ],
  SetNodeFilterParameter: [
    ref.types.void, ['string', 'int', 'int', ref.types.float]
  ],
  SetNodeName: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.CString]
  ],
  SetNodePosition: [
    ref.types.void, [ref.types.CString, ref.types.int, KLPair]
  ],
  SetVariableSelection: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetMixerCurrentVolumes: [
    FloatArray, [ref.types.CString, ref.types.int, ref.refType(ref.types.int)]
  ],
  SetMixerSocketCurrentVolume: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.int, ref.types.float]
  ]
}

module.exports.dllLibFunctions = libMap
