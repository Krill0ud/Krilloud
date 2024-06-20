var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)
var ArrayType = require('ref-array-di')(ref)
const PoolTreeArray = ArrayType(ref.types.CString)
var poolFileArray = new PoolTreeArray(1024)
console.log(poolFileArray)

const KrilloudPoolFile = struct({
  id: ref.types.int,
  full_file_name: ref.types.CString,
  file_name: ref.types.CString,
  folder_tree: PoolTreeArray,
  folder_tree_length: ref.types.int
})

const KrilloudAudioSource = struct({
  id: ref.types.int,
  filename: ref.types.CString,
  volume: ref.types.float
})

module.exports.KrilloudAudioSource = KrilloudAudioSource
module.exports.KrilloudPoolFile = KrilloudPoolFile
