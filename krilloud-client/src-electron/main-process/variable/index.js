const lib = require('./apiJSONLib').lib
const projectLib = require('../project/apiJSONLib').lib
const express = require('express')
const router = express.Router()

router.post('/create', function (req, res) {
  lib.KCreateNewVariable(req.body)
  res.send({ response: 'OK' })
})

router.delete('/:name', async function (req, res) {
  const result = await lib.KRemoveVar(req.params.name)
  res.send(result)
})

router.post('/:name/name', function (req, res) {
  lib.KSetVariableName(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/value', function (req, res) {
  lib.KSetVariableCurrentValue(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/type', function (req, res) {
  lib.KSetVariableType(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/source', function (req, res) {
  lib.KSetVariableSource(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/content', function (req, res) {
  lib.KSetVariableContent(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/range-min', function (req, res) {
  lib.KSetVariableRangeMin(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/range-max', function (req, res) {
  lib.KSetVariableRangeMax(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.post('/:name/on-play', function (req, res) {
  lib.KSetVariableOnPlay(req.params.name, req.body.value)
  res.send({ response: 'OK' })
})

router.get('/', function (req, res) {
  if (!projectLib.KHasOpenedProject()) {
    res.send([])
  } else {
    const varsNames = lib.KGetVariableNameList()
    const results = []
    varsNames.forEach(item => {
      const elem = lib.KGetVariable(item)
      elem.id = varsNames.indexOf(item)
      results.push(elem)
    })
    // console.log('ready to return', results)
    res.send(results)
  }
})

router.post('/:name/save', function (req, res) {
  const variable = req.body
  lib.KSaveVariable(variable)
  res.send({ response: 'OK' })
})

module.exports = router
