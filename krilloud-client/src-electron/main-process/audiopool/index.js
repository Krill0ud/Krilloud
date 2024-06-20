const lib = require('./apiJSONLib').lib
const projectLib = require('../project/apiJSONLib').lib
const express = require('express')
const router = express.Router()
const { shell } = require('electron')
var fs = require('fs')
var path = require('path')

router.post('/refresh', function (req, res) {
  console.log('refresh audio pool')
  // const op = lib.OpenProject()
  // console.log('project opened2')
  res.json({})
})

router.get('/tree', function (req, res) {
  if (!projectLib.KHasOpenedProject()) {
    res.send([])
  } else {
    const results = lib.KGetPoolTree()
    res.send(results)
  }
})

router.get('/', function (req, res) {
  if (!projectLib.KHasOpenedProject()) {
    res.send([])
  } else {
    const audioList = lib.KGetAudioPoolList()
    console.log(audioList)
    const results = []
    audioList.forEach(item => {
      const elem = lib.KGetFileData(item.id)
      elem.header = 'generic'
      results.push(elem)
    })
    // console.log('ready to return', results)
    res.send(results)
  }
})

router.get('/show-audio-source-info/:tag/:nodeId', function (req, res) {
  const tagName = req.params.tag
  const nodeId = req.params.nodeId
  const audioList = lib.KGetContainerdIDs(tagName, nodeId)
  res.send(JSON.stringify(audioList))
})

router.put('/:tag/:nodeId/add-sound/:soundId', function (req, res) {
  lib.KAddSound(req.params.tag, req.params.nodeId, req.params.soundId, req.body.fileFullPath, req.body.volume)
  res.send({ response: 'OK' })
})

router.delete('/:tag/:nodeId/delete-sound/:soundId', function (req, res) {
  lib.KRemoveSound(req.params.tag, req.params.nodeId, req.params.soundId)
  res.send({ response: 'OK' })
})

router.put('/:tag/:nodeId/set-volume/:soundId', function (req, res) {
  lib.KSetContainedClipVolume(req.params.tag, req.params.nodeId, req.params.soundId, req.body.volume)
  res.send({ response: 'OK' })
})

router.put('/:tag/:nodeId/set-loopable', function (req, res) {
  lib.KSetAudioContainerLoopable(req.params.tag, req.params.nodeId, req.body.loop)
  res.send({ response: 'OK' })
})

router.post('/add-files', function (req, res) {
  const result = lib.KAddFiles(req.body.pathList, req.body.path)
  res.send({ response: 'OK', result: result })
})

router.post('/move-file', function (req, res) {
  const result = lib.KMovePoolFile(req.body.source, req.body.output)
  res.send({ response: 'OK', result: result })
})

router.post('/rename-file', function (req, res) {
  const result = lib.KRenamePoolFile(req.body.fileName, req.body.newFileName)
  res.send({ response: 'OK', result: result })
})

router.delete('/file', function (req, res) {
  const result = lib.KRemovePoolFile(req.body.path, true)
  res.send({ response: 'OK', result: result })
})

router.delete('/directory', function (req, res) {
  const result = lib.KDeleteDirectory(req.body.fullpath)
  res.send({ response: 'OK', result: result })
})

router.post('/move-directory', function (req, res) {
  const result = lib.KMovePoolDirectory(req.body.source, req.body.output)
  res.send({ response: 'OK', result: result })
})

router.post('/create-directory', function (req, res) {
  const result = lib.KCreateNewSubdirectory(req.body.path)
  res.send({ response: 'OK', result: result })
})

router.post('/selection-mode/', function (req, res) {
  const result = lib.KSetAudioSourceSelectionMode(req.body.tagName, req.body.nodeId, req.body.mode)
  res.send({ response: 'OK', result: result })
})

router.get('/show-in-explorer/:path', function (req, res) {
  shell.openPath(req.params.path)
  res.send({ response: 'OK' })
})

router.post('/add-directories', function (req, res) {
  copyFolderRecursiveSync(req.body.source, req.body.target)
  res.send({ response: 'OK' })
})

function copyFolderRecursiveSync (source, target) {
  var files = []

  // Check if folder needs to be created or integrated
  var targetFolder = path.join(target, path.basename(source))
  if (!fs.existsSync(targetFolder)) {
    console.log('mkdir ' + targetFolder)
    fs.mkdirSync(targetFolder)
  }

  // Copy
  if (fs.lstatSync(source).isDirectory()) {
    files = fs.readdirSync(source)
    files.forEach(function (file) {
      var curSource = path.join(source, file)
      if (fs.lstatSync(curSource).isDirectory()) {
        console.log('isDirectory ', curSource, targetFolder)
        copyFolderRecursiveSync(curSource, targetFolder)
      } else {
        console.log('is Not Directory ', curSource, targetFolder)
        copyFileSync(curSource, targetFolder)
      }
    })
  }
}

function copyFileSync (source, target) {
  var targetFile = target

  // If target is a directory, a new file with the same name will be created
  if (fs.existsSync(target)) {
    if (fs.lstatSync(target).isDirectory()) {
      targetFile = path.join(target, path.basename(source))
    }
  }
  console.log('copy file', targetFile, source)
  fs.writeFileSync(targetFile, fs.readFileSync(source))
}

module.exports = router
