const lib = require('./apiJSONLib').lib
const projectLib = require('../project/apiJSONLib').lib
const express = require('express')
const router = express.Router()

router.post('/:name/play', function (req, res) {
  console.log(req.params.name)
  console.log('Play')
  lib.KPlayTag(req.params.name)
  res.send({ response: 'OK' })
})

router.post('/:name/stop', function (req, res) {
  console.log('Stop', req.params.name)
  lib.KStopTag(req.params.name)
  const isPlaying = lib.KIsTagPlaying(req.params.name)
  res.send({ response: 'OK', isPlaying: isPlaying })
})

router.get('/:name/check-playing-status', function (req, res) {
  const isPlaying = lib.KIsTagPlaying(req.params.name)
  res.send({ isPlaying })
})

router.post('/create', function (req, res) {
  console.log('create new tag playing')
  lib.KCreateNewTag(req.body)
  res.send({ response: 'OK' })
})

router.delete('/:name', function (req, res) {
  console.log('delete tag playing')
  lib.KRemoveTag(req.params.name)
  console.log(req.params.name)
  res.send({ response: 'OK' })
})

router.put('/:name/rename', function (req, res) {
  console.log('rename tag index', req.params.name, req.body)
  const op = lib.KRenameTag(req.params.name, req.body.newName)
  console.log('Rename tag?', op)
  res.send(op)
})

router.post('/:name/loop', function (req, res) {
  console.log('setting tag loop', req.params.name, req.body.value)
  lib.KSetTagLoopable(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/channel', function (req, res) {
  console.log('setting channel', req.params.name, req.body.value)
  lib.KSetTagChannel(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/attenuation', function (req, res) {
  lib.KSetTagAttenuation(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/audio3D', function (req, res) {
  lib.KSetTag3D(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/delay', function (req, res) {
  lib.KSetTagDistanceDelay(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/doppler', function (req, res) {
  lib.KSetTagDopplerFactor(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/isdoppler', function (req, res) {
  lib.KSetTagDopplerEffect(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/min-max-distance', function (req, res) {
  lib.KSetTagMinMaxDistance(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/rolloff', function (req, res) {
  lib.KSetTagRollOffFactor(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/clone', function (req, res) {
  lib.KCloneTag(req.params.name)
  res.send({ response: 'OK' })
})

router.get('/', function (req, res) {
  if (!projectLib.KHasOpenedProject()) {
    res.send([])
  } else {
    const tags = lib.KGetTagList()
    const results = []
    tags.forEach(item => {
      const tag = lib.KGetTag(item)
      tag.id = tags.indexOf(item)
      tag.isPlaying = false
      tag.header = 'generic'
      results.push(tag)
    })
    // console.log('ready to return tags', results)
    res.send(results)
  }
})

module.exports = router
