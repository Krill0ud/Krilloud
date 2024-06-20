
export const addNewChannel = (state, channel) => {
  state.channels.push(channel)
}
export const setCurrentChannel = (state, channel) => {
  console.log('entra al current mutations', channel)
  state.currentChannel = channel
}

export const deleteChannel = (state, params) => {
  const index = params.index
  state.channels.splice(index, 1)
}

export const setChannels = (state, channels) => {
  state.channels = channels
}

export const setChannelLabel = (state, v) => {
  console.log('entra al channelname mutations', state.currentChannel)
  state.currentChannel.label = v
}

export const updateValueChannels = (state, v) => {
  state.channels.forEach((item, i) => {
    item.value = i
  })
  setChannels(state.channels)
}
