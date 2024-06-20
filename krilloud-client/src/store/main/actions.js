import axios from 'axios'
const baseApiUrl = `${process.env.API_URL}`
import { debounce } from 'quasar'
// import SetFilterParameterAction from '../../node-editor/actions/SetFilterParameterAction'
// import SetMixerVolumeAction from '../../node-editor/actions/SetMixerVolumeAction'

export async function loadTree ({ state }, name) {
  const result = await axios.get(
    `${baseApiUrl}/nodes/load-tree/${name}`, {}
  )
  return result.data
}

export async function createNode ({ state, rootState }, payload) {
  const result = await axios.post(
    `${baseApiUrl}/nodes/create`, {
      tagName: rootState.tag.currentTag.name,
      type: payload.type,
      position: payload.position,
      varName: payload.varName,
      parameterValues: payload.parameterValues
    }
  )
  return result.data
}

export async function removeNode ({ state, rootState }, id) {
  const result = await axios.delete(
    `${baseApiUrl}/nodes/${id}`, {
      data: {
        tagName: rootState.tag.currentTag.name
      }
    }
  )
  return result.data
}

export async function createConnection ({ state, rootState }, payload) {
  const result = await axios.post(
    `${baseApiUrl}/nodes/${payload.nodeId}/link/create`, {
      tagName: rootState.tag.currentTag.name,
      socketId: payload.socketId,
      nodeIdToLink: payload.nodeIdToLink,
      socketIdToLink: payload.socketIdToLink
    }
  )
  return result.data
}

export async function removeConnection ({ state, rootState }, payload) {
  const result = await axios.delete(
    `${baseApiUrl}/nodes/${payload.nodeId}/link/${payload.socketId}`, {
      data: {
        tagName: rootState.tag.currentTag.name,
        socketId: payload.socketId,
        nodeIdToLink: payload.nodeIdToLink,
        socketIdToLink: payload.socketIdToLink
      }
    }
  )
  return result
}

export async function addSocket ({ state, rootState }, payload) {
  const result = await axios.post(
    `${baseApiUrl}/nodes/${payload.nodeId}/socket/create/${payload.socketId}`, {
      tagName: rootState.tag.currentTag.name,
      componentName: payload.componentName
    }
  )
  return result.data
}

export async function removeSocket ({ state, rootState }, payload) {
  const result = await axios.delete(
    `${baseApiUrl}/nodes/${payload.nodeId}/socket/${payload.socketId}`, {
      data: {
        tagName: rootState.tag.currentTag.name
      }
    }
  )
  return result.data
}

const setFilterParameterDeb = debounce(async function (state, rootState, params) {
  console.log(params)
  const tagname = params.tagname
  const idNode = params.idNode
  const idFilter = params.id
  const value = params.value
  const prevValue = params.prevValue
  const data = { tagname: tagname, value: value }
  // const name = params.name
  if (prevValue !== undefined) { // means that wasn call from own history plugin
    /* rootState.rete.editor.trigger('addhistory', new SetFilterParameterAction(
      { id: idFilter, value: prevValue, tagname: tagname, idNode: idNode, name: name },
      { id: idFilter, value: value, tagname: tagname, idNode: idNode, name: name })) */
  }

  const result = await axios.put(
    `${baseApiUrl}/nodes/${idNode}/filter-parameter/${idFilter}`, data)
  console.log(result)
  return result
}, 500)

export async function setFilterParameter ({ state, rootState }, params) {
  setFilterParameterDeb(state, rootState, params)
}

const setMixerParameterDeb = debounce(async function (state, rootState, params) {
  const tagname = rootState.tag.currentTag.name
  const idNode = params.idNode
  const index = params.index
  const value = params.volume
  const data = { value: value }
  const prevValue = params.prevValue

  if (prevValue !== undefined) { // means that wasn call from own history plugin
    /* rootState.rete.editor.trigger('addhistory', new SetMixerVolumeAction(
      { index: index, value: prevValue, tagname: tagname, idNode: idNode },
      { index: index, value: value, tagname: tagname, idNode: idNode })) */
  }

  const result = await axios.put(
      `${baseApiUrl}/nodes/${tagname}/mixers/${idNode}/volume/${index}`, data)
  console.log(result)
  console.log('datica', data)
  return result
}, 0)

