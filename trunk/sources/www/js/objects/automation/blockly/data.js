
/**
 * Load Yadoms data
 * @return {{plugins: {}, devices: {}, keywords: {}, recipients: {}}}
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadDataForBlocklyCustomBlocks_ = function () {

    var result = {
        plugins: {},
        devices: {},
        keywords: {},
        recipients: {},
        enumerations : {}
    };

    PluginInstanceManager.getAll(function (list) {
        $.each(list, function (key, plugin) {
            result.plugins[plugin.id] = plugin;
        });
    }, true);

    DeviceManager.getAll(function (list) {
        $.each(list, function (deviceKey, device) {
            result.devices[device.id] = device;
        });
    }, true);

    KeywordManager.getAll(function (list) {
        $.each(list, function (keywordKey, keyword) {
            result.keywords[keyword.id] = keyword;
        });
    }, true);

    RecipientManager.getAll(function (list) {
        $.each(list, function (recipientKey, recipient) {
            result.recipients[recipient.id] = recipient;
        });
    }, true);


    $.each(result.keywords, function (index, keywordData) {
        var pluginData = result.plugins[result.devices[keywordData.deviceId].pluginId];
        if (!isNullOrUndefined(keywordData) && keywordData.type.toUpperCase() === "enum".toUpperCase()) {
            var typeInfo = keywordData.typeInfo;
            if (!isNullOrUndefined(typeInfo) && !isNullOrUndefined(typeInfo.name) && !isNullOrUndefined(typeInfo.values)) {
                var typeToSet = "enum_" + typeInfo.name;

                //all is OK, this is a new enum, ask for translation
                var translatedEnum = [];
                $.each(typeInfo.values, function (index, value) {
                    var trad = $.t(pluginData.type + ":enumerations." + typeInfo.name + "." + value, { defaultValue: pluginData.package.enumerations[typeInfo.name][value] });
                    translatedEnum.push([trad, value]);
                });

                result.enumerations[typeToSet] = {
                    typeToSet: typeToSet,
                    name: typeInfo.name,
                    values: translatedEnum
                };

                //apply new dropdown list
                /*
                this.currentEnumerationTypeName = typeInfo.name;
                var enumDropDown = $.isFunction(this.getField_) ? this.getField_(this.enumerationDropDownName) : this.getField(this.enumerationDropDownName);

                enumDropDown.refresh(translatedEnum, enumDropDown.getValue());*/
            }
        }

    });
    

    return result;
};




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
    return keywordList;
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
    return recipientList;
};
