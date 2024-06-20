var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const KLTag = struct({
  tag_name: 'string',
  is3D: ref.types.bool,
  min_3D_distance: ref.types.float,
  max_3D_distance: ref.types.float,
  roll_off_factor: ref.types.float,
  attenuation: ref.types.int,
  distance_delay: ref.types.bool,
  doppler_active: ref.types.bool,
  doppler_factor: ref.types.float,

  channel_id: ref.types.int,

  isLoopable: ref.types.bool
})

module.exports.KLTag = KLTag
