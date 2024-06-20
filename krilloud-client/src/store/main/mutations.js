
export const addNewChannel = (state, channel) => {
  state.channels.push(channel)
}
export const setCurrentChannel = (state, channel) => {
  state.setCurrentChannel = channel
}

export const deleteChannel = (state, channel) => {
  const index = state.channels.findIndex((item) => item.id === (channel.id))
  state.channels.splice(index, 1)
}

export const setShowDeleteElementModal = (state, v) => {
  state.showDeleteElementModal = v
}

export const setIsModal = (state, v) => {
  state.isModal = v
}

export const setModeEditing = (state, v) => {
  state.modeEditing = v
}

export const setVersionNumber = (state, v) => {
  state.versionNumber = v
}

export const setTemporalElementDrag = (state, { element, type }) => {
  state.temporalElementDrag = element
  state.temporalElementDrag.type = type
}

export const setRealtimeObjectList = (state, v) => {
  state.realtimeObjectList = v
}

export const setRealtimeTagsList = (state, v) => {
  state.realtimeTagsList = v
}

export const setRealTimeState = (state, v) => {
  state.realTimeState = v
}
