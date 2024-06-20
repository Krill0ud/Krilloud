import Color from 'color'

export function assignMaterialColor (material, color) {
  const c = material.color

  try {
    const [r, g, b] = Color(color).rgb().color

    c.r = r / 255
    c.g = g / 255
    c.b = b / 255
  } catch (e) {
    c.r = c.g = c.b = 0
  }
}

export function assignMaterialGlossiness (material, color) {
  material.roughness = 1 - Color(color).luminosity()
}