export async function getFilterParameter ({ state, rootState }, params) {
  console.log('estos son los parametros q llegan', params)
  const tagName = rootState.tag.currentTag.name
  const idNode = params.nodeId
  const result = await axios.get(
    `${baseApiUrl}/nodes/show-filter-parameter-info/${tagName}/${idNode}`)
  console.log('parámetros del filtro', result)
  return result
}

export async function setMixerVolume ({ state, rootState }, params) {
  setMixerParameterDeb(state, rootState, params)
  /* console.log('estos son los parametros q llegan', params)
  const tagname = rootState.tag.currentTag.name
  const idNode = params.nodeId
  const index = params.index
  const value = params.volume
  const data = { value: value }
  const result = await axios.put(
    `${baseApiUrl}/nodes/${tagname}/mixers/${idNode}/volume/${index}`, data)
  console.log(result)
  return result */
}

export async function removeMixerSocket ({ state, rootState }, params) {
  const tagname = rootState.tag.currentTag.name
  const idNode = params.nodeId
  const result = await axios.delete(
    `${baseApiUrl}/nodes/${tagname}/mixers/${idNode}`, {
      data: {
      }
    })
  console.log(result)
  return result
}

export async function createMixerSocket ({ state, rootState }, params) {
  const tagname = rootState.tag.currentTag.name
  const idNode = params.nodeId
  const result = await axios.post(
    `${baseApiUrl}/nodes/${tagname}/mixers/${idNode}/create`, {
    })
  console.log(result)
  return result
}

export async function renameNode ({ state, rootState }, params) {
  const tagname = rootState.tag.currentTag.name
  const idNode = params.nodeId
  const name = params.name
  const result = await axios.patch(
    `${baseApiUrl}/nodes/${idNode}/name`, {
      name: name,
      tagName: tagname
    })
  console.log(result)
  return result
}

const updateNodePositionDeb = debounce(async function (state, rootState, payload) {
  const result = await axios.post(
  `${baseApiUrl}/nodes/${payload.nodeId}/update-position`, {
    tagName: rootState.tag.currentTag.name,
    position: payload.position
  }
  )
  return result.data
}
, 500)

export async function updateNodePosition ({ state, rootState }, payload) {
  updateNodePositionDeb(state, rootState, payload)
}

export async function connect ({ state }, params) {
  const port = params.port
  const result = await axios.put(
    `${baseApiUrl}/realtime/connect/`, { port: port }
  )
  return result
}

export async function disconnect ({ state }) {
  const result = await axios.post(
    `${baseApiUrl}/realtime/disconnect`
  )
  return result
}

export async function isConectionActive ({ state }) {
  const result = await axios.get(
    `${baseApiUrl}/realtime/isConectionActive`
  )
  return result.data
}

export async function connectionStablished ({ state }) {
  const result = await axios.get(
    `${baseApiUrl}/realtime/connectionStablished`
  )
  return result.data
}

export async function isActiveServer ({ state }) {
  const result = await axios.get(
    `${baseApiUrl}/realtime/isServerActive`
  )
  return result.data
}

export async function getIngameObjectList ({ state }) {
  const result = await axios.get(
    `${baseApiUrl}/realtime/ingame-object-list`
  )
  return result.data
}

export async function getPlayingTagsList ({ state }) {
  const result = await axios.get(
    `${baseApiUrl}/realtime/playing-tags-list`
  )
  return result.data
}

export async function getVersionNumber ({ state, commit }) {
  if (!state.versionNumber) {
    const result = await axios.get(
      `${baseApiUrl}/version`)
    commit('setVersionNumber', `v${result.data.version}`)
  }
  return state.versionNumber
}

export async function relaunch ({ state, commit }, path) {
  await axios.post(
      `${baseApiUrl}/relaunch`, {
        path: path
      })
}
