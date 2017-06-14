/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function PluginInstanceManager() { }

/**
 * The system category string
 * @type {string}
 * @private
 */
PluginInstanceManager._systemCategory = "system";

/**
 * Create a plugin instance object from server JSON
 * @param json The json data provided by server
 * @returns {PluginInstance} The created instance
 */
PluginInstanceManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.type), "json.type must be defined");
    assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");
    assert(!isNullOrUndefined(json.autoStart), "json.autoStart must be defined");
    assert(!isNullOrUndefined(json.category), "json.category of a pluginInstance must be defined");

    return new PluginInstance(json.id, json.displayName, json.type, json.configuration, json.autoStart, json.category, json.deviceConfiguration);
};

/**
 * Tells if a plugin instance is system category
 * @returns {boolean}
 */
PluginInstanceManager.isSystemCategory = function (pluginInstance) {
    return pluginInstance.category.toLowerCase() === PluginInstanceManager._systemCategory.toLowerCase();
};

/**
 * Get a plugin instance using its ID
 * @param pluginInstanceId The plugin instance id to get
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.get = function (pluginInstanceId) {
    assert(!isNullOrUndefined(pluginInstanceId), "pluginInstanceId must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("rest/plugin/" + pluginInstanceId)
        .done(function (data) {
            if (!isNullOrUndefinedOrEmpty(data))
                d.resolve(PluginInstanceManager.factory(data));
            else
                d.reject($.t("objects.generic.errorGetting", { objectName: "Plugin with Id = " + pluginInstanceId }));
        })
    .fail(function (errorMessage) {
        notifyError($.t("objects.generic.errorGetting", { objectName: "Plugin with Id = " + pluginInstanceId }), errorMessage);
        d.reject(errorMessage);
    });
    return d.promise();
};

/**
 * Function which list all available plugins instance
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.getAll = function () {
    var d = new $.Deferred();

    RestEngine.getJson("rest/plugin/instance")
       .done(function (data) {
           var result = [];
           var arrayOfDeffered = [];
           data.plugin.forEach(function (value) {
               try {
                   var pi = PluginInstanceManager.factory(value);
                   var deffered = PluginInstanceManager.downloadPackage(pi);
                   arrayOfDeffered.push(deffered);
                   deffered.done(function () {
                       result.push(pi);
                   }).fail(function () {
                       pi.package = {
                           name: value.displayName,
                           failToLoad: true
                       };
                       result.push(pi);
                       console.warn("fail to get plugin " + value.displayName);
                   });

               } catch (ex) {
                   notifyWarning($.t("objects.generic.errorLoading", { objectName: "plugin " + value }), JSON.stringify(ex));
               }

           });

           $.whenAll(arrayOfDeffered).done(function () {
               d.resolve(result);
           });
       })
       .fail(function (internalErrorMessage) {
           var errorMessage = $.t("objects.generic.errorLoading", { objectName: "plugin instances" }) + " Inner error :" + internalErrorMessage;
           notifyError(errorMessage);
           d.reject(errorMessage);
       });

    return d.promise();
};

/**
 * Get the pluginInstance state
 * @param pluginInstance  The plugin instance to get state
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.getState = function (pluginInstance) {
    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var d = new $.Deferred();
    if (!pluginInstance.isSystemCategory()) {
        RestEngine.getJson("/rest/plugin/" + pluginInstance.id + "/state")
            .done(function (data) {
                d.resolve(data);
            })
            .fail(function () {
                d.resolve({ state: "Unknown" });
            });
    } else {
        d.resolve();
    }
    return d.promise();
};

/**
 * Start a plugin instance
 * @param pluginInstance The plugin instance to start
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.start = function (pluginInstance) {
    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
    var d = new $.Deferred();

    if (!pluginInstance.isSystemCategory()) {
        RestEngine.putJson("/rest/plugin/" + pluginInstance.id + "/start")
        .done(d.resolve)
        .fail(d.reject);
    } else {
        d.resolve();
    }
    return d.promise();
};

/**
 * Stop a plugin instance
 * @param pluginInstance The plugin instance to stop
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.stop = function (pluginInstance) {

    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
    var d = new $.Deferred();

    if (!pluginInstance.isSystemCategory()) {
        RestEngine.putJson("/rest/plugin/" + pluginInstance.id + "/stop")
        .done(d.resolve)
        .fail(d.reject);
    } else {
        d.resolve();
    }
    return d.promise();
};

/**
 * Delete a plugin instance
 * @param pluginInstance The plugin instance to delete
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.deleteFromServer = function (pluginInstance) {
    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
    var d = new $.Deferred();

    if (!pluginInstance.isSystemCategory()) {
        RestEngine.deleteJson("/rest/plugin/" + pluginInstance.id)
        .done(d.resolve)
        .fail(d.reject);
    } else {
        d.resolve();
    }
    return d.promise();

};

/**
 * Create a plugin instance to server
 * @param {object} pluginInstance The plugin instance to create to server
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.createToServer = function (pluginInstance) {
    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
    var d = new $.Deferred();

    if (!pluginInstance.isSystemCategory()) {
        var creationData = JSON.stringify({ displayName: pluginInstance.displayName, type: pluginInstance.type, configuration: pluginInstance.configuration, autoStart: pluginInstance.autoStart });
        RestEngine.postJson("/rest/plugin", { data: creationData })
        .done(function (data) {
            //we update our information from the server
            pluginInstance.id = data.id;
            pluginInstance.displayName = data.displayName;
            pluginInstance.type = data.type;
            pluginInstance.configuration = data.configuration;
            pluginInstance.autoStart = parseBool(data.autoStart);
            d.resolve();
        })
        .fail(d.reject);
    } else {
        d.resolve();
    }
    return d.promise();
};

/**
 * Update a plugin instance to server
 * @param pluginInstance The plugin instance to update
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.updateToServer = function (pluginInstance) {
    assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var d = new $.Deferred();

    //we can't update system plugins
    if (!pluginInstance.isSystemCategory()) {
        RestEngine.putJson("/rest/plugin/" + pluginInstance.id, { data : JSON.stringify(pluginInstance)})
            .done(function (data) {
                //it's okay
                //we update our information from the server
                pluginInstance = PluginInstanceManager.factory(data);
                d.resolve();
            })
            .fail(d.reject);
    } else {
        d.resolve();
    }

    return d.promise();
};

/**
 * Get the plugin instance log
 * @param pluginInstance The plugin instance
 * @return {Promise} The log
 */
