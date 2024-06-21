const lib = require('./apiJSONLib').lib
const express = require('express')
const router = express.Router()

router.post('/open', function (req, res) {
  try {
    console.log('trying to open project', req.body.path)
    const project = lib.KOpenProject(req.body.path)
    res.send({ response: 'OK', project: project })
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/create', function (req, res) {
  try {
    const projectName = req.body.name
    const projectPath = req.body.folderProject
    const gamePath = req.body.folderGame
    lib.KNewProject(projectPath, gamePath, projectName)
    res.send({ response: 'OK' })
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/close', function (req, res) {
  try {
    lib.KCloseProject()
    res.send({ response: 'OK' })
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/save', function (req, res) {
  try {
    const op = lib.KSaveCurrentProject()
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.put('/saveAs', function (req, res) {
  try {
    var projectName = req.body.projectName
    console.log(projectName)
    const op = lib.KSaveCurrentProjectAs(projectName)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/export', function (req, res) {
  try {
    const op = lib.KExportProject(req.body.alsoSave)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/changeProjectPath', function (req, res) {
  try {
    const op = lib.KSetNewGamePath(req.body.newProjectPath)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})

router.post('/changeProjectName', function (req, res) {
  try {
    const newName = req.body.newProjecName
    const op = lib.KSetNewProjectName(newName)
    res.json(op)
  } catch (e) {
    console.log(e)
    return res.status(400).send({
      message: 'This is an error!'
    })
  }
})
module.exports = router
