
/**
 * Load Yadoms data
 * @return {{plugins: {}, devices: {}, keywords: {}, recipients: {}}}
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadDataForBlocklyCustomBlocks_ = function () {

    var d = $.Deferred();

    var result = {
        plugins: {},
        devices: {},
        keywords: {},
        recipients: {},
        enumerations: {},
		capacities : {},

        getPluginIdFromKeywordId: function (keywordId) {
            if (this.keywords[keywordId] && this.devices[this.keywords[keywordId].deviceId])
                return this.devices[this.keywords[keywordId].deviceId].pluginId;
            return undefined;
        }
    };

    PluginInstanceManager.getAll().done(function (listPlugins) {
       $.each(listPlugins, function (key, plugin) {
          result.plugins[plugin.id] = plugin;
       });

       DeviceManager.getAll()
       .done(function (listDevices) {
          $.each(listDevices, function (deviceKey, device) {
			 if(parseBool(device.blacklist) === false)
				result.devices[device.id] = device;
          });

          KeywordManager.getAll()
          .done(function (listKeywords) {
             $.each(listKeywords, function (keywordKey, keyword) {
				 //only for not blacklisted devices
				 if(result.devices[keyword.deviceId]) {
					result.keywords[keyword.id] = keyword;
					result.capacities[keyword.capacityName] = keyword;
				 }
             });

             RecipientManager.getAll()
             .done(function (listRecipients) {
                $.each(listRecipients, function (recipientKey, recipient) {
                   result.recipients[recipient.id] = recipient;
                });

                //finalize contructing data
                Blockly.Yadoms.LoadDataFinalize_(result);

                d.resolve(result);
             })
             .fail(function(error) {
                notifyError($.t("modals.recipient.errorGettingList"), error);
             });
          });
       });
    });
   
    return d.promise();
};

/**
 * Finalize data loading
 * @param {Object} result The already loaded objects
 * @returns {} 
 * @private
 */
Blockly.Yadoms.LoadDataFinalize_ = function(result) {
   $.each(result.keywords, function (index, keywordData) {
      var device = result.devices[keywordData.deviceId];
      if (device) {
         var pluginData = result.plugins[device.pluginId];
         if (keywordData && keywordData.type.toUpperCase() === "enum".toUpperCase()) {
            var typeInfo = keywordData.typeInfo;
            if (!isNullOrUndefined(typeInfo) && !isNullOrUndefined(typeInfo.name) && !isNullOrUndefined(typeInfo.values)) {
               var typeToSet = "enum_" + typeInfo.name;

               //all is OK, this is a new enum, ask for translation
               var translatedEnum = [];
               $.each(typeInfo.values, function (index2, value) {
                  var trad = $.t("plugins/" + pluginData.type + ":enumerations." + typeInfo.name + ".values." + value, { defaultValue: value });
                  translatedEnum.push([trad, value]);
               });

               var translatedName = $.t("plugins/" + pluginData.type + ":enumerations." + typeInfo.name + ".name", { defaultValue: typeInfo.name });

               result.enumerations[typeToSet] = {
                  typeToSet: typeToSet,
                  name: typeInfo.name,
                  translatedName: translatedName,
                  values: translatedEnum
               };
            }
         }
      }
   });
}



/**
 * Check if a plugin contains keywords matching specifications
 * @param plugin The plugin to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @param allowedKeywordCapacities Array of allowed capacities (all capacities allowed if undefined)
 * @return {boolean} True if the plugin have matching keywords
 * @constructor
 * @private
 */
