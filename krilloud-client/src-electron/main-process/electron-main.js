import { app, BrowserWindow, nativeTheme } from 'electron'
const nativeImage = require('electron').nativeImage
const path = require('path')
const http = require('http')
const express = require('express')
const bodyParser = require('body-parser')
const expressApp = express()
expressApp.use(bodyParser.json())
const cors = require('cors')
const router = express.Router()

let errorMessage
try {
  const dllLib = require('./apiDLLLib').dllLib
  console.log('dll try', dllLib)
} catch (e) {
  console.log('dll error', e)
  errorMessage = 1
}
const projectRoutes = require('./project')
const audioPoolRoutes = require('./audiopool')
const channelRoutes = require('./channel')
const nodeRoutes = require('./node')
const tagRoutes = require('./tag')
const variableRoutes = require('./variable')
const realTimeRoutes = require('./realtime')

const libProject = require('./project/apiJSONLib').lib

var morgan = require('morgan')
morgan.token('body', (req, res) => JSON.stringify(req.body))
expressApp.use(morgan(':method :url :status :response-time ms - :res[content-length] :body - :req[content-length]'))
const appVersion = require('../../package.json').version

// get the icon path as per installed app
const iconPath = path.join(__dirname, '/../icons/Krilloud_icon.png')
// create the appIcon
const appIcon = nativeImage.createFromPath(iconPath)
// appIcon.setTemplateImage(true)

try {
  if (process.platform === 'win32' && nativeTheme.shouldUseDarkColors === true) {
    require('fs').unlinkSync(require('path').join(app.getPath('userData'), 'DevTools Extensions'))
  }
} catch (_) { }

/**
 * Set `__statics` path to static files in production;
 * The reason we are setting it here is that the path needs to be evaluated at runtime
 */
if (process.env.PROD) {
  global.__statics = __dirname
}

let mainWindow
async function createWindow () {
  /**
   * Initial window options
   */
  if (errorMessage === 1) {
    mainWindow = new BrowserWindow({
      width: 1000,
      height: 600,
      useContentSize: true,
      webPreferences: {
        nodeIntegration: process.env.QUASAR_NODE_INTEGRATION,
        nodeIntegrationInWorker: process.env.QUASAR_NODE_INTEGRATION
      },
      icon: appIcon
    })
    mainWindow.setMenuBarVisibility(false)
    // mainWindow.hide()
    await require('electron').dialog.showMessageBox(mainWindow,
      {
        type: 'error',
        title: 'Error in main process',
        message: 'Internal error'
      })
    app.quit()
  } else {
    mainWindow = new BrowserWindow({
      width: 1000,
      height: 600,
      useContentSize: true,
      webPreferences: {
        // Change from /quasar.conf.js > electron > nodeIntegration;
        // More info: https://quasar.dev/quasar-cli/developing-electron-apps/node-integration
        nodeIntegration: process.env.QUASAR_NODE_INTEGRATION,
        nodeIntegrationInWorker: process.env.QUASAR_NODE_INTEGRATION
        // devTools: process.env.PROD
        // More info: /quasar-cli/developing-electron-apps/electron-preload-script
        // preload: path.resolve(__dirname, 'electron-preload.js')
      },
      icon: appIcon
    })

    mainWindow.setMenuBarVisibility(false)
    mainWindow.loadURL(`${process.env.APP_URL}`)

    mainWindow.on('closed', () => {
      // mainWindow = null
    })

    mainWindow.on('closed', async function (e) {
      const count = BrowserWindow.getAllWindows()
        .filter(b => {
          return b.isVisible()
        })
        .length

      if (count === 0) {
        if (libProject.KHasOpenedProject()) {
          var choice = await require('electron').dialog.showMessageBox(this,
            {
              type: 'question',
              buttons: ['Yes', 'No'],
              title: 'Confirm',
              message: 'Do you want to save before close?'
            })
          if (choice.response === 0) {
            await libProject.KSaveProject()

            console.log('project saved')
          }
          await libProject.KCloseProject()
        }
        if (process.platform !== 'darwin') {
          app.quit()
        }
      }
    })
  }
  return mainWindow
}

app.on('ready', createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    // app.quit()
  }
})

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow()
  }
})

expressApp.use(cors())

expressApp.use('/projects', projectRoutes)
expressApp.use('/audio-pools', audioPoolRoutes)
expressApp.use('/channels', channelRoutes)
expressApp.use('/nodes', nodeRoutes)
expressApp.use('/tags', tagRoutes)
expressApp.use('/variables', variableRoutes)
expressApp.use('/realtime', realTimeRoutes)

expressApp.use('/', router)

// mainWindow es limpiada al hacer destroy por el listener on close linea 68
// tengo que mantener siempre una viva porque sino el proces muere por evento linea 77
function relaunch () {
  let oldWindow = null
  let newWindow = null
  if (mainWindow) {
    oldWindow = mainWindow
  }
  newWindow = createWindow()
  if (oldWindow) {
    oldWindow.destroy()
    oldWindow.quit()
    mainWindow = newWindow
  }
}
router.post('/relaunch', function (req, res) {
  relaunch()
  res.send({ result: 'OK' })
  })

router.get('/version', function (req, res) {
  res.send({ version: appVersion })
})

http.createServer(expressApp).listen(8080, () => {
  console.log('Server listening at port 8080')
})
