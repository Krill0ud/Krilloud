export function modalOn (state, getters, rootState) {
  const showExportProjectModal = rootState.project.showExportProjectModal
  const showOptionsProjectModal = rootState.project.showOptionsProjectModal
  const showNewProjectModal = rootState.project.showNewProjectModal
  const showSaveAsProjectModal = rootState.project.showSaveAsProjectModal
  const showAudioPoolViewer = rootState.audioPool.showAudioPoolViewer
  const isModal = rootState.main.isModal
  const modeEditing = rootState.main.modeEditing
  if (showAudioPoolViewer || showExportProjectModal || showOptionsProjectModal || showNewProjectModal || showSaveAsProjectModal || isModal || modeEditing) {
    return true
  } else {
    return false
  }
}
