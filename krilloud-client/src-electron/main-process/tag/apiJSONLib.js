const dllLib = require('../apiDLLLib').dllLib
const KLTag = require('./structs').KLTag

const exportLib = {}

exportLib.KGetTag = (tagName) => {
  var op1 = new KLTag()
  console.log(op1)
  var op = dllLib.GetTag(tagName)
  var json = {
    name: op.tag_name,
    audio3D: op.is3D || false,
    attenuation: op.attenuation,
    channel: op.channel_id,
    minDistance: op.min_3D_distance.toFixed(3),
    maxDistance: op.max_3D_distance.toFixed(3),
    rolloff: op.roll_off_factor.toFixed(3) || false,
    isdoppler: op.doppler_active || false,
    doppler: op.doppler_factor.toFixed(3),
    delay: op.distance_delay || false,
    loop: op.isLoopable || false
  }
  return json
}

exportLib.KGetTagList = () => {
  const tagCount = dllLib.GetTagCount()
  const result = []
  for (let x = 0; x < tagCount; x++) {
    result.push(dllLib.GetTagName(x))
  }
  return result
}

exportLib.KPlayTag = (tagName) => {
  dllLib.PlayTag(tagName)
}

exportLib.KStopTag = (tagName) => {
  dllLib.StopTag(tagName)
}

exportLib.KCloneTag = (tagName) => {
  dllLib.CloneTag(tagName)
}

exportLib.KGetTagCount = () => {
  return dllLib.GetTagCount()
}

exportLib.KIsTagPlaying = (tagName) => {
  return dllLib.IsTagPlaying(tagName)
}

exportLib.KCreateNewTag = (tag) => {
  dllLib.CreateNewTag(tag.name)
  dllLib.SetTagLoopable(tag.name, tag.loop)
  dllLib.SetTagChannel(tag.name, tag.channel)
  dllLib.SetTagAttenuation(tag.name, tag.attenuation)
  dllLib.SetTag3D(tag.name, tag.audio3D)
  dllLib.SetTagDistanceDelay(tag.name, tag.delay)
  dllLib.SetTagDopplerEffect(tag.name, tag.isdoppler)
  dllLib.SetTagDopplerFactor(tag.name, tag.doppler)
  dllLib.SetTagMinMaxDistance(tag.name, tag.minDistance, tag.maxDistance)
  dllLib.SetTagRollOffFactor(tag.name, tag.rolloff)
}
exportLib.KRenameTag = (oldName, newName) => {
  const result = dllLib.RenameTag(oldName, newName)
  return result
}

exportLib.KRemoveTag = (tagName) => {
  dllLib.RemoveTag(tagName)
}

exportLib.KSetTagLoopable = (tagName, value) => {
  dllLib.SetTagLoopable(tagName, value)
}

exportLib.KSetTagChannel = (tagName, value) => {
  dllLib.SetTagChannel(tagName, value)
}

exportLib.KSetTagAttenuation = (tagName, value) => {
  dllLib.SetTagAttenuation(tagName, value)
}

exportLib.KSetTag3D = (tagName, value) => {
  dllLib.SetTag3D(tagName, value)
}

exportLib.KSetTagDistanceDelay = (tagName, value) => {
  dllLib.SetTagDistanceDelay(tagName, value)
}

exportLib.KSetTagDopplerEffect = (tagName, value) => {
  dllLib.SetTagDopplerEffect(tagName, value)
}

exportLib.KSetTagDopplerFactor = (tagName, value) => {
  dllLib.SetTagDopplerFactor(tagName, value)
}

exportLib.KSetTagMinMaxDistance = (tagName, value) => {
  dllLib.SetTagMinMaxDistance(tagName, value.minDistance, value.maxDistance)
}

exportLib.KSetTagRollOffFactor = (tagName, value) => {
  dllLib.SetTagRollOffFactor(tagName, value)
}

module.exports.lib = exportLib
