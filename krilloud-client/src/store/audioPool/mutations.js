export const setAudios = (state, audios) => {
  state.audios = audios.tree
  state.indexOfAudios = audios.indexOfAudios
}
export const setshowAudioPoolViewer = (state, value) => {
  state.showAudioPoolViewer = value
}
export const setshowAddAudioModal = (state, value) => {
  state.showAddAudioModal = value
}

export const setCurrentAudioResources = (state, audios) => {
  state.currentAudioResources = audios
}
