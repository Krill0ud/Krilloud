export default function () {
  return {
    temporalElementDrag: {},
    realtimeObjectList: [],
    realtimeTagsList: [],
    realTimeState: false,
    versionNumber: null,
    isModal: false,
    modeEditing: false,
    showDeleteElementModal: false,
    varTypes: [{ label: 'Global', value: 0 }, { label: 'Local', value: 1 }
    ],
    varContentTypes: [{ label: 'Ranged', value: 0 }, { label: 'Raw', value: 1 }],
    varSourceTypes: [{ label: 'Random', value: 0 }, { label: 'Random no repetition', value: 1 }, { label: 'In Game', value: 2 }],
    attenuationOptions: [{ label: 'No attenuation', value: 0 }, { label: 'Inverse distance', value: 1 }, { label: 'Linear distance', value: 2 }, { label: 'Exponential distance', value: 3 }],
    playOrderOptions: [
      {
        label: 'Random',
        value: 0
      },
      {
        label: 'Sequential',
        value: 1
      }
    ]

  }
}
