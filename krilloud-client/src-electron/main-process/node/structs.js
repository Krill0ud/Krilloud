var ref = require('ref-napi')
const struct = require('ref-struct-di')(ref)

const KrilloudNodeConnection = struct({
  my_socket_id: ref.types.int,
  connected_node_id: ref.types.int,
  connected_node_socket_id: ref.types.int
})

const KrilloudNodeSocket = struct({
  socket_id: ref.types.int,
  conection_type: ref.types.int,
  data_type: ref.types.int,
  links_array_length: ref.types.int,
  links: ref.refType(KrilloudNodeConnection)
})

const KrilloudNode = struct({
  type: ref.types.int,
  id: ref.types.int,
  name: ref.types.CString,
  pos_x: ref.types.float,
  pos_y: ref.types.float,
  size_x: ref.types.float,
  size_y: ref.types.float,
  can_be_delted: ref.types.bool,
  inputs_count: ref.types.int,
  outputs_count: ref.types.int,

  sockets_list_length: ref.types.int,
  sockets: ref.refType(KrilloudNodeSocket)
})

const FilterParameter = struct(
  {
    name: ref.types.CString,
    value: ref.types.float,
    min_value: ref.types.float,
    max_value: ref.types.float
  }
)

const KLPair = struct({
  first: ref.types.float,
  second: ref.types.float
})

module.exports.KrilloudNode = KrilloudNode
module.exports.KLPair = KLPair
module.exports.FilterParameter = FilterParameter
