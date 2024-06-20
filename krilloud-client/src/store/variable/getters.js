
export function initialData (state) {
  let id = 1
  if (state.variables.length > 0) {
    const variable = state.variables.reduce((p, c) => p.id > c.id ? p : c)
    id = variable.id + 1
  }
  const result = {
    id: id,
    name: 'Variable ' + id,
    icon: 'edit',
    header: 'generic',
    simulated: 0,
    varType: 0,
    varContentType: 1,
    varSourceType: 2,
    minRange: 0,
    maxRange: 1,
    onPlayOnly: false,
    onlyIntegers: false
  }
  return result
}
