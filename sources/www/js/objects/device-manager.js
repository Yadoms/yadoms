/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function DeviceManager() { }

DeviceManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.pluginId), "json.pluginId must be defined");
    assert(!isNullOrUndefined(json.name), "json.name must be defined");
    assert(!isNullOrUndefined(json.friendlyName), "json.friendlyName must be defined");
    assert(!isNullOrUndefined(json.model), "json.model must be defined");
    assert(!isNullOrUndefined(json.type), "json.model must be defined");

    return new Device(json.id, json.pluginId, json.name, json.friendlyName, json.model, json.type, json.configuration, json.blacklist);
};

/**
 * Get a device from its id
 * @param {Integer|String} deviceId The device id to find
 * @return {Promise}
 */
DeviceManager.get = function (deviceId) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("rest/device/" + deviceId)
    .done(function(data) {
        d.resolve(DeviceManager.factory(data));
    })
    .fail(d.reject);
    
    return d.promise();
};

/**
 * Get all devices
 * @ return {Promise}
 */
DeviceManager.getAll = function () {
    var d = new $.Deferred();

    RestEngine.getJson("rest/device")
    .done(function (data) {
        var devices = [];
        //foreach result we append a <tr>
        $.each(data.device, function (index, value) {
            devices.push(DeviceManager.factory(value));
        });
        d.resolve(devices);
    })
    .fail(d.reject);

    return d.promise();
};

/**
 * Get the device attached to a plugin
 * @param {Object} device The device
 * @ return {Promise}
 */
DeviceManager.getAttachedPlugin = function (device, force) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    if(!device.attachedPlugin || force === true) {
        PluginInstanceManager.get(device.pluginId)
        .done(function (pluginInstance) {
            device.attachedPlugin = pluginInstance;
            d.resolve();
        }).fail(d.reject);
    } else {
       d.resolve();
    }

    return d.promise();
};

/**
 * Get plugins attached to an array of devices
 * @param {Object} devices All devices
 * @ return {Promise}
 */
DeviceManager.getAttachedPlugins = function (devices) {
    assert(!isNullOrUndefined(devices), "devices must be defined");
    assert(devices instanceof Array, "devices must be an array");

    var d = new $.Deferred();
    PluginInstanceManager.getAll()
       .done(function (pluginInstances) {
          $.each(devices, function (index, device) {
             $.each(pluginInstances, function (index2, instance) {
                 if (device.pluginId === instance.id){
                    devices[index].attachedPlugin = PluginInstanceManager.factory(instance);
                 }
             });
          });
          d.resolve();
       })
       .fail(d.reject);
    return d.promise();
};

/**
 * Get the keywords attached to a device id
 * @param {Integer} deviceId The device id
 * @ return {Promise}
 */
DeviceManager.getKeywordsByDeviceId = function (deviceId) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("/rest/device/" + deviceId + "/keyword")
    .done(function (data) {
        var list = [];
        $.each(data.keyword, function (index, value) {
            list.push(KeywordManager.factory(value));
        });
        d.resolve(list);
    })
    .fail(d.reject);

    return d.promise();
};

/**
 * Get the keywords attached to a device id
 * @param {Integer} deviceId The device id
 * @ return {Promise}
 */
DeviceManager.getKeywordsBydeviceIdAndCapacity = function (deviceId, KeywordAccessMode, capacityName) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
    assert(!isNullOrUndefined(KeywordAccessMode), "KeywordAccessMode must be defined");
    assert(!isNullOrUndefined(capacityName), "capacity must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("/rest/device/" + deviceId + "/" + KeywordAccessMode + "/" + capacityName)
    .done(function (data) {
        var list = [];
        $.each(data.keyword, function (index, value) {
            list.push(KeywordManager.factory(value));
        });
        d.resolve(list);
    })
    .fail(d.reject);

    return d.promise();
};

/**
 * Get the keywords attached to a device
 * @param {Object} device The device
 * @param {boolean} forceReload if true force reloading keywords fro mserver
 * @ return {Promise}
 */
