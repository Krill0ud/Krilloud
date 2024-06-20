var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const KrilloudIngameObject = struct({
  object_id: ref.types.int,
  position_x: ref.types.float,
  position_y: ref.types.float,
  position_z: ref.types.float,
  aVelX: ref.types.float,
  aVelY: ref.types.float,
  aVelZ: ref.types.float

})

const KrilloudPlayingTag = struct({
  id: ref.types.size_t,
  parent_object_id: ref.types.int,
  tagname: ref.types.CString,
  playHandle: ref.types.float,
  channel_id: ref.types.int,
  is3D: ref.types.bool,
  stop: ref.types.bool
})

module.exports.KrilloudPlayingTag = KrilloudPlayingTag
module.exports.KrilloudIngameObject = KrilloudIngameObject
