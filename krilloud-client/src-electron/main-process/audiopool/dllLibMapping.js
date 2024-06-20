
const KrilloudPoolFile = require('./structs').KrilloudPoolFile
const KrilloudAudioSource = require('./structs').KrilloudAudioSource

var ref = require('ref-napi')
var ArrayType = require('ref-array-di')(ref)

const KLPoolFileArray = ArrayType(KrilloudPoolFile)
// var string = ref.types.
// const StringArray = ArrayType(string)
// const stringArray = new StringArray(256)
const PoolTreeArray = ArrayType(ref.types.CString)
console.log(new PoolTreeArray(1024))
var poolFileArray = new KLPoolFileArray(1024)
console.log(poolFileArray) // , stringArray)
const KrilloudAudioSourceArray = ArrayType(KrilloudAudioSource)
var audioSource = new KrilloudAudioSourceArray(1024)
console.log(audioSource)

const libMap = {
  RefreshPool: [
    ref.types.void, []
  ],
  GetPoolTree: [
    KLPoolFileArray, [ref.refType(ref.types.int)]
  ],
  GetPoolTree2: [
    KLPoolFileArray, [ref.refType(ref.types.int)]
  ],
  GetPoolFileCount: [
    ref.types.size_t, []
  ],
  GetFileData: [
    KrilloudPoolFile, ['int']
  ],
  GetPoolItemID: [
    ref.types.int32, ['string']
  ],
  AddFiles: [
    ref.types.void, [PoolTreeArray, ref.types.int, ref.types.CString]
  ],
  MovePoolFile: [
    ref.types.bool, ['string', 'string']
  ],
  RenamePoolFile: [
    ref.types.bool, ['string', 'string']
  ],
  RemovePoolFile: [
    ref.types.bool, ['string', ref.types.bool]
  ],
  SetAudioContainerLoopable: [
    ref.types.void, ['string', 'int', ref.types.bool]
  ],
  GetAudioContainerLoopable: [
    ref.types.bool, ['string', 'int']
  ],
  RemoveSound: [
    ref.types.int32, ['string', 'int', 'int']
  ],
  AddSound: [
    ref.types.int32, ['string', 'int', 'int', 'string', 'int']
  ],
  GetContainedIDs: [
    KrilloudAudioSourceArray, ['string', 'int', ref.refType(ref.types.int)]
  ],
  SetContainedClipVolume: [
    ref.types.void, ['string', 'int', 'int', ref.types.float]
  ],
  CreateNewSubdirectory: [
    ref.types.bool, [ref.types.CString]
  ],
  MovePoolDirectory: [
    ref.types.bool, [ref.types.CString, ref.types.CString]
  ],
  DeleteDirectory: [
    ref.types.bool, [ref.types.CString]
  ],
  SetAudioSourceSelectionMode: [
    ref.types.void, [ref.types.CString, ref.types.int, ref.types.int]
  ],
  GetAudioSourceSelectionMode: [
    ref.types.int, [ref.types.CString, ref.types.int]
  ]
}

module.exports.dllLibFunctions = libMap
