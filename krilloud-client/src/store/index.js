import Vue from 'vue'
import Vuex from 'vuex'

import main from './main'
import rete from './rete'
import project from './project'
import variable from './variable'
import tag from './tag'
import channel from './channel'
import audioPool from './audioPool'

Vue.use(Vuex)

/*
 * If not building with SSR mode, you can
 * directly export the Store instantiation;
 *
 * The function below can be async too; either use
 * async/await or return a Promise which resolves
 * with the Store instance.
 */

export default function (/* { ssrContext } */) {
  const Store = new Vuex.Store({
    modules: {
      main,
      rete,
      project,
      variable,
      tag,
      channel,
      audioPool
    }

    // enable strict mode (adds overhead!)
    // for dev mode only
    // strict: process.env.DEBUGGING
  })

  return Store
}
