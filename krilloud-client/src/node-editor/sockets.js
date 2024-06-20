import { Socket } from 'rete'

const variable = new Socket('variable')
const audio = new Socket('audio')

/* variable.combineWith(audio)
audio.combineWith(variable) */

export { variable, audio }
