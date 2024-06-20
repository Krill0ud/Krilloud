// import Vue from 'vue'
export const addNewVariable = (state, variable) => {
  state.variables.push(variable)
  // Vue.set(state.variables, state.variables.length, variable)
}

export const editVariable = (state, variable) => {
  const index = state.variables.findIndex((item) => item.id === (variable.id))
  state.variables.splice(index, 1, variable)
}

export const deleteVariable = (state, variable) => {
  const index = state.variables.findIndex((item) => item.id === (variable.id))
  state.variables.splice(index, 1)
}

export const setCurrentVariable = (state, variable) => {
  state.currentVariable = variable
}

export const setVariables = (state, variables) => {
  state.variables = variables
}
