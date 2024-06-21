const KLInfoDLL = require('./structs').KLInfoDLL
const dllLib = require('../apiDLLLib').dllLib

const exportLib = {}
exportLib.KOpenProject = (path) => {
  var json = {}
  if (dllLib.HasOpenedProject() && path) {
    dllLib.CloseCurrentProject()
  }
  if (!dllLib.HasOpenedProject() && path) {
    const op = dllLib.OpenProject(path)
    json.first = op.first
    json.second = op.second
  }
  const projectName = dllLib.GetProjectName()
  const projectPath = dllLib.GetProjectPath()
  const projectGame = dllLib.GetGamePath()
  const result = {
    name: projectName,
    folderProject: projectPath,
    folderGame: projectGame
  }
  return result
}

exportLib.KNewProject = (projectPath, gamePath, projectName) => {
  var json = {}
  var op = new KLInfoDLL()
  op = dllLib.NewProject(projectPath, gamePath, projectName)
  json.first = op.first
  json.second = op.second
  return json
}

exportLib.KCloseProject = () => {
  var op = dllLib.CloseCurrentProject()
  console.log('Project closed ', op)
}

exportLib.KSaveProject = () => {
  var json = {}
  var op = dllLib.SaveCurrentProject()
  console.log('Project save')
  return json
}

exportLib.KSaveCurrentProjectAs = (projectName) => {
  var json = {}

  var op = dllLib.SaveCurrentProjectAs(projectName)
  console.log('Project save as: ', op)
  return json
}

exportLib.KExportProject = (alsoSave) => {
  var json = {}
  dllLib.ExportCurrentProject(alsoSave)
  return json
}

exportLib.KHasOpenedProject = () => {
  return dllLib.HasOpenedProject()
}

exportLib.KSetNewProjectPath = (newPath) => {
  var op = dllLib.SetNewProjectPath(newPath)
  console.log('path changed', op)
  return op
}

exportLib.KSetNewGamePath = (newPath) => {
  var op = dllLib.SetNewGamePath(newPath)
  console.log('path changed', op)
  return op
}

exportLib.KSetNewProjectName = (newName) => {
  var op = dllLib.SetNewProjectName(newName)
  console.log('name changed ', op)
  return op
}

module.exports.lib = exportLib
