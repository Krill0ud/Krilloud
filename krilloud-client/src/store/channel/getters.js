
export function channelInitialData (state) {
  return {
    id: null,
    name: 'Channel ' + (state.channels.length),
    volume: 0.5,
    label: 'Channel ' + (state.channels.length),
    value: state.channels.length
  }
}
