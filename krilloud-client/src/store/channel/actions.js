import axios from 'axios'
const routeBase = `${process.env.API_URL}/channels`

export async function save ({ state, commit }, channel) {
  const result = await axios.post(
    `${routeBase}/create`, { channel }
  )
  console.log('kcreated', result)
  channel.id = result.data.id
  commit('addNewChannel', channel)
  await commit('updateValueChannels')
}

export async function init ({ commit }) {
  const channels = await axios.get(
    `${routeBase}/`
  )
  commit('setChannels', channels.data)
  await commit('updateValueChannels')
  return channels.data
}

export async function deleteChannel ({ state, dispatch, commit, rootGetters }, channel) {
  await dispatch('tag/updateChannelReference', { channel }, { root: true })
  const result = await axios.delete(
    `${routeBase}/${channel.index}`)
  console.log(result)

  commit('deleteChannel', channel)
  await commit('updateValueChannels')
}

export async function setVolume ({ commit }, channel) {
  const channelId = channel.id
  const volume = channel.volume
  const result = await axios.post(
    `${routeBase}/${channelId}/volume`, { volume }
  )
  return result
}

export async function changeChannelName ({ commit, dispatch }, channel) {
  const channelId = channel.id
  const newName = channel.name
  const result = await axios.post(
    `${routeBase}/${channelId}/name`, { newName }
  )
  return result
}
