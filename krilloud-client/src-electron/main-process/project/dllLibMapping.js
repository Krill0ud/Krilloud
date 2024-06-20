
const KLInfoDLL = require('./structs').KLInfoDLL
var ref = require('ref-napi')

const libMap = {
  OpenProject: [
    KLInfoDLL, ['string']
  ],
  NewProject: [
    KLInfoDLL, ['string', 'string', 'string']
  ],
  CloseCurrentProject: [
    ref.types.void, []
  ],
  SaveCurrentProject: [
    ref.types.void, []
  ],
  ExportCurrentProject: [
    ref.types.void, [ref.types.bool]
  ],
  GetProjectPath: [
    ref.types.CString, []
  ],
  GetGamePath: [
    ref.types.CString, []
  ],
  GetProjectName: [
    ref.types.CString, []
  ],
  HasOpenedProject: [
    ref.types.bool, []
  ],
  SetNewProjectPath: [
    ref.types.bool, ['string']
  ],
  SetNewGamePath: [
    ref.types.bool, ['string']
  ],
  SetNewProjectName: [
    ref.types.bool, ['string']
  ],
  SaveCurrentProjectAs: [
    ref.types.bool, ['string']
  ]
}

module.exports.dllLibFunctions = libMap
