export default function () {
  return {
    tags: [
      {
        id: 1,
        name: 'Tag 1',
        audio3D: true,
        attenuation: 'No attenuation',
        channel: 'Master',
        minDistance: 0.000,
        maxDistance: 0.000,
        rolloff: 0.000,
        isdoppler: false,
        doppler: 0.00,
        delay: false,
        loop: true,
        isPlaying: true,
        retigger: true
      },
      {
        id: 2,
        name: 'Tag 2',
        audio3D: true,
        attenuation: 'No attenuation',
        channel: 'Master',
        minDistance: 0.000,
        maxDistance: 0.000,
        rolloff: 0.000,
        isdoppler: false,
        doppler: 0.00,
        delay: false,
        loop: true,
        isPlaying: true,
        retigger: true
      },
      {
        id: 3,
        name: 'Tag 3',
        audio3D: false,
        attenuation: 'No attenuation',
        channel: 'Master',
        minDistance: 0.000,
        maxDistance: 0.000,
        rolloff: 0.000,
        isdoppler: false,
        doppler: 0.00,
        delay: false,
        loop: true,
        isPlaying: true,
        retigger: true
      }
    ],
    currentTag: null,
    isLoadingTag: false
  }
}
