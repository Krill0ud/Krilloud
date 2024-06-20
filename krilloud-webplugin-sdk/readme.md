# Sdk Krilloud web plugin

_Sdk to implement the integration of krilloud web plugin_

## Installation 🚀

The next files are required to use the plugin: 

1. Files provided by the plugin:

    - **Krilloud.js**: Core Krilloud functions.
    - **krilloud.wasm**: Webassembly output file

2. Files generated by the krilloud application exporting a project. For more information and to download the tool, visit [Krilloud.com](https://krilloud.com/):
    - **Contract.json**: json exported by krilloud
    - **.krll**: sound bank file exported by krilloud

### First step

First of all to start with the implementation, add the next two scripts:
```
<script src="krilloud.js"></script>
<script src="sdk.js"></script>
```
### Sdk functions 

| Funtions      | Parameters                    |
| ------------- | ------------------------------ |
| `krilloudSDK.status()`      | Check connection and availability of sdk functions      |
| `krilloudSDK.start(contractPath, soundBankPath)`   | File path [contractPath](#referencias) and [soundBankPath](#referencias) |
| `krilloudSDK.load(tagsToLoad)`   | tagsToLoad: [Array of tags object](#referencias)   |
| `krilloudSDK.play(tagName, objectId)`   | tagName: tag name,   [ObjectId ](#referencias): object associated with that tag |
|` krilloudSDK.stopTag(tagName, objectId) `  | tagName: tag name objectId: object associated with that tag    |
| `krilloudSDK.setVar(tagVar, objectId, value)`   | [tagVar ](#referencias): variable name, objectId: object associated with that variable , value: value to set  |
| `krilloudSDK.updateListenerPosition(posX, posY, posZ, atX, atY, atZ, upX, upY, upZ, velX, velY, velZ)`   | [position references ](#referencias): update listener position  |
| `krilloudSDK.updateObjectPosition(objectId, posX, posY, posZ, velX, velY, velZ`   | [position references ](#referencias): update object position, objectId: object associated with tag  |



### References

| Reference | Description |
| ------  | ------ |
|  contractPath | **string**, "pathTo/contract.json" relative path of contract.json in the project, taking as reference the path of krilloud.js|
|  soundBankPath | **string**,  "pathTo/file.krll" relative path of .krll, taking as reference the path of  krilloud.js |
|  objectId | **number**, id of the game object bind with that tag  |
|  tagsToLoad | **array**, tagsToLoad = [ {names: "tag1,tag2...", objectId: 1}, {names: "tag3, tag4", objectId: 2}]  names is a string with the names of the tags separated by commas (,) this name must be exactly the same as the one exported from the krilloud project.|
|  tagVar | **string**,  variable name |
|  position references: posX, posY, posZ | **number**,  vector position x, y, z (listener or object)  |
| position references: atX, atY, atZ | **number**,  vector look at  |
| position references: upX, upY, upZ | **number**,  vector look up  |
| position references: velX, velY, velZ | **number**, vector speed  |


### Use 📋
First of all, to initialized the sdk:
```
await krilloudSDK.start(contractPath, soundBankPath);
```
After that, you can load the tags that will be used in the project:
```
// Load tags
    var tagsToLoad = [
        { names: "musica,ui", objectId: canvasObjectSound },
        { names: "step,pasos,ui", objectId: playerObjectSound },
        { names: "musicaBugs,bugs", objectId: enemyObjectSound },
    ];
    await krilloudSDK.load(tagsToLoad);
```


Once the tags are loaded, the functions available in the SDK can be executed, such as play and stop or set a specific variable value.

```
// Play tag "musica"
    let canvasObjectSound = 0
    krilloudSDK.play("musica", canvasObjectSound);
```

```
// Set variable - music_pitch
    let speedMusic = 0.5
    krilloudSDK.setVar("music_pitch", canvasObjectSound, speedMusic);
```

### 3D audio implementation 📋
Krilloud provides two functions to implement audio 3D (only to tags set with 3D option). 
```
krilloudSDK.updateObjectPosition(1, 0, 0, 1, 0, 0, 0)
```
```
krilloudSDK.updateListenerPosition(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
```

### Demos
 - [Arcade-game](https://krillaudio.com/demos/v2/arcade-game): js demo 
 - [Slot-game](https://krillaudio.com/demos/v2/slot-game): pixy.js - typescript demo
 - [Three.js-game](https://krilloud.com/demos/threejs/examples/webaudio_sandbox.html): three.js - demo

### Compatibility

- Chrome
- Firefox
- iOS Safari
- IE / Edge
- Opera

### TODOS
- Auto-importing krilloud.js file from a cdn
- Publish npm
- Versioning

## License
**SoLoud audio engine**
Copyright (c) 2013-2020 Jari Komppa

**Krilloud**
Interactive Audio, Node-programming, audio middleware, The fastest tool for Audio Implementation.