Blockly.Yadoms.PluginHasKeywordMatching_ = function (plugin, canWrite, allowedKeywordTypes, allowedKeywordCapacities) {
    var currentPluginMatch = false;

    $.each(Blockly.Yadoms.data.devices, function (index, device) {
        if (device.pluginId === plugin.id) {
            if (Blockly.Yadoms.DeviceHasKeywordMatching_(device, canWrite, allowedKeywordTypes, allowedKeywordCapacities) === true) {
                currentPluginMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
        // ReSharper disable once NotAllPathsReturnValue
    });

    return currentPluginMatch;
};

/**
 * Check if a device contains keywords matching specifications
 * @param device The device to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @param allowedKeywordCapacities Array of allowed capacities (all capacities allowed if undefined)
 * @return {boolean} True if the device have matching keywords
 * @constructor
 * @private
 */
Blockly.Yadoms.DeviceHasKeywordMatching_ = function (device, canWrite, allowedKeywordTypes, allowedKeywordCapacities) {
    var currentDeviceMatch = false;

    $.each(Blockly.Yadoms.data.keywords, function (index, keyword) {
        if (keyword.deviceId === device.id) {
            if (Blockly.Yadoms.KeywordMatching_(keyword, canWrite, allowedKeywordTypes, allowedKeywordCapacities) === true) {
                currentDeviceMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
        // ReSharper disable once NotAllPathsReturnValue
    });
    return currentDeviceMatch;
};

/**
 * Check if a keyword  matching specifications
 * @param keyword The keyword to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @param allowedKeywordCapacities Array of allowed capacities (all capacities allowed if undefined)
 * @return {boolean} True if the keyword matches
 * @constructor
 * @private
 */
Blockly.Yadoms.KeywordMatching_ = function (keyword, canWrite, allowedKeywordTypes, allowedKeywordCapacities) {
    //filter on readonly
    if (canWrite && !keyword.isWritable()) {
        return false;
    }

    var matchTypes = Blockly.Yadoms.KeywordMatchingType_(keyword, allowedKeywordTypes);
    var matchCapacity = Blockly.Yadoms.KeywordMatchingCapacity_(keyword, allowedKeywordCapacities);
    return matchTypes && matchCapacity;
};

/**
 * Check if a keyword  matching allowed types
 * @param keyword The keyword to check
 * @param allowedKeywordTypes Array of allowed types
 * @return {boolean} True if the keyword matches
 * @constructor
 * @private
 */
Blockly.Yadoms.KeywordMatchingType_ = function (keyword, allowedKeywordTypes) {
    if (allowedKeywordTypes != null) {
        return $.inArray(keyword.type.toLowerCase(), allowedKeywordTypes) !== -1;
    }
    //if allowedKeywordTypes is null or empty
    return true;
};

/**
 * Check if a keyword  matching allowed capacities
 * @param keyword The keyword to check
 * @param allowedKeywordCapacities Array of allowed capacities
 * @return {boolean} True if the keyword matches
 * @constructor
 * @private
 */
Blockly.Yadoms.KeywordMatchingCapacity_ = function (keyword, allowedKeywordCapacities) {
    if (allowedKeywordCapacities != null) {
        return $.inArray(keyword.capacityName.toLowerCase(), allowedKeywordCapacities) !== -1;
    }
    //if allowedKeywordCapacities is null or empty
    return true;
};

/**
 * Return the plugin id matching a device
 * @param device The device
 * @return {Integer} The plugin id
 * @private
 */
Blockly.Yadoms.GetPluginMatchingDevice_ = function (device) {
    if (device) {
        if (Blockly.Yadoms.data.devices[device]) {
            return Blockly.Yadoms.data.devices[device].pluginId;
        }
    }

    return null;
}

/**
 * Return an array of devices (Array: [name, id]) for a plugin instance id
 * @param selectedPluginId The plugin id
 * @param onlyWritableKeywords If true, only devices with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @param allowedKeywordCapacities Array of allowed capacities
 * @return {Array} The devices array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadDevices_ = function (onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities) {

    var deviceList = [];
    $.each(Blockly.Yadoms.data.devices, function (index, device) {
        if (Blockly.Yadoms.DeviceHasKeywordMatching_(device, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities)) {
            deviceList.push([device.friendlyName, device.id]);
        }
    });
	
	deviceList.sort(function(a, b){
	   var aName = a[0].toLowerCase();
	   var bName = b[0].toLowerCase(); 
	   return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
	});
	
    return deviceList;
};

/**
 * Return an array of devices (Array: [name, id]) for a device id
 * @param selectedDeviceId The device id
 * @param onlyWritableKeywords If true, only devices with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @param allowedKeywordCapacities Array of allowed capacities
 * @return {Array} The devices array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadKeywords_ = function (selectedDeviceId, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities) {

    var keywordList = [];
    $.each(Blockly.Yadoms.data.keywords, function (index, keyword) {
        if (keyword.deviceId === selectedDeviceId) {
            if (Blockly.Yadoms.KeywordMatching_(keyword, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities)) {
                keywordList.push([keyword.friendlyName, keyword.id]);
            }
        }
    });
	
	keywordList.sort(function(a, b){
	   var aName = a[0].toLowerCase();
	   var bName = b[0].toLowerCase(); 
	   return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
	});
	
    return keywordList;
};

/**
 * Return an array of capacities (Array: [name, name]) 
 * @private
 */
Blockly.Yadoms.LoadCapacities_ = function() {
    var capacityList = [];
    $.each(Blockly.Yadoms.data.capacities, function (index, keyword) {
		capacityList.push([index, index]);
    });
	
	capacityList.sort(function(a, b){
	   var aName = a[0].toLowerCase();
	   var bName = b[0].toLowerCase(); 
	   return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
	});
	
    return capacityList;	
};

/**
 * Return an array of recipients (Array: [name, id])
 * @return {Array} The recipients array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadRecipients_ = function () {
    var recipientList = [];
    $.each(Blockly.Yadoms.data.recipients, function (index, recipient) {
        recipientList.push([recipient.toString(), recipient.id]);
    });
	
	recipientList.sort(function(a, b){
	   var aName = a[0].toLowerCase();
	   var bName = b[0].toLowerCase(); 
	   return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
	});
	
    return recipientList;
};
