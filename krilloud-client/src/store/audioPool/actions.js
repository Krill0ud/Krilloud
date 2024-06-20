const baseApiUrl = `${process.env.API_URL}/audio-pools`
import axios from 'axios'

export async function init ({ commit, getters, rootGetters, state }) {
  const audios = await axios.get(
    `${baseApiUrl}/tree`
  )
  commit('setAudios', audios.data)
}

export async function getAudioContainer ({ state }, params) {
  var tagName = params.tagName
  var nodeId = params.nodeId
  const audiosSelected = await axios.get(
    `${baseApiUrl}/show-audio-source-info/${tagName}/${nodeId}/`
  )
  return audiosSelected.data
}

export async function addSound ({ state, rootState }, params) {
  var tagName = params.tagName || rootState.tag.currentTag.name
  var nodeId = params.nodeId
  var soundId = params.soundId
  var fileFullPath = params.fullPathName
  var volume = params.volume
  var data = { fileFullPath: fileFullPath, volume: volume }
  const soundAdded = await axios.put(
    `${baseApiUrl}/${tagName}/${nodeId}/add-sound/${soundId}`, data
  )
  return soundAdded.data
}

export async function removeSound ({ state, dispatch }, params) {
  var tagName = params.tagName
  var nodeId = params.nodeId
  var soundId = params.soundId
  const result = await axios.delete(
    `${baseApiUrl}/${tagName}/${nodeId}/delete-sound/${soundId}`
  )
  console.log(result, 'removeSound')
  dispatch('init')
}

export async function setAudioVolume ({ state }, params) {
  const tagName = params.tagName
  const nodeId = params.nodeId
  const soundId = params.soundId
  const value = params.value
  const soundAdded = await axios.put(
    `${baseApiUrl}/${tagName}/${nodeId}/set-volume/${soundId}`, { volume: value }
  )
  return soundAdded.data
}

export async function renameFile ({ commit, state, dispatch }, params) {
  const oldFileName = params.audio.fullPath
  const newFileName = oldFileName.substring(0, (oldFileName.lastIndexOf('/') || oldFileName.lastIndexOf('\\')) + 1) + params.newName + oldFileName.substring(oldFileName.lastIndexOf('.'))
  const result = await axios.post(
    `${baseApiUrl}/rename-file`, {
      fileName: oldFileName,
      newFileName: newFileName
    }
  )
  console.log(result, 'rename file')
  dispatch('init')
}

export async function removeFile ({ commit, state, dispatch }, fullPath) {
  const result = await axios.delete(
    `${baseApiUrl}/file`, {
      data: {
        path: fullPath
      }
    }
  )
  console.log(result, 'remove file')
  dispatch('init')
}

export async function addFiles ({ commit, state, dispatch }, params) {
  const result = await axios.post(
    `${baseApiUrl}/add-files`, {
      path: params.destFolder,
      pathList: params.filePaths
    }
  )
  console.log(result, 'add file')
  dispatch('init')
}

export async function addDirectories ({ commit, state, dispatch }, params) {
  const result = await axios.post(
    `${baseApiUrl}/add-directories`, {
      target: params.target,
      source: params.source
    }
  )
  console.log(result, 'add directories')
  dispatch('init')
}

export async function removeDirectory ({ commit, dispatch }, fullpath) {
  const result = await axios.delete(
    `${baseApiUrl}/directory`, {
      data: {
        fullpath: fullpath
      }
    }
  )
  console.log(result, 'remove file')
  dispatch('init')
}

export async function moveDirectory ({ commit, dispatch }, params) {
  const result = await axios.post(
    `${baseApiUrl}/move-directory`, {
      source: params.source,
      output: params.output
    }
  )
  console.log(result, 'remove file')
  dispatch('init')
}

export async function moveFile ({ commit, dispatch }, params) {
  const result = await axios.post(
    `${baseApiUrl}/move-file`, {
      source: params.source,
      output: params.output
    }
  )
  console.log(result, 'remove file')
  dispatch('init')
}

export async function createDirectory ({ commit, dispatch }, params) {
  const result = await axios.post(
    `${baseApiUrl}/create-directory`, {
      path: params
    }
  )
  console.log(result, 'remove file')
  dispatch('init')
}

export async function setSelectionMode ({ state, rootState }, params) {
  const tagName = rootState.tag.currentTag.name
  const nodeId = params.nodeId
  const mode = params.mode
  const result = await axios.post(
    `${baseApiUrl}/selection-mode/`, { tagName: tagName, nodeId: nodeId, mode: mode }
  )
  return result
}

export async function setAudioContainerLoopable ({ state, rootState }, params) {
  console.log('entro al actions de setAudioContainerLoopable', params)
  const tagName = rootState.tag.currentTag.name
  const nodeId = params.nodeId
  const loop = params.loop
  const result = await axios.put(
    `${baseApiUrl}/${tagName}/${nodeId}/set-loopable`, { loop: loop }
  )
  return result
}

export async function showInExplorer ({ state, rootState }, params) {
  const result = await axios.get(
    `${baseApiUrl}/show-in-explorer/${encodeURIComponent(params)}`, { }
  )
  return result
}
