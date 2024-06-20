const KrilloudChannelDLL = require('./structs').KrilloudChannelDLL
var ref = require('ref-napi')

const libMap = {
  CreateNewChannel: [
    ref.types.bool, ['string', 'int']
  ],
  GetChannelCount: [
    ref.types.int32, []
  ],
  GetChannelName: [
    ref.types.CString, ['int']
  ],
  GetChannel: [
    KrilloudChannelDLL, ['int']
  ],
  RemoveChannel: [
    ref.types.void, ['int']
  ],
  ChangeChannelVolume: [
    ref.types.void, ['int', 'float']
  ],
  ChangeChannelName: [
    ref.types.void, ['int', 'string']
  ]

}

module.exports.dllLibFunctions = libMap
