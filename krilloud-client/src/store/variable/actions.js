const baseApiUrl = `${process.env.API_URL}/variables`
import axios from 'axios'
import { Notify } from 'quasar'

export async function saveVariable ({ dispatch, commit, rootGetters }, variable) {
  const result = await axios.post(
    `${baseApiUrl}/create`, variable
  )
  console.log(result)

  commit('addNewVariable', variable)
}

export async function editVariable ({ dispatch, commit, rootGetters }, variable) {
  const result = await axios.post(
    `${baseApiUrl}/${variable.name}/save`, variable
  )
  console.log(result)
}

export async function init ({ commit, getters, rootGetters, state }) {
  const variables = await axios.get(
    `${baseApiUrl}/`
  )
  commit('setVariables', variables.data)
}

export async function close ({ commit }) {
  console.log('cleaning variables')
  commit('setVariables', [])
}

export async function deleteVariable ({ dispatch, commit, getters, state }, variable) {
  const result = await axios.delete(
    `${baseApiUrl}/${variable.name}`)
  console.log(result)
  commit('deleteVariable', variable)
  if (result.data.length > 0) {
    Notify.create({
      type: 'positive',
      message: 'Variable was removed successfully. Nodes referencing to this variable have been removed in tags:',
      caption: `${result.data}`
    })
  }
}

export async function setAttribute ({ state }, params) {
  console.log(params)
  const name = params.name
  const value = params.value
  const attribute = params.attribute
  const data = { value: value }
  const result = await axios.post(
    `${baseApiUrl}/${name}/${attribute}`, data)
  return result
}
