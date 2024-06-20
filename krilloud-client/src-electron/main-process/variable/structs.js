var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const VarType = ['Global', 'Local']
const VarSourceType = ['Random_Repetition', 'Random_No_Repetition', 'InGame']
const VarContentType = ['Ranged', 'Raw']

const KrilloudVariable = struct({
  variable_name: ref.types.CString,
  current_value: ref.types.float,
  type: ref.types.int,
  source: ref.types.int,
  content: ref.types.int,
  range_min: ref.types.float,
  range_max: ref.types.float,
  set_only_on_play: ref.types.bool,
  previous_name: ref.types.CString

})

module.exports.VarType = VarType
module.exports.VarSourceType = VarSourceType
module.exports.VarContentType = VarContentType
module.exports.KrilloudVariable = KrilloudVariable
