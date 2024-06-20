const dllLib = require('../apiDLLLib').dllLib
const tagLib = require('../tag/apiJSONLib').lib

const KrilloudVariable = require('./structs').KrilloudVariable

const exportLib = {}

exportLib.KGetNodeAssociatedKVar = (tagName, nodeId) => {
  var op = new KrilloudVariable()
  op = dllLib.GetNodeAssociatedKVar(tagName, nodeId)
  var json = {
    name: op.variable_name,
    simulated: op.current_value ? parseFloat(op.current_value.toFixed(3)) : 0,
    minRange: op.range_min,
    maxRange: op.range_max,
    onPlayOnly: op.set_on_play_only,

    varType: op.type,
    varSourceType: op.source,
    varContentType: op.content,

    icon: 'edit',
    header: 'generic',
    onlyIntegers: false
  }
  return json
}

exportLib.KGetVariable = (name) => {
  const varId = dllLib.GetVariablePosition(name)
  var op = new KrilloudVariable()
  op = dllLib.GetKVariable(name)
  var json = {
    name: op.variable_name || op.previous_name,
    id: varId,
    simulated: op.current_value.toFixed(3) ? parseFloat(op.current_value.toFixed(3)) : 0,
    minRange: op.range_min.toFixed(3),
    maxRange: op.range_max.toFixed(3),

    varType: op.type,
    varSourceType: op.source,
    varContentType: op.content,

    icon: 'edit',
    header: 'generic',
    onlyIntegers: false,
    onPlayOnly: op.set_on_play_only || false
  }
  // console.log('pasa por aquí', json)
  return json
}

exportLib.KGetVariableNameList = () => {
  const count = dllLib.GetKVarCount()
  const result = []
  for (let x = 0; x < count; x++) {
    result.push(dllLib.GetVariableName(x))
  }
  return result
}

exportLib.KCreateNewVariable = (newVar) => {
  dllLib.CreateNewVariable(newVar.name)
  dllLib.SetVariableCurrentValue(newVar.name, newVar.simulated)
  dllLib.SetVariableType(newVar.name, newVar.varType)
  dllLib.SetVariableSource(newVar.name, newVar.varSourceType)
  dllLib.SetVariableContent(newVar.name, newVar.varContentType)
  dllLib.SetVariableRangeMin(newVar.name, newVar.minRange)
  dllLib.SetVariableRangeMax(newVar.name, newVar.maxRange)
  dllLib.SetVariableOnPlay(newVar.name, newVar.onPlayOnly)
}

async function updateVariables (mode) {
  const varList = []
  const taglistDeletedVar = []
  const count = dllLib.GetKVarCount()
  for (let x = 0; x < count; x++) {
    const varName = dllLib.GetVariableName(x)
    const op = dllLib.GetKVariable(varName)
    const varData = {
      actualName: op.variable_name,
      previousName: op.previous_name
    }
    varList.push(varData)
  }

  const tagList = tagLib.KGetTagList()

  tagList.forEach(async tag => {
    try {
      const nodeLib = require('../node/apiJSONLib').lib
      const list = await nodeLib.KGetNodesIDList(tag)
      list.forEach(async nodeId => {
        const nodeData = nodeLib.KGetNodeType(tag, nodeId)
        if (nodeData.type === 1) {
          if (mode === 'rename') {
            renameAction(nodeId, nodeData, tag, varList)
          } else {
            const tagnameDeleted = deleteAction(nodeId, nodeData, tag, varList)
            const tagExist = taglistDeletedVar.findIndex(item => item === tagnameDeleted)
            if (tagExist === -1) {
              taglistDeletedVar.push(tagnameDeleted)
            }
          }
        }
      })
    } catch (error) {
      console.log(error)
      return error
    }
  })
  return taglistDeletedVar
}
function deleteAction (nodeId, node, tag, varList) {
  const varExist = varList.findIndex(itemVar => itemVar.actualName === node.name || itemVar.previousName === node.name)
  if (varExist === -1) {
    const nodeLib = require('../node/apiJSONLib').lib
    nodeLib.KRemoveNode(nodeId, tag)
    return tag
  }
}

function renameAction (nodeId, nodeData, tag, varList) {
  let varAssoc = new KrilloudVariable()
  varAssoc = dllLib.GetNodeAssociatedKVar(tag, nodeId)

  if (varAssoc.variable_name !== nodeData.name) {
    const varToReference = varList.findIndex(itemVar => itemVar.actualName === nodeData.name || itemVar.previousName === nodeData.name)
    dllLib.SetVariableSelection(tag, nodeId, varToReference)
  }
}

exportLib.KSaveVariable = (newVar) => {
  const oldNameVariable = dllLib.GetVariableName(newVar.index)
  dllLib.SetVariableName(oldNameVariable, newVar.name)
  dllLib.SetVariableCurrentValue(newVar.name, newVar.simulated)
  dllLib.SetVariableType(newVar.name, newVar.varType)
  dllLib.SetVariableSource(newVar.name, newVar.varSourceType)
  dllLib.SetVariableContent(newVar.name, newVar.varContentType)
  dllLib.SetVariableRangeMin(newVar.name, newVar.minRange)
  dllLib.SetVariableRangeMax(newVar.name, newVar.maxRange)
  dllLib.SetVariableOnPlay(newVar.name, newVar.onPlayOnly)
  const mode = 'rename'
  updateVariables(mode)
}

exportLib.KRemoveVar = async (name) => {
  const position = dllLib.GetVariablePosition(name)
  dllLib.RemoveVariable(position)
  const mode = ''
  const result = await updateVariables(mode)
  return result
}

exportLib.KSetVariableName = (varName, value) => {
  dllLib.SetVariableName(varName, value)
}

exportLib.KSetVariableCurrentValue = (varName, value) => {
  dllLib.SetVariableCurrentValue(varName, value)
}

exportLib.KSetVariableType = (varName, value) => {
  dllLib.SetVariableType(varName, value)
}

exportLib.KSetVariableSource = (varName, value) => {
  dllLib.SetVariableSource(varName, value)
}

exportLib.KSetVariableContent = (varName, value) => {
  dllLib.SetVariableContent(varName, value)
}

exportLib.KSetVariableRangeMin = (varName, value) => {
  dllLib.SetVariableRangeMin(varName, value)
}

exportLib.KSetVariableRangeMax = (varName, value) => {
  dllLib.SetVariableRangeMax(varName, value)
}

exportLib.KSetVariableOnPlay = (varName, value) => {
  dllLib.SetVariableOnPlay(varName, value)
}

module.exports.lib = exportLib
