(function (window) {
  var krilloudSDK = {};
  var globalPath;
  var relativePath;
  var isMainLoopInit;
	var globalReturn;

  class Queue {
    constructor() {
      this.runningExec = false;
      this.queueList = [];
      this.isEmpty = null;
    }

    isRunnigExec() {
      return this.runningExec;
    }

    addRequest(request) {
      this.queueList.push(request);
      return this.queueList;
    }

    async executeList() {
      this.runningExec = true;
      var firstElement = this.queueList.shift();
      const unboundAction = firstElement.action;
      const boundAction = unboundAction.bind(firstElement);

      let bound = await boundAction();

      if (this.queueList.length > 0) {
        this.executeList();
      } else {
        this.runningExec = false;
      }
    }
  }

  var queue = new Queue();

  krilloudSDK.status = function () {
    console.log("krilloudSDK enabled")
  };


  krilloudSDK.start = async function (contractPath, soundBankPath) {
    globalPath = contractPath;
    relativePath = soundBankPath;
    await new Promise((resolve, reject) => {
      let id = setTimeout(() => {
        clearTimeout(id);
        resolve('Timed out in '+ 2000 + 'ms.')
      }, 2000)
    })
    return new Promise((resolve, reject) => {
      js_wrapped_EX_Krilloud_Create(
        globalPath,
        relativePath
      ).then((result) => {
        js_wrapped_EX_Krilloud_Init(1, 0, 0, 0, 2).then((result) => {
          resolve(0);
        });
      });
    });
  };

  krilloudSDK.load = async function (tagsToLoad) {
    const module = {
      tags: tagsToLoad,
      relativePathWeb: relativePath,
      action: function () {
        return new Promise(async (resolve, reject) => {
          for (var i = 0; i < this.tags.length; i++) {
            let tagToLoad = this.tags[i];
            await js_wrapped_EX_Krilloud_Load(
              tagToLoad.names,
              tagToLoad.names.split(",").length,
              tagToLoad.objectId,
              this.relativePathWeb
            );
          }
          await mainLoop();
          resolve(0);
        });
      },
    };
    manageRequest(module);
  };

  krilloudSDK.play = function (tagName, objectId) {
    const module = {
      tagName: tagName,
      objectId: objectId,
      action: function () {
        return js_wrapped_EX_Krilloud_Play(this.tagName, this.objectId);
      },
    };
    manageRequest(module);
  };

  krilloudSDK.stopTag = async function (tagName, objectId) {
    const module = {
      tagName: tagName,
      objectId: objectId,
      action: function () {
        return js_wrapped_EX_Krilloud_StopTag(this.tagName, this.objectId);
      },
    };
    manageRequest(module);
  };

  krilloudSDK.updateListenerPosition = async function (posX, posY, posZ, atX, atY, atZ, upX, upY, upZ, velX, velY, velZ) {
    const module = {
      posX: posX,
      posY: posY,
      posZ: posZ,
      atX: atX,
      atY: atY,
      atZ: atZ,
      upX: upX,
      upY: upY,
      upZ: upZ,
      velX: velX,
      velY: velY,
      velZ: velZ,
      action: function () {
        return js_wrapped_EX_Krilloud_UpdateListenerPosition(this.posX, this.posY, this.posZ, this.atX, this.atY, this.atZ, this.upX, this.upY, this.upZ, this.velX, this.velY, this.velZ);
      },
    };
    manageRequest(module);
  };

  krilloudSDK.updateObjectPosition = async function (objectId, posX, posY, posZ, velX, velY, velZ) {
    const module = {
      objectId: objectId,
      posX: posX,
      posY: posY,
      posZ: posZ,
      velX: velX,
      velY: velY,
      velZ: velZ,
      action: function () {
        return js_wrapped_EX_Krilloud_UpdateObjectPosition(this.objectId, this.posX, this.posY, this.posZ, this.velX, this.velY, this.velZ);
      },
    };
    manageRequest(module);
  };

  krilloudSDK.setVar = function (tagVar, objectId, value) {
    const module = {
      tagVar: tagVar,
      objectId: objectId,
      value: value,
      action: function () {
        if (Number.isInteger(value)) {
          return js_wrapped_EX_Krilloud_UpdateKVar_INT(
            this.tagVar,
            this.objectId,
            this.value
          );
        } else {
          return js_wrapped_EX_Krilloud_UpdateKVar_FLOAT(
            this.tagVar,
            this.objectId,
            this.value
          );
        }
      },
    };
    manageRequest(module);
  };

	krilloudSDK.getChannelVolume = function (callbackResult, channelId ) {
    const module = {
			callbackResult: callbackResult,
			channelId: channelId,
      action: function () {
				return new Promise( async (resolve, reject) => {
					const result = await js_wrapped_EX_GetChannelVolume(channelId)
					if(callbackResult) {
						callbackResult(result)
					}
					resolve(result)
				});
        }
    };
    manageRequest(module);
  };

	krilloudSDK.getchannelVolumeByName = function (channelName) {
    const module = {
			callbackResult: callbackResult,
			channelName: channelName,
      action: function () {
				return new Promise( async (resolve, reject) => {
					const result = await js_wrapped_EX_GetChannelVolumeByName(channelName)
					if(callbackResult) {
						callbackResult(result)
					}
					resolve(result)
				});
        }
    };
    manageRequest(module);
  };

	krilloudSDK.changeChannelVolume = function (channelId, volume) {
    const module = {
			channelId: channelId,
			volume: volume,
      action: function () {
          return js_wrapped_EX_ChangeChannelVolume(channelId, volume);
        }
    };
    manageRequest(module);
  };

	krilloudSDK.changeChannelVolumeByName = function (channelName, volume) {
    const module = {
			channelName: channelName,
			volume: volume,
      action: function () {
          return js_wrapped_EX_ChangeChannelVolumeByName(channelName, volume);
        }
    };
    manageRequest(module);
  };


  async function manageRequest(request) {
    queue.addRequest(request);
    if (!queue.isRunnigExec()) {
      queue.executeList();
    }
  }

  async function mainLoop() {
    if (!isMainLoopInit) {
      await js_wrapped_EX_Krilloud_MainLoop();
      isMainLoopInit = true;
    }
  }

  async function cancelMainLoop() {
    if (isMainLoopInit) {
      await js_wrapped_EX_Krill_Cancel_MainLoop();
      isMainLoopInit = false;
    }
  }
  window.krilloudSDK = krilloudSDK;
})(window, undefined);
