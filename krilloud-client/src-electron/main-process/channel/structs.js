var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const KrilloudChannelDLL = struct({
  channel_id: ref.types.int32,
  channel_name: 'string',
  parent_id: ref.types.int32,
  volume: ref.types.float
})

module.exports.KrilloudChannelDLL = KrilloudChannelDLL
