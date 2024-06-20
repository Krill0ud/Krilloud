const dllLib = require('../apiDLLLib').dllLib
const KrilloudIngameObject = require('./structs').KrilloudIngameObject
const KrilloudPlayingTag = require('./structs').KrilloudPlayingTag

var ref = require('ref-napi')
const exportLib = {}

exportLib.KConnect = (ip, port) => {
  dllLib.Connect(ip, port)
}

exportLib.KDisconnect = () => {
  dllLib.Disconnect()
}

exportLib.KIsConectionActive = () => {
  const op = dllLib.IsConectionActive()
  return op
}

exportLib.KConnectionStablished = () => {
  var op = dllLib.ConnectionStablished()
  return op
}

exportLib.KIsServerActive = () => {
  const op = dllLib.IsServerActive()
  return op
}

exportLib.KGetIngameObjectList = () => {
  var length = ref.alloc(ref.types.int, 0)
  var result = []
  var objects = new KrilloudIngameObject()
  objects = dllLib.GetIngameObjectList(length)
  for (var i = 0; i < length.deref(); i++) {
    var object = {
      id: objects[i].object_id
    }
    result.push(object)
  }
  return result
}

exportLib.KGetPlayingTagsList = () => {
  var length = ref.alloc(ref.types.int, 0)
  var resultTags = []
  var tags = new KrilloudPlayingTag()
  tags = dllLib.GetPlayingTagsList(length)
  for (var j = 0; j < length.deref(); j++) {
    var tag = {
      tagname: tags[j].tagname,
      channelId: tags[j].channel_id,
      is3D: tags[j].is3D,
      stop: tags[j].stop
    }
    resultTags.push(tag)
  }
  return resultTags
}

module.exports.lib = exportLib