PluginInstanceManager.getLog = function (pluginInstance) {
	assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
	return RestEngine.getJson("rest/plugin/" + pluginInstance.id + "/log");
}

/**
 * Send an extra command to a plugin instance
 * @param pluginInstance The plugin instance
 * @param extraQuery   The extraQuery
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.postExtraQuery= function (pluginInstance, commandName, commandData) {
	assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
	return RestEngine.postJson("/rest/plugin/" + pluginInstance.id + "/extraQuery/" + commandName, { data: JSON.stringify(commandData) });
};


PluginInstanceManager.getVirtualDevicesSupportedCapacities= function () {
	return RestEngine.getJson("/rest/system/virtualDevicesSupportedCapacities");
};


PluginInstanceManager.buildVirtualDevicePackage=function () {
   
	var d = $.Deferred();
   
   PluginInstanceManager.getVirtualDevicesSupportedCapacities()
   .done(function (capacities) {      
   
      pkg = {
         type: 'system',
         supportManuallyDeviceCreation: 'true',
         deviceConfiguration: {
            staticConfigurationSchema: {
               schemas: {
                  virtualDevice: {
                     types: {
                        virtualDeviceType: {
                           canBeCreatedManually: 'true'
                           }
                        },
                     content: {
                        capacity: {
                           type: 'enum',
                           sort : 'true',
                           values: {}
                        }
                     }
                  }
               }
            }
         }
      }
      
      for (var cap in capacities.capacities) {
         pkg.deviceConfiguration.staticConfigurationSchema.schemas.virtualDevice.content.capacity.values[cap] = cap;
      }
   
      d.resolve(pkg);
   })
   .fail(function (error) {
      console.error("Fail to get virtual devices supported capacities ");      
   
      pkg = {
         type: 'system',
         supportManuallyDeviceCreation: 'false'
      }
      d.resolve(pkg);
   });
   
   return d.promise();
};


/**
 * Download a plugin package for an instance
 * @param pluginInstance The plugin instance
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.downloadPackage = function (pluginInstance) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   var d = new $.Deferred();

   if (!pluginInstance.package) {
      if (!pluginInstance.isSystemCategory()) {
         RestEngine.getJson("plugins/" + pluginInstance.type + "/package.json")
         .done(function (data) {
            pluginInstance.package = data;

            //we manage i18n
            i18n.options.resGetPath = '__ns__/locales/__lng__.json';
            i18n.loadNamespace("plugins/" + pluginInstance.type);

            //we restore the resGetPath
            i18n.options.resGetPath = "locales/__lng__.json";

            d.resolve();
         })
         .fail(d.reject);
      }
      else if (pluginInstance.type === 'system') {
         PluginInstanceManager.buildVirtualDevicePackage()
         .done(function (pkg) {
            pluginInstance.package = pkg;
            
            //we manage i18n
            i18n.options.resGetPath = '__ns__/locales/__lng__.json';
            i18n.loadNamespace("plugins/" + pluginInstance.type);

            //we restore the resGetPath
            i18n.options.resGetPath = "locales/__lng__.json";

            d.resolve();
         })
      } else {
         d.resolve();
      }
   } else {
      d.resolve();
   }

   return d.promise();
};

/**
 * Get all plugin instances which supports manually created device
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.getPluginInstanceHandleManuallyDeviceCreation = function () {
    var d = new $.Deferred();

    //we can't download package from system plugins
    RestEngine.getJson("rest/plugin/instance/handleManuallyDeviceCreation")
        .done(function (data) {
			YadomsInformationManager.getList()
            .done(function (yadomsInfo) {      
            var pluginInstanceList = [];

            if (!isNullOrUndefinedOrEmpty(data.plugin)) {
                $.each(data.plugin, function (index, value) {
                    var pi = PluginInstanceManager.factory(value);
                    //we don't show system plugins to user
                    if (!pi.isSystemCategory()) {
						if( (!yadomsInfo.developerMode && !pi.type.startsWith("dev-")) || yadomsInfo.developerMode) {
							pluginInstanceList.push(pi);
						}
					}
                });
            }
            d.resolve(pluginInstanceList);
			})
			.fail(d.reject);
        })
        .fail(d.reject);
    return d.promise();
};

/**
 * Create a device manually
 * @param pluginInstance The plugin instance
 * @param deviceName The device name
 * @param deviceModel The device model
 * @param deviceConfiguration The device configuration
 * @return {Promise} A promise for the result
 */
PluginInstanceManager.createManuallyDevice = function (pluginInstance, deviceName, deviceModel, deviceType, deviceConfiguration) {
   return RestEngine.postJson("/rest/plugin/" + pluginInstance.id + "/createDevice", { data: JSON.stringify({ name: deviceName, model: deviceModel, type: deviceType, configuration: deviceConfiguration }) });
};