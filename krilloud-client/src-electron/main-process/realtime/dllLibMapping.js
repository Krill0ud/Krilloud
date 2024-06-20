const KrilloudIngameObject = require('./structs').KrilloudIngameObject
const KrilloudPlayingTag = require('./structs').KrilloudPlayingTag

var ref = require('ref-napi')
var ArrayType = require('ref-array-di')(ref)

const KLIngameObjectArray = ArrayType(KrilloudIngameObject)
var inGameObject = new KLIngameObjectArray(256)
console.log(inGameObject)

const KLPlayingtagArray = ArrayType(KrilloudPlayingTag)
var playingTag = new KLPlayingtagArray(256)
console.log(playingTag)

const libMap = {
  Connect: [
    ref.types.void, ['string', 'string']
  ],
  Disconnect: [
    ref.types.void, []
  ],
  IsConectionActive: [
    ref.types.bool, []
  ],
  ConnectionStablished: [
    ref.types.bool, []
  ],
  GetIngameObjectList: [
    KLIngameObjectArray, [ref.refType(ref.types.int)]
  ],
  GetPlayingTagsList: [
    KLPlayingtagArray, [ref.refType(ref.types.int)]
  ],
  IsServerActive: [
    ref.types.bool, []
  ]
}

module.exports.dllLibFunctions = libMap
