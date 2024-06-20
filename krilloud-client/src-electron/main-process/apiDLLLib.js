const ffi = require('ffi-napi')
const path = require('path')
const projectMapping = require('./project/dllLibMapping')
const tagMapping = require('./tag/dllLibMapping')
const channelMapping = require('./channel/dllLibMapping')
const variableMapping = require('./variable/dllLibMapping')
const audiosMapping = require('./audiopool/dllLibMapping')
const nodeMapping = require('./node/dllLibMapping')
const realTimeMapping = require('./realtime/dllLibMapping')
let dir
if (process.env.PROD) {
  global.__statics = __dirname
  dir = path.join(__dirname, '..', 'lib', 'Krilloud_API_x64.dll')
} else {
  dir = path.join(__statics, 'lib', 'Krilloud_API_x64.dll')
}

let lib
const libPayload = Object.assign({},
  projectMapping.dllLibFunctions,
  tagMapping.dllLibFunctions,
  channelMapping.dllLibFunctions,
  variableMapping.dllLibFunctions,
  nodeMapping.dllLibFunctions,
  audiosMapping.dllLibFunctions,
  realTimeMapping.dllLibFunctions
)
// console.log('libPayload', libPayload)
try {
  lib = new ffi.Library(dir, libPayload
    // TAGS
    // END TAGS
    /*
    Play: [
      ref.types.void, []
    ],
    Stop: [
      ref.types.void, []
    ],
    RefreshAudioPool: [
      ref.types.void, []
    ],
    GetPoolFileCount: [
      ref.types.size_t, []
    ],
    CloseProject: [
      ref.types.void, []
    ] */
  )
  // console.log('lib', lib)
} catch (e) {
  console.log(e)
  throw e
}

module.exports.dllLib = lib
