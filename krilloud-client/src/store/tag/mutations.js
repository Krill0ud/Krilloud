export const addNewTag = (state, tag) => {
  state.tags.push(tag)
}

export const deleteTag = (state, tag) => {
  const index = state.tags.findIndex((item) => item.name === tag)
  state.tags.splice(index, 1)
}

export const setCurrentTag = (state, tag) => {
  state.currentTag = tag
}

export const setTagInfo = (state, tag) => {
  const index = state.tags.indexOf(tag)
  state.tags[index] = tag
}

export const setTags = (state, tags) => {
  state.tags = tags
}

export const setIsPlaying = (state, v) => {
  state.currentTag.isPlaying = v
}

export const setIsLoadingTag = (state, v) => {
  state.isLoadingTag = v
}
