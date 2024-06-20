const KrilloudChannelDLL = require('./structs').KrilloudChannelDLL
const dllLib = require('../apiDLLLib').dllLib

const exportLib = {}

exportLib.KGetChannelList = () => {
  dllLib.GetChannelCount()
}

exportLib.KGetChannelCount = () => {
  var countchannels = dllLib.GetChannelCount()
  return countchannels
}

exportLib.KGetChannel = (position) => {
  var op = new KrilloudChannelDLL()
  op = dllLib.GetChannel(position)
  var json = {
    name: op.channel_name,
    id: op.channel_id,
    volume: op.volume,
    label: op.channel_name,
    value: op.channel_id
  }
  return json
}

exportLib.KCreateChannel = (channelName, parentId) => {
  const result = dllLib.CreateNewChannel(channelName, 0)
  console.log('resultChannel', result)
  if (result) {
    const countchannels = dllLib.GetChannelCount()
    const newChannel = exportLib.KGetChannel(countchannels - 1)
    console.log('newChannel', newChannel)
    return newChannel.id
  }
}

exportLib.KRemoveChannel = (position) => {
  dllLib.RemoveChannel(position)
}

exportLib.KChangeChannelVolume = (channelId, volume) => {
  dllLib.ChangeChannelVolume(channelId, parseFloat(volume))
}

exportLib.KChangeChannelName = (channelId, newName) => {
  dllLib.ChangeChannelName(channelId, newName)
}
module.exports.lib = exportLib
