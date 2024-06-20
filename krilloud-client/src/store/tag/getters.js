
export function initialData (state) {
  return {
    id: state.tags.length + 1,
    name: 'Tag ' + (state.tags.length + 1),
    audio3D: false,
    attenuation: 'No attenuation',
    channel: 'Master',
    minDistance: 0.000,
    maxDistance: 0.000,
    rolloff: 0.000,
    isdoppler: false,
    doppler: 0.000,
    delay: false,
    loop: false,
    isPlaying: false
  }
}
