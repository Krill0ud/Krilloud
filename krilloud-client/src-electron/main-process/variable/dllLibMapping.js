
const KrilloudVariable = require('./structs').KrilloudVariable

var ref = require('ref-napi')

const libMap = {
  GetVariablePosition: [
    ref.types.int32, ['string']
  ],
  GetKVariable: [
    KrilloudVariable, ['string']
  ],
  GetVariableName: [
    'string', [ref.types.size_t]
  ],
  GetKVarCount: [
    ref.types.size_t, []
  ],
  CreateNewVariable: [
    ref.types.void, ['string']
  ],
  RemoveVariable: [
    ref.types.void, [ref.types.int32]
  ],
  SetVariableName: [
    ref.types.bool, ['string', 'string']
  ],
  SetVariableCurrentValue: [
    ref.types.bool, ['string', ref.types.float]
  ],
  SetVariableType: [
    ref.types.bool, ['string', ref.types.int]
  ],
  SetVariableSource: [
    ref.types.bool, ['string', ref.types.int]
  ],
  SetVariableContent: [
    ref.types.bool, ['string', ref.types.int]
  ],
  SetVariableRangeMin: [
    ref.types.bool, ['string', ref.types.float]
  ],
  SetVariableRangeMax: [
    ref.types.bool, ['string', ref.types.float]
  ],
  SetVariableOnPlay: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  GetNodeAssociatedKVar: [
    KrilloudVariable, [ref.types.CString, ref.types.int]
  ],
  SetSimulatedValue: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.float]
  ],
  SetOnlyOnPlay: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.bool]
  ],
  SetVariableSelection: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetVariableSelection: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ]
}

module.exports.dllLibFunctions = libMap
