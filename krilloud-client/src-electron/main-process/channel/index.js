const projectLib = require('../project/apiJSONLib').lib
const lib = require('./apiJSONLib').lib
const express = require('express')
const router = express.Router()

router.post('/create', function (req, res) {
  try {
    var channelName = req.body.channel.name
    var parentId = req.body.channel.id
    const idNewChannel = lib.KCreateChannel(channelName, parentId)
    console.log('idchannel', idNewChannel)
    res.send({ response: 'OK', id: idNewChannel })
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!, ('
    })
  }
})
router.delete('/:id', function (req, res) {
  const index = parseInt(req.params.id)
  lib.KRemoveChannel(index)
  /*   const channelId = 3
  let channelDeleted = false
  const channels = lib.KGetChannelCount()
  for (let x = 0; x < channels && !channelDeleted; x++) {
    const channel = lib.KGetChannel(x)
    if (channel.id === channelId) {
      lib.KRemoveChannel(parseInt(x))
      channelDeleted = true
    }
  } */
  res.send('OK')
})

router.post('/:id/volume', function (req, res) {
  const channelId = req.params.id
  const volume = req.body.volume
  lib.KChangeChannelVolume(channelId, volume)
  res.send('OK')
})

router.post('/:id/name', function (req, res) {
  const channelId = req.params.id
  const newName = req.body.newName
  lib.KChangeChannelName(channelId, newName)
  res.send('OK')
})

router.get('/', function (req, res) {
  if (!projectLib.KHasOpenedProject()) {
    res.send([])
  } else {
    const channels = lib.KGetChannelCount()
    const results = []
    for (let x = 0; x < channels; x++) {
      results.push(lib.KGetChannel(x))
    }
    res.send(results)
    console.log('resultados channels', results)
  }
})
module.exports = router
