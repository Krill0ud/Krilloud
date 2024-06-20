const baseApiUrl = `${process.env.API_URL}/tags`
import axios from 'axios'

export async function save ({ dispatch, commit, rootGetters }, tag) {
  const result = await axios.post(
    `${baseApiUrl}/create`, tag
  )
  console.log(result)

  if (!tag.data) {
    tag.data = rootGetters['rete/initialData']
  }
  tag.header = 'generic'
  commit('addNewTag', tag)
  dispatch('rete/loadTag', tag.id, { root: true })
}

export async function init ({ commit, dispatch, rootGetters, state }) {
  const tags = await axios.get(
    `${baseApiUrl}/`
  )
  commit('setTags', tags.data)
  if (state.tags.length === 0) {
    const tag = { id: 1, name: 'Tag 1' }
    // tag.data = rootGetters['rete/initialData']
    commit('addNewTag', tag)
    commit('setCurrentTag', tag)
  } else {
    commit('setCurrentTag', state.tags[0])
  }
  if (rootGetters['project/isOpenedProject']) {
    await dispatch('rete/loadTag', null, { root: true })
  }
  // call api state.tags
}

export async function close ({ commit, dispatch, rootGetters, state }) {
  commit('setTags', [])
}

export async function deleteTag ({ dispatch, commit, getters, state }, tag) {
  const result = await axios.delete(
    `${baseApiUrl}/${tag}`)
  console.log(result)
  commit('deleteTag', tag)
  const firstTag = state.tags[0]
  dispatch('rete/loadTag', firstTag.id, { root: true })
}

export async function renameTag ({ dispatch, commit, getters, state }, tag) {
  const oldName = tag.oldName
  const newName = tag.newName
  const result = await axios.put(
    `${baseApiUrl}/${oldName}/rename`, { newName })
  console.log(result)
  /* const tagElement = state.tags.find(item => item.name === oldName)
  if (tagElement.name === state.currentTag.name) {
    state.currentTag.name = tagElement.name
  }
  tagElement.name = newName */
}

export async function play ({ state }) {
  const result = await axios.post(
    `${baseApiUrl}/${state.currentTag.name}/play`)
  console.log(result)
}

export async function stop ({ state, commit }) {
  const result = await axios.post(
    `${baseApiUrl}/${state.currentTag.name}/stop`)
  commit('setIsPlaying', result.data.isPlaying)
  console.log(result)
}

export async function clone ({ state, commit }, payload) {
  await axios.post(
    `${baseApiUrl}/${payload.tagName}/clone`)
  const tags = await axios.get(
      `${baseApiUrl}/`
  )
  commit('setTags', tags.data)
}

export async function isPlaying ({ state, commit }) {
  const result = await axios.get(
    `${baseApiUrl}/${state.currentTag.name}/check-playing-status`)
  commit('setIsPlaying', result.data.isPlaying)
  console.log(result)
  return result
}

export async function setAttribute ({ state, rootState }, params) {
  let attribute = params.attribute
  let data = {}
  let currentTagName = params.tagName
  if (params.updateRef) {
    currentTagName = params.tagName
    attribute = params.attribute
    data = { value: params.value }
  } else {
    data = { value: params.value }
  }
  const result = await axios.post(
    `${baseApiUrl}/${currentTagName}/${attribute}`, data)
  // state.currentTag[params.attribute] = data.value
  const item = state.tags.find(item => item.name === params.tagName)
  item[params.attribute] = data.value
  return result
}

export async function updateChannelReference ({ state }, params) {
  const channelId = params.channel.channelId
  state.tags.forEach(item => {
    if (item.channel === channelId) {
      const channelInfo = {
        attribute: 'channel',
        value: 0,
        currentTag: item.name,
        updateRef: true
      }
      setAttribute({ state }, channelInfo)
      item.channel = 0
    }
  })
}
