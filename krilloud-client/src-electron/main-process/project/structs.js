var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const KLInfoDLL = struct({
  first: ref.types.bool,
  second: 'string'
})

module.exports.KLInfoDLL = KLInfoDLL
