
const KLTag = require('./structs').KLTag
var ref = require('ref-napi')

const libMap = {
  GetTag: [
    KLTag, ['string']
  ],
  GetTagName: [
    'string', [ref.types.size_t]
  ],
  PlayTag: [
    ref.types.void, ['string']
  ],
  StopTag: [
    ref.types.void, ['string']
  ],
  CloneTag: [
    ref.types.void, ['string']
  ],
  GetTagCount: [
    ref.types.size_t, []
  ],
  IsTagPlaying: [
    ref.types.bool, ['string']
  ],
  RenameTag: [
    ref.types.bool, ['string', 'string']
  ],
  CreateNewTag: [
    ref.types.void, ['string']
  ],
  RemoveTag: [
    ref.types.void, ['string']
  ],
  SetTagLoopable: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  SetTagChannel: [
    ref.types.bool, ['string', ref.types.int32]
  ],
  SetTagAttenuation: [
    ref.types.bool, ['string', ref.types.int32]
  ],
  SetTag3D: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  SetTagDistanceDelay: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  SetTagDopplerEffect: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  SetTagDopplerFactor: [
    ref.types.bool, ['string', ref.types.float]
  ],
  SetTagMinMaxDistance: [
    ref.types.bool, ['string', ref.types.float, ref.types.float]
  ],
  SetTagRollOffFactor: [
    ref.types.bool, ['string', ref.types.float]
  ]
}

module.exports.dllLibFunctions = libMap
