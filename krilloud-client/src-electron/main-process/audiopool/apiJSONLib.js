const dllLib = require('../apiDLLLib').dllLib
const KrilloudPoolFile = require('./structs').KrilloudPoolFile
var ref = require('ref-napi')

const exportLib = {}

exportLib.KRefreshPool = () => {
  dllLib.RefreshPool()
}

exportLib.KGetAudioPoolList = () => {
  const count = dllLib.GetPoolFileCount()
  const results = []
  for (let x = 0; x < count; x++) {
    results.push(x)
  }
  return results
}

exportLib.KGetFileData = (id) => {
  var op = new KrilloudPoolFile()
  op = dllLib.GetFileData(id)
  var json = {
    id: op.id,
    fullPath: op.full_file_name,
    fileName: op.fileName
  }
  return json
}

function convertToArray (arrayToConvert, length) {
  const array = []
  for (var j = 0; j < length; j++) {
    const folderTree = arrayToConvert[j]
    array.push(folderTree)
  }
  return array
}

function getInitialTree () {
  return [
    {
      label: 'Sound Files',
      name: 'Sound Files',
      root: true,
      dragDisabled: true,
      addTreeNodeDisabled: true,
      addLeafNodeDisabled: true,
      editNodeDisabled: true,
      delNodeDisabled: true,
      children: [
      ]
    }
  ]
}

function getArray (fileData) {
  if (fileData.id === -1) {
    const name = fileData.full_file_name.split('/')
    fileData.file_name = name[name.length - 1]
    const array = fileData.full_file_name.split('/')
    return array
  } else {
    const array = convertToArray(fileData.folder_tree, fileData.folder_tree_length)
    return array
  }
}

exportLib.KGetPoolTree = () => {
  dllLib.RefreshPool()
  try {
    var tree = getInitialTree()
    var indexOfAudios = []
    const lengthFilesKLPollFileArray = ref.alloc(ref.types.int, 0)
    const files = dllLib.GetPoolTree2(lengthFilesKLPollFileArray)

    for (var i = 0; i < lengthFilesKLPollFileArray.deref(); i++) {
      let fileData = ref.get(files[i], 0, KrilloudPoolFile)
      fileData = fileData.toJSON()
      // console.log('datosdll', fileData)

      const array = getArray(fileData)

      const soundFilesIndex = array.indexOf('Sound Files')
      // console.log('indexof sound files', soundFilesIndex)

      const fullPathSoundFiles = fileData.full_file_name.substring(0, fileData.full_file_name.indexOf('Sound Files')) + 'Sound Files'
      tree[0].id = soundFilesIndex
      tree[0].fullPath = fullPathSoundFiles

      let lastChildren = tree[0].children
      let lastPath = fullPathSoundFiles

      for (var y = (soundFilesIndex + 1); y < array.length; y++) {
        lastPath = lastPath + '/' + array[y]
        const elemToAdd = {
          label: array[y],
          name: array[y],
          id: new Date().getTime(),
          dragDisabled: false,
          addTreeNodeDisabled: true,
          addLeafNodeDisabled: true,
          editNodeDisabled: true,
          delNodeDisabled: true,
          fullPath: lastPath,
          children: []
        }
        const childrenIfExists = lastChildren.find(item => item.label === array[y])

        if (!childrenIfExists) {
          lastChildren.push(elemToAdd)
          lastChildren = elemToAdd.children
        } else {
          lastChildren = childrenIfExists.children
        }
      }
      if (fileData.id > 0) {
        const audio = {
          id: fileData.id,
          label: fileData.file_name,
          name: fileData.file_name,
          fullPath: fileData.full_file_name,
          delNodeDisabled: true,
          editNodeDisabled: true,
          isLeaf: true
        }
        lastChildren.push(audio)
        indexOfAudios.push(audio)
      } else {
      }
    }
    return { tree: tree, indexOfAudios: indexOfAudios }
  } catch
  (e) {
    console.log(e)
    return e
  }
}

exportLib.KGetPoolItemID = (fileName) => {
  var int = dllLib.GetPoolItemID(fileName)
  return int
}

exportLib.KGetContainerdIDs = (tagName, nodeId) => {
  try {
    var result = []
    var length = ref.alloc(ref.types.int, 0)
    var listaudioSources = dllLib.GetContainedIDs(tagName, nodeId, length)
    for (var i = 0; i < length.deref(); i++) {
      const listAudios = listaudioSources[i]
      const dataFile = dllLib.GetFileData(listaudioSources[i].id)
      var audio = {
        id: listAudios.id,
        fullPathName: listAudios.filename,
        fileName: dataFile.file_name,
        volume: listAudios.volume
      }
      result.push(audio)
    }
    // console.log('este es el getcontainer final', JSON.stringify(result))
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KAddSound = (tagName, nodeId, soundId, fileFullPath, volume) => {
  try {
    var result = dllLib.AddSound(tagName, nodeId, soundId, fileFullPath, volume)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KRemoveSound = (tagName, nodeId, soundId) => {
  try {
    var result = dllLib.RemoveSound(tagName, nodeId, soundId)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KSetContainedClipVolume = (tagName, nodeId, soundId, volume) => {
  try {
    var result = dllLib.SetContainedClipVolume(tagName, nodeId, soundId, volume)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KGetAudioContainerLoopable = (tagName, nodeId) => {
  try {
    // console.log('begin GetLoopableAudioContainer')
    var result = dllLib.GetAudioContainerLoopable(tagName, nodeId)
    // console.log('se ejecutó el GetAudioContainerLoopable', result)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}
exportLib.KSetAudioContainerLoopable = (tagName, nodeId, loop) => {
  try {
    // console.log('begin SetLoopableAudioContainer')
    var result = dllLib.SetAudioContainerLoopable(tagName, nodeId, loop)
    // console.log('se ejecutó el SetLoopableAudioContainer')
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KSetAudioSourceSelectionMode = (tagName, nodeId, mode) => {
  try {
    var result = dllLib.SetAudioSourceSelectionMode(tagName, nodeId, mode)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KGetAudioSourceSelectionMode = (tagName, nodeId) => {
  try {
    var result = dllLib.GetAudioSourceSelectionMode(tagName, nodeId)
    return result || 0
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KCreateNewSubdirectory = (path) => {
  try {
    var result = dllLib.CreateNewSubdirectory(path.replace(/\//g, '\\\\'))
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KMovePoolDirectory = (source, output) => {
  try {
    var result = dllLib.MovePoolDirectory(source.replace(/\//g, '\\\\'), output.replace(/\//g, '\\\\'))
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KDeleteDirectory = (path) => {
  try {
    var result = dllLib.DeleteDirectory(path.replace(/\//g, '\\\\'))
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KRemovePoolFile = (path, updateFileData) => {
  try {
    var result = dllLib.RemovePoolFile(path, updateFileData)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KRenamePoolFile = (name, newName) => {
  try {
    var result = dllLib.RenamePoolFile(name, newName)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KMovePoolFile = (name, path) => {
  try {
    var result = dllLib.MovePoolFile(name.replace(/\//g, '//'), path.replace(/\//g, '//'))
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

exportLib.KAddFiles = (pathList, path) => {
  try {
    var filesToAdd = []
    pathList.forEach((item, index) => {
      filesToAdd[index] = item.replace(/\//g, '\\\\')
    })
    var result = dllLib.AddFiles(filesToAdd, pathList.length, path)
    return result
  } catch (e) {
    console.log(e)
    return e
  }
}

module.exports.lib = exportLib
