import { LocalStorage } from 'quasar'
import Vue from 'vue'

export const setProject = (state, project) => {
  state.project = project
}

export const setProjectName = (state, newName) => {
  state.project.name = newName
}

export const setProjectPath = (state, newPath) => {
  state.project.folderGame = newPath
}

export const setShowOpenProjectModal = (state, value) => {
  state.showOpenProjectModal = value
}

export const setShowSaveAsProjectModal = (state, value) => {
  state.showSaveAsProjectModal = value
}

export const setShowNewProjectModal = (state, value) => {
  state.showNewProjectModal = value
}
export const showOptionsProjectModal = (state, value) => {
  state.showOptionsProjectModal = value
}
export const showExportProjectModal = (state, value) => {
  state.showExportProjectModal = value
}

export const addRecentProject = (state, project) => {
  if (!state.recentProjects.find(item => item.project.path === project.project.path)) {
    state.recentProjects.unshift(project)
    LocalStorage.set(`krill-recents-${Vue.prototype.$store.state.main.versionNumber}`, JSON.stringify(state.recentProjects))
  } else {
    const indexOfProj = state.recentProjects.findIndex(item => item.project.path === project.project.path)
    state.recentProjects[indexOfProj] = project
    LocalStorage.set(`krill-recents-${Vue.prototype.$store.state.main.versionNumber}`, JSON.stringify(state.recentProjects))
  }
}

export const setRecentProjects = (state, recents) => {
  state.recentProjects = recents
}
