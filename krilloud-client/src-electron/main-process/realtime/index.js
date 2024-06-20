const lib = require('./apiJSONLib').lib
const express = require('express')
const router = express.Router()
const ip = require('ip')

router.put('/connect', function (req, res) {
  const ipNode = ip.address()
  try {
    lib.KConnect(ipNode, req.body.port)
    res.send({ response: 'OK' })
  } catch (e) {
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/disconnect', function (req, res) {
  try {
    lib.KDisconnect()
    res.send({ response: 'OK' })
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.get('/isConectionActive', function (req, res) {
  try {
    const op = lib.KIsConectionActive()
    return res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.get('/connectionStablished', function (req, res) {
  try {
    const op = lib.KConnectionStablished()
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.get('/isServerActive', function (req, res) {
  try {
    const op = lib.KIsServerActive()
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.get('/ingame-object-list', function (req, res) {
  try {
    const op = lib.KGetIngameObjectList()
    console.log('ingame-object-list', op)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.get('/playing-tags-list', function (req, res) {
  try {
    const op = lib.KGetPlayingTagsList()
    console.log('playing-tags-list', op)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

module.exports = router