DeviceManager.getKeywords = function (device, forceReload) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    if (isNullOrUndefined(device.keywords) || forceReload === true) {
        DeviceManager.getKeywordsByDeviceId(device.id)
        .done(function (list) {
            device.keywords = list;
            d.resolve(list);
        })
        .fail(d.reject);
    }
    else {
        //keyword has already been gotten
        d.resolve();
    }

    return d.promise();
};

/**
 * Delete a device
 * @param {Object} device The device to delete
 * @param {Boolean} deleteDevice Indicate if the device should be deleted (true) or just blacklisted (true)
 * @ return {Promise}
 */
DeviceManager.deleteDevice = function (device, deleteDevice) {
    assert(!isNullOrUndefined(device), "device must be defined");

    if (isNullOrUndefined(deleteDevice)) {
        deleteDevice = false;
    }

    return RestEngine.deleteJson("/rest/device/" + device.id + (deleteDevice ? "/removeDevice" : "/blacklist"));
};

/**
 * Restore a device
 * @param {Object} device The device to restore
 * @ return {Promise}
 */
DeviceManager.restoreDevice = function(device) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    RestEngine.putJson("/rest/device/" + device.id + "/restore")
    .done(function (data) {
        //it's okay
        //we update our information from the server
        device = DeviceManager.factory(data);
        d.resolve(device);
    }).fail(d.reject);

    return d.promise();
};

/**
 * Get a device configuration schema
 * @param {Object} device The device to get the configuration schema
 * @ return {Promise}
 */
DeviceManager.getConfigurationSchema = function(device) {

    var d = new $.Deferred();

    DeviceManager.getAttachedPlugin(device)   
    .done(function () {
        //get the plugin package.json
        PluginInstanceManager.downloadPackage(device.attachedPlugin)
        .done(function () {
            //try to get schema from the device model
            device.attachedPlugin.getPackageDeviceConfigurationSchema()
            .done(function(deviceConfig) {
                var schema = {};
                if(deviceConfig) {
                    //Manage static configuration
                    if(deviceConfig.staticConfigurationSchema && deviceConfig.staticConfigurationSchema.schemas) {

                        //find all static configurations matching the device model
                        var staticConfigMatchingDevice= {};
                        for(var k in deviceConfig.staticConfigurationSchema.schemas) {
                            if(_.some(deviceConfig.staticConfigurationSchema.schemas[k].types, function(typeContent, model) { 
                              return model == "*" || model == device.type;
                            }))  {
                               //add it to resulting schema
                               var config = deviceConfig.staticConfigurationSchema.schemas[k];
                               if(config && config.content) {
                                  for(var l in config.content) {
                                     config.content[l].i18nBasePath = "plugins." + device.attachedPlugin.type + ":deviceConfiguration.staticConfigurationSchema.schemas." + k + ".content.";
                                  }                            
                                  schema = _.merge(schema, config.content);
                               }
                            }
                        }
                    }

                    //Manage dynamic configuration
                    if(deviceConfig.dynamicConfigurationSchema && parseBool(deviceConfig.dynamicConfigurationSchema) === true) {

                        //ask the device configuration from the plugin instance 
                        RestEngine.getJson("/rest/device/" + device.id + "/configurationSchema")
                        .done(function (dynamicSchema) {
                            if(dynamicSchema)
                                schema = _.merge(schema, dynamicSchema);
                            device.attachedPlugin.applyBinding(schema, true, true)
                            .done(d.resolve)
                            .fail(d.reject);
                        }).fail(d.reject);                        
                    } else {
                        device.attachedPlugin.applyBinding(schema, true, true)
                        .done(d.resolve)
                        .fail(d.reject);
                    }
                } else {
                    //device configuration not exists in package.json
                    d.resolve(schema);
                }
            }).fail(d.reject);

        }).fail(d.reject);
    }).fail(d.reject);

    return d.promise();
}

/**
 * Update a device configuration
 * @param {Object} device The device to update
 * @ return {Promise}
 */
DeviceManager.updateToServer = function (device) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    RestEngine.putJson("/rest/device/" + device.id + "/configuration", { data: JSON.stringify(device) })
    .done(function (data) {
        //it's okay
        //we update our information from the server
        device = DeviceManager.factory(data);
        d.resolve(device);
    }).fail(d.reject);

    return d.promise();
};
