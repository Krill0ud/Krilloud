const lib = require('./apiJSONLib').lib
const projectLib = require('../project/apiJSONLib').lib
const express = require('express')
const router = express.Router()

/**
 * @todo: we are using in all the endpoints tagName as a parameter. we might change the path of nodes in this way: /:tagName/nodes/...
 */

/**
 * Node creation / remove
 */
router.post('/create', function (req, res) {
  const type = req.body.type
  const tagName = req.body.tagName
  const position = req.body.position
  const result = lib.KAddNode(type, tagName, position, req.body.varName)
  res.send(result)
})

router.delete('/:nodeId', function (req, res) {
  const tagName = req.body.tagName
  const nodeId = req.params.nodeId
  const result = lib.KRemoveNode(nodeId, tagName)
  res.json({ id: result })
})

/**
 * Tree
 */
router.get('/load-tree/:tag', function (req, res) {
  try {
    if (!projectLib.KHasOpenedProject()) {
      res.send()
    }
    const resultTree = {
      id: 'krill@0.1.0'
    }
    const tagName = req.params.tag
    const nodes = {}
    const list = lib.KGetNodesIDList(tagName)
    list.forEach(element => {
      const node = lib.KGetNodeData(tagName, element)
      nodes[element] = node
    })

    resultTree.nodes = nodes
    res.json(resultTree)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

/***
 * Connections
 */

router.post('/:nodeId/link/create', function (req, res) {
  const result = lib.KCreateNodeLink(req.body.tagName, req.params.nodeId, req.body.socketId,
    req.body.nodeIdToLink, req.body.socketIdToLink)
  const result2 = lib.KCreateNodeLink(req.body.tagName, req.body.nodeIdToLink, req.body.socketIdToLink,
    req.params.nodeId, req.body.socketId)
  console.log(result, result2)
  res.send(result)
})

router.delete('/:nodeId/link/:socketId', function (req, res) {
  const result = lib.KRemoveNodeLinks(req.body.tagName, req.params.nodeId, req.params.socketId)
  const result2 = lib.KRemoveNodeLinks(req.body.tagName, req.body.nodeIdToLink, req.body.socketIdToLink)
  res.send({ result: result, result2: result2 })
})

router.post('/:nodeId/socket/create/:socketId', function (req, res) {
  const result = lib.KCreateNodeSocket(req.body.tagName, req.params.nodeId, req.params.socketId,
    req.params.socketId, req.body.componentName)
  res.send(result)
})

router.post('/:nodeId/update-position', function (req, res) {
  const result = lib.KSetNodePosition(req.body.tagName, req.params.nodeId, req.body.position)
  res.send(result)
})

/**
 * @todo: we should make patchs in every update action
 */
router.patch('/:nodeId/name', function (req, res) {
  const result = lib.KSetNodeName(req.body.tagName, req.params.nodeId, req.body.name)
  res.send(result)
})

router.delete('/:nodeId/socket/:socketId', function (req, res) {
  const result = lib.KDeleteNodeSocket(req.body.tagName, req.params.nodeId, req.params.socketId)
  res.send(result)
})

// testing purpose
router.get('/load-tree2/:tag', function (req, res) {
  res.send({ response: 'OK', tree: lib.KGetNodeData('Music', 3) })
})

router.put('/:idNode/filter-parameter/:idFilter', function (req, res) {
  lib.KSetNodeFilterParameter(req.body.tagname, req.params.idNode, req.params.idFilter, req.body.value)
  res.send({ response: 'OK' })
})

// en el crete del node dependiendo de que tipo es,
// va a haber que hacer algo parecido a la preparacion del data en el load tree

router.get('/show-node-socket-info/:tag', function (req, res) {
  const tagName = req.params.tag
  const list = lib.KGetNodesIDList(tagName)
  list.forEach(element => {
    lib.KGetNodeTypeAndClass(tagName, element)
    lib.KGetConnections(tagName, element)
  })
  res.send()
})

router.get('/show-filter-parameter-info/:tag/:nodeId', function (req, res) {
  const tagName = req.params.tag
  const nodeId = req.params.nodeId
  var result = lib.KGetFilterParameters(tagName, nodeId)
  res.send(JSON.stringify(result))
})

// MIXER
router.get('/:tag/mixers/:nodeId/volumes', function (req, res) {
  const tagName = req.params.tag
  const nodeId = req.params.nodeId
  var result = lib.KGetMixerCurrentVolumes(tagName, nodeId)
  res.send(JSON.stringify(result))
})

router.put('/:tag/mixers/:idNode/volume/:index', function (req, res) {
  lib.KSetMixerSocketCurrentVolume(req.params.tag, req.params.idNode, req.params.index, req.body.value)
  res.send({ response: 'OK' })
})

router.delete('/:tag/mixers/:idNode', function (req, res) {
  lib.KDeleteMixerSocket(req.params.tag, req.params.idNode)
  res.send({ response: 'OK' })
})

router.post('/:tag/mixers/:idNode/create', function (req, res) {
  lib.KAddMixerSocket(req.params.tag, req.params.idNode)
  res.send({ response: 'OK' })
})

module.exports = router
