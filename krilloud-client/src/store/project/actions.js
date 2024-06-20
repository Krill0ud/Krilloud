import axios from 'axios'
import { Loading } from 'quasar'
const routeBase = `${process.env.API_URL}/projects`

export async function getOpenedProject ({ commit, dispatch, getters }, path) {
  const result = await axios.post(
    `${routeBase}/open`, {
      path: path
    }
  )
  return result.data.project
}

export async function open ({ commit, dispatch, getters }, path) {
  Loading.show({
    message: 'Loading project'
  })
  await dispatch('variable/close', {}, { root: true })
  await dispatch('tag/close', {}, { root: true })
  commit('setProject', {})

  console.log(process.env.API_URL)
  const result = await axios.post(
    `${routeBase}/open`, {
      path: path
    }
  )
  const projectResult = result.data.project
  projectResult.path = path
  await commit('setProject', result.data.project)
  await dispatch('channel/init', {}, { root: true })
  await dispatch('variable/init', {}, { root: true })
  await dispatch('audioPool/init', {}, { root: true })
  await dispatch('tag/init', {}, { root: true })
  const date = (new Date()).toLocaleDateString()
  commit('addRecentProject', { project: projectResult, date: date })
  commit('setShowOpenProjectModal', false)
  commit('audioPool/setshowAudioPoolViewer', false, { root: true })
  console.log(result)
  Loading.hide()
}
export async function saveNewProject ({ commit, dispatch }, project) {
  console.log('entra en actions')
  commit('setProject', project)
  const result = await axios.post(
    `${routeBase}/create`, project
  )
  console.log(result)
  await dispatch('getOpenedProject', `${project.folderProject}/${project.name}.kpj`)
  dispatch('main/relaunch', `${project.folderProject}/${project.name}.kpj`, { root: true })
  // dispatch('open', `${project.folderProject}/${project.name}.kpj`)
  // TODO ver si limpiamos a mano todo el store, o si nos lo iremos trayendo de la api dispatch('rete/loadTag', null, { root: true })
}

export async function close ({ commit, dispatch }) {
  const result = await axios.post(
    `${routeBase}/close`, {}
  )
  await dispatch('variable/close', {}, { root: true })
  await dispatch('tag/close', {}, { root: true })
  commit('setProject', {})
  commit('setShowOpenProjectModal', true)
  console.log(result, 'close actions')
}

export async function save ({ commit }, project) {
  const result = await axios.post(
    `${routeBase}/save`, {}
  )
  const date = (new Date()).toLocaleDateString()
  commit('addRecentProject', { project: project, date: date })
  console.log(result, 'save actions')
}

export async function saveAs ({ commit }, projectName) {
  const result = await axios.put(
    `${routeBase}/saveAs`, { projectName: projectName }
  )
  // commit('addRecentProject', projectName)
  console.log(result, 'save actions')
}

export async function exportProject ({ commit }, alsoSave) {
  const result = await axios.post(
    `${routeBase}/export`, { alsoSave: alsoSave }
  )
  return result
}

export async function changeProjectPath ({ commit }, newProjectPath) {
  const result = await axios.post(
    `${routeBase}/changeProjectPath`, { newProjectPath: newProjectPath }
  )
  if (result.data) {
    commit('setProjectPath', newProjectPath)
  }
}

export async function changeProjectName ({ commit }, newProjectName) {
  const result = await axios.post(
    `${routeBase}/changeProjectName`, { newProjecName: newProjectName }
  )
  if (result.data) {
    commit('setProjectName', newProjectName)
  }
}
