/**

Keyword value 
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#r53dn5


Keyword setter
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#rttajf

Operator
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#g3gzx9

Type switch
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#3tdeuk

Notification
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#oh6zax

 Is For
 https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#34eirj


Exemple de générateur de code (python, xml, dart,....)
https://blockly-demo.appspot.com/static/demos/code/index.html

*/

/**
 * Hardcoded interpreter name
 * @type {string}
 */
var BlocklyName = "blockly";

/**
 * Initialize Blockly.Yadoms structure
 * @constructor
 */
Blockly.Yadoms = function() {
};


/**
 * List of operators for numbers
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.NumberOperators_ = Blockly.RTL ? [
    ['=', 'EQ'],
    ['\u2260', 'NEQ'],
    ['>', 'LT'],
    ['\u2265', 'LTE'],
    ['<', 'GT'],
    ['\u2264', 'GTE']
] : [
    ['=', 'EQ'],
    ['\u2260', 'NEQ'],
    ['<', 'LT'],
    ['\u2264', 'LTE'],
    ['>', 'GT'],
    ['\u2265', 'GTE']
];

/**
 * List of operators for strings
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.StringOperators_ = Blockly.RTL ? [
    ['=', 'EQ'],
    ['\u2260', 'NEQ'],
    ['>', 'LT'],
    ['\u2265', 'LTE'],
    ['<', 'GT'],
    ['\u2264', 'GTE']
] : [
    ['=', 'EQ'],
    ['\u2260', 'NEQ'],
    ['<', 'LT'],
    ['\u2264', 'LTE'],
    ['>', 'GT'],
    ['\u2265', 'GTE']
];

/**
 * List of operators for booleans
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.BooleanOperators_ = Blockly.RTL ? [
    ['=', 'EQ'],
    ['\u2260', 'NEQ']
] : [
    ['=', 'EQ'],
    ['\u2260', 'NEQ']
];


/**
 * Load Yadoms data
 * @return {{plugins: {}, devices: {}, keywords: {}, recipients: {}}}
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadDataForBlocklyCustomBlocks_ = function () {

    var result = {
        plugins : {},
        devices : {},
        keywords : {},
        recipients : {}
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

    return result;
};



/**
 * Create the toolbox
 * @return {string} The toolbox xml string
 */
Blockly.Yadoms.CreateToolbox_ = function() {

    var catLogic = $.t("blockly.toolbox.catLogic");
    var catLoops = $.t("blockly.toolbox.catLoops");
    var catMath = $.t("blockly.toolbox.catMath");
    var catText = $.t("blockly.toolbox.catText");
    var catLists = $.t("blockly.toolbox.catLists");
    var catColour = $.t("blockly.toolbox.catColour");

    var toolbox = '<xml>';
    toolbox += '  <category name="Yadoms">';
    toolbox += '    <block type="yadoms_keyword_value"></block>';
    toolbox += '    <block type="yadoms_affect_keyword"></block>';
    toolbox += '    <block type="yadoms_notification_simple"></block>';
    toolbox += '    <block type="yadoms_notification_advanced"></block>';
    toolbox += '    <block type="yadoms_enumeration"></block>';
    toolbox += '    <block type="yadoms_logic_compare_become"></block>';
    toolbox += '    <block type="yadoms_logic_compare_is"></block>';
    toolbox += '    <block type="yadoms_sleep">';
    toolbox += '        <value name="Time">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">10</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '    </block>';
    toolbox += '    <block type="yadoms_log">';
    toolbox += '        <value name="LogContent">';
    toolbox += '           <block type="text">';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catLogic +'">';
    toolbox += '     <block type="controls_if"></block>';
    toolbox += '     <block type="logic_compare"></block>';
    toolbox += '     <block type="logic_operation"></block>';
    toolbox += '     <block type="logic_negate"></block>';
    toolbox += '     <block type="logic_boolean"></block>';
    toolbox += '     <block type="logic_null"></block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catLoops +'">';
    toolbox += '     <block type="controls_repeat_ext">';
    toolbox += '        <value name="TIMES">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">10</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="controls_whileUntil"></block>';
    toolbox += '     <block type="controls_for">';
    toolbox += '        <value name="FROM">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">1</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="TO">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">10</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="BY">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">1</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="controls_forEach"></block>';
    toolbox += '     <block type="controls_flow_statements"></block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catMath +'">';
    toolbox += '     <block type="math_number"></block>';
    toolbox += '     <block type="math_arithmetic"></block>';
    toolbox += '     <block type="math_single"></block>';
    toolbox += '     <block type="math_trig"></block>';
    toolbox += '     <block type="math_constant"></block>';
    toolbox += '     <block type="math_number_property"></block>';
    toolbox += '     <block type="math_change">';
    toolbox += '        <value name="DELTA">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">1</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="math_round"></block>';
    toolbox += '     <block type="math_on_list"></block>';
    toolbox += '     <block type="math_modulo"></block>';
    toolbox += '     <block type="math_constrain">';
    toolbox += '        <value name="LOW">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">1</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="HIGH">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">100</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="math_random_int">';
    toolbox += '        <value name="FROM">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">1</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="TO">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">100</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="math_random_float"></block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catText +'">';
    toolbox += '     <block type="text"></block>';
    toolbox += '     <block type="text_join"></block>';
    toolbox += '     <block type="text_append">';
    toolbox += '        <value name="TEXT">';
    toolbox += '           <block type="text"></block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="text_length"></block>';
    toolbox += '     <block type="text_isEmpty"></block>';
    toolbox += '     <block type="text_indexOf">';
    toolbox += '        <value name="VALUE">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">text</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="text_charAt">';
    toolbox += '        <value name="VALUE">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">text</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="text_getSubstring">';
    toolbox += '        <value name="STRING">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">text</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="text_changeCase"></block>';
    toolbox += '     <block type="text_trim"></block>';
    toolbox += '     <block type="text_print"></block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catLists +'">';
    toolbox += '     <block type="lists_create_empty"></block>';
    toolbox += '     <block type="lists_create_with"></block>';
    toolbox += '     <block type="lists_repeat">';
    toolbox += '        <value name="NUM">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">5</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="lists_length"></block>';
    toolbox += '     <block type="lists_isEmpty"></block>';
    toolbox += '     <block type="lists_indexOf">';
    toolbox += '        <value name="VALUE">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">list</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="lists_getIndex">';
    toolbox += '        <value name="VALUE">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">list</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="lists_setIndex">';
    toolbox += '        <value name="LIST">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">list</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="lists_getSublist">';
    toolbox += '        <value name="LIST">';
    toolbox += '           <block type="variables_get">';
    toolbox += '              <field name="VAR">list</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="lists_split">';
    toolbox += '        <value name="DELIM">';
    toolbox += '           <block type="text">';
    toolbox += '              <field name="TEXT">,</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '  </category>';
    toolbox += '  <category name=" ' + catColour +'">';
    toolbox += '     <block type="colour_picker"></block>';
    toolbox += '     <block type="colour_random"></block>';
    toolbox += '     <block type="colour_rgb">';
    toolbox += '        <value name="RED">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">100</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="GREEN">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">50</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="BLUE">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">0</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '     <block type="colour_blend">';
    toolbox += '        <value name="COLOUR1">';
    toolbox += '           <block type="colour_picker">';
    toolbox += '              <field name="COLOUR">#ff0000</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="COLOUR2">';
    toolbox += '           <block type="colour_picker">';
    toolbox += '              <field name="COLOUR">#3333ff</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '        <value name="RATIO">';
    toolbox += '           <block type="math_number">';
    toolbox += '              <field name="NUM">0.5</field>';
    toolbox += '           </block>';
    toolbox += '        </value>';
    toolbox += '     </block>';
    toolbox += '  </category>';
    toolbox += '  <sep></sep>';
    toolbox += '</xml>';
    return toolbox;
};

/**
 * Load the blockly language file
 * @param callback The callback used when load is done
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadLanguageScript_ = function(callback) {

    //first try to load user specific language
    var currentLng = i18n.options.lng.substring(0, 2);

    $.getScript( "libs/blockly/js/locales/" + currentLng + ".js" )
        .done(function() {
            callback();
        })
        .fail(function() {
            console.warning("Fail to load Blockly for " + currentLng);
            //if it fails, load english one
            $.getScript( "libs/blockly/js/locales/en.js" )
                .done(function() {
                    callback();
                }.fail(function() {
                    callback();
                }));
        });
};

/**
 * Initialize blockly for Yadoms
 * @param $domTarget The div in which inject Blockly
 * @param initialContent The initial blockly content (XML)
 * @param maxTopBlocks The maximum number of TopBlocks
 * @constructor
 */
Blockly.Yadoms.Initialize = function($domTarget, initialContent, maxTopBlocks) {
    Blockly.Yadoms.data = Blockly.Yadoms.LoadDataForBlocklyCustomBlocks_();

    Blockly.Yadoms.LoadLanguageScript_(function() {
        //inject blockly dom+js
        Blockly.inject($domTarget, {toolbox: Blockly.Yadoms.CreateToolbox_()});

        //create empty script (with fixed condition/action block)
        if(!isNullOrUndefinedOrEmpty(initialContent)) {
            var xml = Blockly.Xml.textToDom(initialContent);
            Blockly.Xml.domToWorkspace(Blockly.mainWorkspace, xml);
        }

        //initialize validation
        Blockly.Validation.Init(maxTopBlocks);
    });
};

/**
 * Check if a plugin contains keywords matching specifications
 * @param plugin The plugin to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @return {boolean} True if the plugin have matching keywords
 * @constructor
 * @private
 */
Blockly.Yadoms.PluginHasKeywordMatching_ = function(plugin, canWrite, allowedKeywordTypes) {
    var currentPluginMatch = false;

    $.each(Blockly.Yadoms.data.devices, function(index, device) {
        if(device.pluginId == plugin.id) {
            if (Blockly.Yadoms.DeviceHasKeywordMatching_(device, canWrite, allowedKeywordTypes) == true) {
                currentPluginMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
    });

    return currentPluginMatch;
};

/**
 * Check if a device contains keywords matching specifications
 * @param device The device to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @return {boolean} True if the device have matching keywords
 * @constructor
 * @private
 */
Blockly.Yadoms.DeviceHasKeywordMatching_ = function(device, canWrite, allowedKeywordTypes) {
    var currentDeviceMatch = false;

    $.each(Blockly.Yadoms.data.keywords, function(index, keyword) {
        if(keyword.deviceId == device.id) {
            if (Blockly.Yadoms.KeywordMatching_(keyword, canWrite, allowedKeywordTypes) == true) {
                currentDeviceMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
    });
    return currentDeviceMatch;
};

/**
 * Check if a keyword  matching specifications
 * @param keyword The keyword to check
 * @param canWrite If true, only keywords with write capacity are allowed
 * @param allowedKeywordTypes Array of allowed types
 * @return {boolean} True if the keyword matches
 * @constructor
 * @private
 */
Blockly.Yadoms.KeywordMatching_ = function(keyword, canWrite, allowedKeywordTypes) {
    //filter on readonly
    if(canWrite && keyword.accessMode != "getset") {
        return false;
    }
    if(allowedKeywordTypes != null && allowedKeywordTypes != undefined) {
        return $.inArray(keyword.type, allowedKeywordTypes) != -1;
    }
    return true;
};


/**
 * Return an array of plugins (Array: [name, id])
 * @param canWrite If true, only plugin with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @return {Array} The plugin array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadPlugins_ = function(canWrite, allowedKeywordTypes) {

    var pluginList = [];
    $.each(Blockly.Yadoms.data.plugins, function(index, plugin) {
        if(Blockly.Yadoms.PluginHasKeywordMatching_(plugin, canWrite, allowedKeywordTypes)) {
            pluginList.push([plugin.name, plugin.id]);
        }
    });
    return pluginList;
};

/**
 * Return an array of devices (Array: [name, id]) for a plugin instance id
 * @param selectedPluginId The plugin id
 * @param canWrite If true, only devices with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @return {Array} The devices array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadDevices_ = function(selectedPluginId, canWrite, allowedKeywordTypes) {

    var deviceList = [];
    $.each(Blockly.Yadoms.data.devices, function(index, device) {
        if(device.pluginId == selectedPluginId) {
            if (Blockly.Yadoms.DeviceHasKeywordMatching_(device, canWrite, allowedKeywordTypes)) {
                deviceList.push([device.friendlyName, device.id]);
            }
        }
    });
    return deviceList;
};

/**
 * Return an array of devices (Array: [name, id]) for a device id
 * @param selectedDeviceId The device id
 * @param canWrite If true, only devices with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @return {Array} The devices array
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadKeywords_ = function(selectedDeviceId, canWrite, allowedKeywordTypes) {

    var keywordList = [];
    $.each(Blockly.Yadoms.data.keywords, function(index, keyword) {
        if(keyword.deviceId == selectedDeviceId) {
            if (Blockly.Yadoms.KeywordMatching_(keyword, canWrite, allowedKeywordTypes)) {
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
Blockly.Yadoms.LoadRecipients_ = function() {
    var recipientList = [];
    $.each(Blockly.Yadoms.data.recipients, function(index, recipient) {
        recipientList.push([recipient.toString(), recipient.id]);
    });
    return recipientList;
};


/**
 * Function which convert a yadoms type, to a blockly type
 * @param yadomsKeywordType  The yadoms type
 * @returns {*} The blockly type matching yadoms type
 * @constructor
 * @param yadomsTypeName The yadoms type name. In case of enum, this is the internal enum name
 */
Blockly.Yadoms.GetBlocklyType_ = function (yadomsKeywordType, yadomsTypeName) {
    switch(yadomsKeywordType.toLowerCase()){
        case "numeric":
            return "Number";
        case "string":
        case "json":
            return "String";
        case "bool":
            return "Boolean";
        case "enum":
            return "enum_" + yadomsTypeName;
        default:
            return null; //is not known allow any type
    }
};



/**
 * Update a block colour depending on its type
 * @param self The block
 * @param type The block type
 * @constructor
 */
Blockly.Yadoms.UpdateBlockColour_ = function(self, type) {
    if(type == "String")
        self.setColour(160);
    else if(type == "Boolean")
        self.setColour(210);
    else if(type == "Number")
        self.setColour(230);
    else if(type == "null" ||  type == null || type == undefined)
        self.setColour(230);
    else
        self.setColour(20);
};


/**
 * Configure a custom block by adding the dropdowns allowing selecting a keyword
 * @param thisBlock The block to configure
 * @param canWrite If true allow keywords that could be written
 * @param allowedKeywordTypes Array of allowed keyword type ("numeric", "string", "bool", "json", "nodata"
 * @param callbackKeywordSelectionChanged A callback for keyword notification change
 * @param pluginDropDownName The name of the plugins DropDown
 * @param deviceDropDownName The name of the devices DropDown
 * @param keywordDropDownName The name of the keywords DropDown
 * @constructor
 */
Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection = function(thisBlock, canWrite, allowedKeywordTypes, callbackKeywordSelectionChanged, pluginDropDownName, deviceDropDownName, keywordDropDownName) {
    var pluginDd, deviceDd, keywordDd;


    //plugin list is static (do not need to update it on each dropdown click)
    var pList = Blockly.Yadoms.LoadPlugins_(canWrite, allowedKeywordTypes);
    if(pList == null || pList == undefined || pList.length == 0) {
        thisBlock.setDisabled(true);
        pList = [["invalid" , "invalid"]];
    }

    //create the plugin dropdown
    pluginDd = new Blockly.FieldDropdown(pList, function(plugin) {
        if(!thisBlock.disabled)
            deviceDd.refresh(Blockly.Yadoms.LoadDevices_(plugin, canWrite, allowedKeywordTypes));
    });


    deviceDd = new Blockly.FieldDropdown(function() {
        var deviceList= Blockly.Yadoms.LoadDevices_(pluginDd.getValue(), canWrite, allowedKeywordTypes);
        if(deviceList == null || deviceList == undefined || deviceList.length == 0) {
            thisBlock.setDisabled(true);
            deviceList = [["invalid" , "invalid"]];
        }
        return deviceList;
    }, function(device) {
        if(!thisBlock.disabled)
            keywordDd.refresh(Blockly.Yadoms.LoadKeywords_(device, canWrite, allowedKeywordTypes));
    });


    keywordDd = new Blockly.FieldDropdown(function() {
        var keywordList= Blockly.Yadoms.LoadKeywords_(deviceDd.getValue(), canWrite, allowedKeywordTypes);
        if(keywordList == null || keywordList == undefined || keywordList.length == 0) {
            thisBlock.setDisabled(true);
            keywordList = [["invalid" , "invalid"]];
        }
        return keywordList;
    }, function(keyword) {
        if (!thisBlock.disabled) {

            var yadomsTypeName="";
            if(!isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo) && !isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo.name))
                yadomsTypeName =  Blockly.Yadoms.data.keywords[keyword].typeInfo.name;

            var type = Blockly.Yadoms.GetBlocklyType_(Blockly.Yadoms.data.keywords[keyword].type, yadomsTypeName);
            callbackKeywordSelectionChanged(keyword, type);
        }
    });

    //add event fire value changed
    pluginDd.setValue =function(newValue) {
        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };

    //add event fire value changed
    deviceDd.setValue =function(newValue) {
        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };


    //add event fire value changed
    keywordDd.setValue =function(newValue) {
        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };


    thisBlock.getSelectedKeyword = function() {
        return keywordDd.getValue();
    };
    thisBlock.getSelectedDevice = function() {
        return deviceDd.getValue();
    };
    thisBlock.getSelectedPlugin = function() {
        return pluginDd.getValue();
    };

    thisBlock.appendDummyInput("YadomsSelection")
        .appendField(pluginDd, pluginDropDownName)
        .appendField(deviceDd, deviceDropDownName)
        .appendField(keywordDd, keywordDropDownName);

    pluginDd.changeHandler_(pluginDd.getValue());

};



/**
 * Get the blockly result (xml, python)
 * @param callback A callback to receive results (xml, python)
 * @constructor
 */
Blockly.Yadoms.GetResult = function(callback) {
    assert(!isNullOrUndefined(callback), "callback must be defined");

    //get xml code
    var xmlDomString = Blockly.Xml.workspaceToDom(Blockly.mainWorkspace);
    var xmlString = Blockly.Xml.domToText(xmlDomString);

    //get python code
    var pythonCode = Blockly.Python.workspaceToCode();

    var pythonCodeIndented = Blockly.Python.prefixLines(pythonCode, '\t');

    var completedPythonCode = '# yMain is the script entry point, called by Yadoms' + '\n';
    completedPythonCode +="def yMain():" + '\n';
    completedPythonCode += '\t' + "print 'Script started'" + '\n';
    completedPythonCode += pythonCodeIndented;

    callback(xmlString, completedPythonCode);
};


/**
 * Dropdown refresh method
 * The function update the dropdown values, select the first one and call the change handler
 * -> Main blockly hack (not using only public memebers)
 * @param data The new dropdown values
 * @param tryToSelectValue The value of new values to select
 */
Blockly.FieldDropdown.prototype.refresh = function(data, tryToSelectValue) {
    if(data != undefined && data.length >0) {
        this.menuGenerator_ = data;

        var valueFound = false;
        $.each(data, function(index, val){
           if(val[1] == tryToSelectValue)
               valueFound =true;
        });

        if(isNullOrUndefined(tryToSelectValue) || !valueFound)
            this.setValue(data[0][1]);
        else
            this.setValue(tryToSelectValue);
//        this.updateTextNode_();
//        if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
//            this.changeHandler_(this.getValue());
    }
};

// And declare it is a subclass of the first
//Blockly.Yadoms.FieldDropdown.prototype = new Blockly.FieldDropdown();


// Add a second class
//function ExFieldDropdown(){
//}
//
//ExFieldDropdown.prototype = new Blockly.FieldDropdown();
//
//
//ExFieldDropdown.prototype.setValue =function(newValue) {
//    var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
//    if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
//        this.changeHandler_(this.getValue());
//    return result;
//};



/**
 * Define a block which allow to read a keyword value
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_keyword_value'] = {
    pluginDropDownName : "Plugin",
    deviceDropDownName : "Device",
    keywordDropDownName : "Keyword",
    init: function () {

        //set custom block parameters
        this.setInputsInline(true);
        this.setOutput(true);
        this.setTooltip('');
        this.setHelpUrl('http://www.example.com/');

        var thisBlock = this;
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(thisBlock, false, ["numeric", "string", "bool", "enum"], function (keyword, keywordType) {
            if (keywordType == null || keywordType == undefined)
                thisBlock.changeOutput("null"); //any type allowed
            else
                thisBlock.changeOutput(keywordType);
            Blockly.Yadoms.UpdateBlockColour_(thisBlock, keywordType);
        }, this.pluginDropDownName, this.deviceDropDownName, this.keywordDropDownName);
    }
};



/**
 * Define the python generation function for yadoms_keyword_value block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['yadoms_keyword_value'] = function(block) {
    var readCommand = 'yadoms.readKeyword(' + dropdown_keyword + ')';

    var dropdown_keyword = block.getSelectedKeyword();
    var keyword = Blockly.Yadoms.data.keywords[dropdown_keyword];
    var code = "";

    switch(keyword.type) {
        case 'numeric':
            code = 'float(' + readCommand + ')';
            break;
        case 'boolean' :
            code = 'bool(' + readCommand + ')';
            break;
        default:
            code = readCommand;
            break;
    }

    return [code, Blockly.Python.ORDER_ATOMIC];
};


/**
 * Define a block which allow to change a keyword value
 * @type {{inputValueName: string, operatorValueName: string, init: Function, updateShape_: Function}}
 */
Blockly.Blocks['yadoms_affect_keyword'] = {
    inputValueName : "Value",
    operatorValueName : "operator",
    unitsInputName : "units",
    pluginDropDownName : "Plugin",
    deviceDropDownName : "Device",
    keywordDropDownName : "Keyword",
    inputValueShown : false,
  init: function() {
      this.setHelpUrl('http://www.example.com/');
      this.setInputsInline(true);
      this.setPreviousStatement(true, "null");
      this.setNextStatement(true, "null");
      this.setTooltip('');
      this.setColour(0);

      this.appendDummyInput()
          .appendField($.t("blockly.blocks.yadoms_affect_keyword.title"));
      var thisBlock = this;

      Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, true, ["numeric", "string", "bool", "nodata", "enum"], function (keyword, keywordType) {
          if (keywordType == null || keywordType == undefined) {
              thisBlock.updateShape_(false);
          }
          else {
              thisBlock.updateShape_(true);
              thisBlock.getInput(thisBlock.inputValueName).setCheck(keywordType);
          }
          thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keyword]);
      }, thisBlock.pluginDropDownName, thisBlock.deviceDropDownName, thisBlock.keywordDropDownName);


      /**
       * Method which make type checks when one of connector changes
       */
      this.onchange = function() {
          var keywordValue = thisBlock.getSelectedKeyword();
          var deviceValue = thisBlock.getSelectedDevice();
          var pluginValue = thisBlock.getSelectedPlugin();

          var inputValue = thisBlock.getInput(thisBlock.inputValueName);
          if (!inputValue) {
              return;  // Block under construction, ignore.
          }
          var block = inputValue.connection.targetBlock();
          if (block) {
              if(block.type == "yadoms_enumeration") {
                  block.updateEnumeration(keywordValue, deviceValue, pluginValue);
              }
          }
      };
  },

    /**
     * Modify this block to have (or not have) an input for 'is divisible by'.
     * @param {boolean} extraInput True if this block has a extra input.
     * @private
     * @this Blockly.Block
     */
    updateShape_: function(extraInput) {

        var opExist = this.getInput(this.operatorValueName);
        var inExist = this.getInput(this.inputValueName);

        this.inputValueShown = extraInput;
        // Add or remove a Value Input.
        if (extraInput) {
            if (!opExist) {
                this.appendDummyInput(this.operatorValueName).setAlign(Blockly.ALIGN_CENTRE).appendField("=");
            }
            if (!inExist) {
                this.appendValueInput(this.inputValueName)
            }
        } else {
            if (inExist) {
                this.removeInput(this.inputValueName);
            }
            if (opExist) {
                this.removeInput(this.operatorValueName);
            }
        }
    },

    updateUnit : function(keyword) {
        var unitsExist = this.getInput(this.unitsInputName);
        if(unitsExist)
            this.removeInput(this.unitsInputName);

        if(!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units) ) {
            this.appendDummyInput(this.unitsInputName).appendField(keyword.units);
        }
    },



    /**
     * Create XML to represent whether the 'divisorInput' should be present.
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function() {
        var container = document.createElement('mutation');
        //attribute name must be lower case
        container.setAttribute('input_shown', this.inputValueShown);
        return container;
    },
    /**
     * Parse XML to restore the 'divisorInput'.
     * @param {!Element} xmlElement XML storage element.
     * @this Blockly.Block
     */
    domToMutation: function(xmlElement) {
        //attribute name must be lower case
        var inputValueShown = (xmlElement.getAttribute('input_shown') == 'true');
        this.updateShape_(inputValueShown);
    }


};


/**
 * Define the python generation function for yadoms_affect_keyword block
 * @param block The block
 * @return {string}
 */
Blockly.Python['yadoms_affect_keyword'] = function(block) {
    var dropdown_keyword = block.getSelectedKeyword();
    var value = Blockly.Python.valueToCode(block, block.inputValueName, Blockly.Python.ORDER_RELATIONAL) || '0';
    return 'yadoms.writeKeyword(' + dropdown_keyword + ',' + value + ')\n';
};





/**
 * Define a "FOR" mutator block for yadoms_logic_compare_is
 */
Blockly.Blocks['yadoms_logic_compare_is_mutator_for'] = {
    init: function() {
        this.setColour(210);
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(false, "null");
        this.setTooltip('');
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_logic_compare_is.for.title_long"));
        //noinspection JSValidateTypes
        this.contextMenu = false;
    }
};
/**
 * Define a "AT" mutator block for yadoms_logic_compare_is
 */
Blockly.Blocks['yadoms_logic_compare_is_mutator_at'] = {
    init: function() {
        this.setColour(210);
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(false, "null");
        this.setTooltip('');
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_logic_compare_is.at.title_long"));
        //noinspection JSValidateTypes
        this.contextMenu = false;
    }
};
/**
 * Define the base mutator block for yadoms_logic_compare_is
 */
Blockly.Blocks['yadoms_logic_compare_is_mutator'] = {
    init: function() {
        this.setColour(Blockly.Blocks.logic.HUE);
        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_logic_compare_is.title"));
        this.appendStatementInput('STACK');
        //noinspection JSValidateTypes
        this.contextMenu = false;
    }
};


/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_compare_is'] = {
    unitsInputName : "units",
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(210);
        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_logic_compare_is.title"));
        this.appendValueInput("A");
        this.appendValueInput("B").appendField(new Blockly.FieldDropdown([['=', 'EQ']]), "OP");
        this.setInputsInline(true);
        this.setOutput(true, "Boolean");

        this.setMutator(new Blockly.Mutator(['yadoms_logic_compare_is_mutator_for', 'yadoms_logic_compare_is_mutator_at']));

        var thisBlock = this;
        this.setTooltip(function() {
            var op = thisBlock.getFieldValue('OP');
            var TOOLTIPS = {
                'EQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_EQ,
                'NEQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_NEQ,
                'LT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LT,
                'LTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LTE,
                'GT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GT,
                'GTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GTE
            };


            return TOOLTIPS[op];
        });


        this.isForMutator = false;
        this.isAtMutator = false;

        /**
         * Method which update the operator list, according to the connected types
         */
        this.updateOperator = function(newCheck) {
            var selectedOperators = [[]];
            if(newCheck == "String") {
                selectedOperators = Blockly.Yadoms.StringOperators_;
            } else if(newCheck == "Boolean") {
                selectedOperators = Blockly.Yadoms.BooleanOperators_;
            } else if(newCheck == "Number") {
                selectedOperators = Blockly.Yadoms.NumberOperators_;
            } else if(newCheck == "Any") {
                selectedOperators = Blockly.Yadoms.NumberOperators_;
            } else {
                //specific types
                selectedOperators = Blockly.Yadoms.BooleanOperators_;
            }

            var operatorDropDown = this.getField_("OP");
            operatorDropDown.menuGenerator_ = selectedOperators;
            operatorDropDown.setValue(selectedOperators[0][1]);
            operatorDropDown.updateTextNode_();
        };

        //apply operator update
        thisBlock.updateOperator(null);

        /**
         * Update the block unit
         * @param keyword The selected keyword
         */
        this.updateUnit = function(keyword) {
            var unitsExist = thisBlock.getInput(this.unitsInputName);
            if(unitsExist)
                thisBlock.removeInput(thisBlock.unitsInputName);

            if(!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units) ) {
                thisBlock.appendDummyInput(thisBlock.unitsInputName).appendField(keyword.units);
            }
        };

        /**
         * Method which make type checks when one of connector changes
         */
        this.onchange = function() {

            //check for A and B connected blocks
            var inputValueA = thisBlock.getInput("A");
            var inputValueB = thisBlock.getInput("B");
            if (!inputValueA || !inputValueB) {
                return;  // Blocks under construction, ignore.
            }
            var blockA = inputValueA.connection.targetBlock();
            var blockB = inputValueB.connection.targetBlock();

            var keywordValue = undefined;
            var deviceValue = undefined;
            var pluginValue = undefined;
            if(blockA && blockA.type == "yadoms_keyword_value") {
                keywordValue = blockA.getSelectedKeyword();
                deviceValue = blockA.getSelectedDevice();
                pluginValue = blockA.getSelectedPlugin();
            }
            if(blockB && blockB.type == "yadoms_keyword_value") {
                keywordValue = blockB.getSelectedKeyword();
                deviceValue = blockB.getSelectedDevice();
                pluginValue = blockB.getSelectedPlugin();
            }

            //update yadoms_enumeration if any
            if (blockA && blockB) {
                if(blockA.type == "yadoms_keyword_value" && blockB.type == "yadoms_enumeration") {
                    blockB.updateEnumeration(keywordValue, deviceValue, pluginValue);
                } else if(blockB.type == "yadoms_keyword_value" && blockA.type == "yadoms_enumeration") {
                    blockA.updateEnumeration(keywordValue, deviceValue, pluginValue);
                }
            }

            //update units
            if(keywordValue != undefined)
                thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keywordValue]);
            else
                thisBlock.updateUnit(undefined);


            //update checks
            if (blockA) {
                if(inputValueB.connection.check_ != blockA.outputConnection.check_) {
                    inputValueB.setCheck(blockA.outputConnection.check_);
                    thisBlock.updateOperator(blockA.outputConnection.check_);
                }
            } else if (blockB) {
                if(inputValueA.connection.check_ != blockB.outputConnection.check_) {
                    inputValueA.setCheck(blockB.outputConnection.check_);
                    thisBlock.updateOperator(blockB.outputConnection.check_);
                }
            } else {
                inputValueA.setCheck(null);
                inputValueB.setCheck(null);
            }
        };

    },
    isAtMutator : false,
    isForMutator : false,
    /**
     * Create XML to represent whether the 'divisorInput' should be present.
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function() {
        var container = document.createElement('mutation');
        container.setAttribute('for_mutator', this.isForMutator);
        container.setAttribute('at_mutator', this.isAtMutator);
        return container;
    },
    /**
     * Parse XML to restore the 'divisorInput'.
     * @param {!Element} xmlElement XML storage element.
     * @this Blockly.Block
     */
    domToMutation: function(xmlElement) {
        var forMutator = (xmlElement.getAttribute('for_mutator') == 'true');
        var atMutator = (xmlElement.getAttribute('at_mutator') == 'true');
        this.updateShape_(forMutator, atMutator);
    },

    updateShape_ : function(forMutator, atMutator) {
        // Disconnect all
        if (this.isForMutator) {
            if(this.getInput("duration")) {
                this.removeInput('duration_text');
                this.removeInput('duration');
                this.removeInput('duration_unit');
            }
        }
        if (this.isAtMutator) {
            if(this.getInput("at")) {
                this.removeInput('at_text');
                this.removeInput('at');
            }
        }
        this.isForMutator = false;
        this.isAtMutator = false;

        var durationUnitsEnum =[
            [$.t("blockly.global.durationUnits.seconds"), 'S'],
            [$.t("blockly.global.durationUnits.minutes"), 'M'],
            [$.t("blockly.global.durationUnits.hours"), 'H'],
            [$.t("blockly.global.durationUnits.days"), 'D']
        ];

        if(forMutator) {
            this.appendDummyInput("duration_text").appendField($.t("blockly.blocks.yadoms_logic_compare_is.for.title"));
            this.appendValueInput("duration").setCheck("Number");
            this.appendDummyInput("duration_unit").appendField(new Blockly.FieldDropdown(durationUnitsEnum), "durationUnit");
            this.isForMutator = true;
        } else if(atMutator) {
            this.appendDummyInput("at_text").appendField($.t("blockly.blocks.yadoms_logic_compare_is.at.title"));
            this.appendDummyInput("at")
                .appendTitle(new Blockly.FieldTextInput('00:00', function(text) {
                    if (text.match(/^([01]?[0-9]|2[0-3]):[0-5][0-9]/))
                        return text;
                    return "00:00";
                }), 'TEXT');
            this.isAtMutator = true;
        }


    },
    decompose: function(workspace) {
        var topBlock = Blockly.Block.obtain(workspace, 'yadoms_logic_compare_is_mutator');
        topBlock.initSvg();

        var connection = topBlock.getInput('STACK').connection;

        if(this.isForMutator) {
            var forBlock = Blockly.Block.obtain(workspace, 'yadoms_logic_compare_is_mutator_for');
            forBlock.initSvg();
            connection.connect(forBlock.previousConnection);
        } else if(this.isAtMutator) {
            var atBlock = Blockly.Block.obtain(workspace, 'yadoms_logic_compare_is_mutator_at');
            atBlock.initSvg();
            connection.connect(atBlock.previousConnection);
        }
        return topBlock;
    },
    /**
     * Reconfigure this block based on the mutator dialog's components.
     * @param {!Blockly.Block} containerBlock Root block in mutator.
     * @this Blockly.Block
     */
    compose: function(containerBlock) {
        // Rebuild the block's optional inputs.

        //get the mutator dialog result
        var clauseBlock = containerBlock.getInputTargetBlock('STACK');

        var forMutator = false;
        var atMutator = false;
        if(clauseBlock) {
            if(clauseBlock.type == 'yadoms_logic_compare_is_mutator_for') {
                forMutator = true;
            } else if(clauseBlock.type == 'yadoms_logic_compare_is_mutator_at') {
                atMutator = true;
            }
        }
        this.updateShape_(forMutator, atMutator);
    }
};


/**
 * Define the python generation function for yadoms_logic_compare_is block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['yadoms_logic_compare_is'] = function(block) {

    // Comparison operator.
    var OPERATORS = {
        'EQ': '==',
        'NEQ': '!=',
        'LT': '<',
        'LTE': '<=',
        'GT': '>',
        'GTE': '>='
    };
    var operator = OPERATORS[block.getFieldValue('OP')];
    var order = Blockly.Python.ORDER_RELATIONAL;
    var argument0 = Blockly.Python.valueToCode(block, 'A', order) || '0';
    var argument1 = Blockly.Python.valueToCode(block, 'B', order) || '0';
    var code = '';

    if(block.isForMutator) {
        //TODO Python yadoms_logic_compare_is : FOR mutation generation not implemented");
        assert(false, "Python : yadoms_logic_compare_is : FOR mutation generation not implemented");
    } else if(block.isAtMutator) {
        //TODO Python yadoms_logic_compare_is : AT mutation generation not implemented");
        assert(false, "Python : yadoms_logic_compare_is : AT mutation generation not implemented");
    } else {
        code = argument0 + ' ' + operator + ' ' + argument1;
    }
    return [code, order];
};



/**
 * Define a condition block which is true when a keyword value reach a value
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_compare_become'] = {
    unitsInputName : "units",
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(210);
        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_logic_compare_become.title"));
        this.appendValueInput("A");
        this.appendValueInput("B").appendField(new Blockly.FieldDropdown([['=', 'EQ']]), "OP");
        this.setInputsInline(true);
        this.setOutput(true, "Boolean");

        var thisBlock = this;
        this.setTooltip(function() {
            var op = thisBlock.getFieldValue('OP');
            var TOOLTIPS = {
                'EQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_EQ,
                'NEQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_NEQ,
                'LT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LT,
                'LTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LTE,
                'GT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GT,
                'GTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GTE
            };


            return TOOLTIPS[op];
        });


        /**
         * Method which update the operator list, according to the connected types
         */
        thisBlock.updateOperator = function(newCheck) {
            var selectedOperators = [[]];
            if(newCheck == "String") {
                selectedOperators = Blockly.Yadoms.StringOperators_;
            } else if(newCheck == "Boolean") {
                selectedOperators = Blockly.Yadoms.BooleanOperators_;
            } else if(newCheck == "Number") {
                selectedOperators = Blockly.Yadoms.NumberOperators_;
            } else if(newCheck == "Any") {
                selectedOperators = Blockly.Yadoms.NumberOperators_;
            } else {
                //specific types
                selectedOperators = Blockly.Yadoms.BooleanOperators_;
            }

            var operatorDropDown = this.getField_("OP");
            operatorDropDown.menuGenerator_ = selectedOperators;
            operatorDropDown.setValue(selectedOperators[0][1]);
            operatorDropDown.updateTextNode_();
        };

        //apply operator update
        thisBlock.updateOperator(null);

        /**
         * Update the block unit
         * @param keyword The selected keyword
         */
        this.updateUnit = function(keyword) {
            var unitsExist = thisBlock.getInput(this.unitsInputName);
            if(unitsExist)
                thisBlock.removeInput(thisBlock.unitsInputName);

            if(!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units) ) {
                thisBlock.appendDummyInput(thisBlock.unitsInputName).appendField(keyword.units);
            }
        };

        /**
         * Method which make type checks when one of connector changes
         */
        this.onchange = function() {

            //check for A and B connected blocks
            var inputValueA = thisBlock.getInput("A");
            var inputValueB = thisBlock.getInput("B");
            if (!inputValueA || !inputValueB) {
                return;  // Blocks under construction, ignore.
            }
            var blockA = inputValueA.connection.targetBlock();
            var blockB = inputValueB.connection.targetBlock();

            var keywordValue = undefined;
            var deviceValue = undefined;
            var pluginValue = undefined;
            if(blockA && blockA.type == "yadoms_keyword_value") {
                keywordValue = blockA.getSelectedKeyword();
                deviceValue = blockA.getSelectedDevice();
                pluginValue = blockA.getSelectedPlugin();
            }
            if(blockB && blockB.type == "yadoms_keyword_value") {
                keywordValue = blockB.getSelectedKeyword();
                deviceValue = blockB.getSelectedDevice();
                pluginValue = blockB.getSelectedPlugin();
            }

            //update yadoms_enumeration if any
            if (blockA && blockB) {
                if(blockA.type == "yadoms_keyword_value" && blockB.type == "yadoms_enumeration") {
                    blockB.updateEnumeration(keywordValue, deviceValue, pluginValue);
                } else if(blockB.type == "yadoms_keyword_value" && blockA.type == "yadoms_enumeration") {
                    blockA.updateEnumeration(keywordValue, deviceValue, pluginValue);
                }
            }

            //update units
            if(keywordValue != undefined)
                thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keywordValue]);
            else
                thisBlock.updateUnit(undefined);

            //update checks
            if (blockA) {
                if(inputValueB.connection.check_ != blockA.outputConnection.check_) {
                    inputValueB.setCheck(blockA.outputConnection.check_);
                    thisBlock.updateOperator(blockA.outputConnection.check_);
                }
            } else if (blockB) {
                if(inputValueA.connection.check_ != blockB.outputConnection.check_) {
                    inputValueA.setCheck(blockB.outputConnection.check_);
                    thisBlock.updateOperator(blockB.outputConnection.check_);
                }
            } else {
                inputValueA.setCheck(null);
                inputValueB.setCheck(null);
            }
        };
    }
};


/**
 * Define the python generation function for yadoms_logic_compare_become block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['yadoms_logic_compare_become'] = function(block) {

    // Comparison operator.
    var OPERATORS = {
        'EQ': '==',
        'NEQ': '!=',
        'LT': '<',
        'LTE': '<=',
        'GT': '>',
        'GTE': '>='
    };
    var operator = OPERATORS[block.getFieldValue('OP')];
    var order = Blockly.Python.ORDER_RELATIONAL;
    var argument0 = Blockly.Python.valueToCode(block, 'A', order) || '0';
    var argument1 = Blockly.Python.valueToCode(block, 'B', order) || '0';
    var code = argument0 + ' ' + operator + ' ' + argument1;

    //TODO Python yadoms_logic_compare_become generation not implemented");
    return [code, order];
};








Blockly.Blocks['yadoms_enumeration'] = {
    enumerationDropDownName : "enumerationList",
    currentEnumerationTypeName : "",
    init: function() {
        var enumDropDown = new Blockly.FieldDropdown([[$.t("blockly.blocks.yadoms_enumeration.title"), "enumeration"]]);

        this.setHelpUrl('http://www.example.com/');
        this.setColour(20);
        this.appendDummyInput()
            .appendField(enumDropDown, this.enumerationDropDownName);
        this.setOutput(true, "");
        this.setTooltip('');

        this.getSelectedEnumValue = function() {
            return enumDropDown.getValue();
        };

        //add event fire value changed
        enumDropDown.setValue =function(newValue) {
            var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
            if(!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
                this.changeHandler_(this.getValue());
            return result;
        };
    },

    updateEnumeration : function(keywordValue, deviceValue, pluginValue) {
        //check keyword value is not null
        if (!isNullOrUndefined(keywordValue)) {

            //check that keyword data is available
            var pluginData = Blockly.Yadoms.data.plugins[pluginValue];
            var keywordData = Blockly.Yadoms.data.keywords[keywordValue];
            if(!isNullOrUndefined(keywordData) && keywordData.type == "enum") {
                var typeInfo = keywordData.typeInfo;
                if( !isNullOrUndefined(typeInfo) && !isNullOrUndefined(typeInfo.name) && !isNullOrUndefined(typeInfo.values) ) {
                    if(this.currentEnumerationTypeName != typeInfo.name) {

                        this.changeOutput("enum_" + typeInfo.name);
                        //all is OK, this is a new enum, ask for translation
                        var translatedEnum = [];
                        $.each(typeInfo.values, function (index, value) {
                            var trad = $.t(pluginData.type + ":enumerations." + typeInfo.name + "." + value);
                            translatedEnum.push([trad, value]);
                        });
                        //apply new dropdown list
                        this.currentEnumerationTypeName = typeInfo.name;
                        var enumDropDown = this.getField_(this.enumerationDropDownName);
                        enumDropDown.refresh(translatedEnum, enumDropDown.getValue());
                    }
                }
            }
        }
    }
};



/**
 * Define the python generation function for yadoms_enumeration block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['yadoms_enumeration'] = function(block) {
    var code = Blockly.Python.quote_(block.getSelectedEnumValue());
    return [code, Blockly.Python.ORDER_ATOMIC];
};



/**
 * Define the yadoms_log block (allow logging into yaomds.log)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_log'] = {
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(160);
        this.appendValueInput("LogContent")
            .setCheck("String")
            .appendField( $.t("blockly.blocks.yadoms_log.title") )
            .appendField(new Blockly.FieldDropdown([[$.t("blockly.blocks.yadoms_log.logLevel.debug"), "debug"], [$.t("blockly.blocks.yadoms_log.logLevel.info"), "info"], [$.t("blockly.blocks.yadoms_log.logLevel.warning"), "warning"], [$.t("blockly.blocks.yadoms_log.logLevel.error"), "error"], [$.t("blockly.blocks.yadoms_log.logLevel.fatal"), "fatal"]]), "LogLevel");
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip('');
    }
};

/**
 * Define the python generation function for yadoms_log block
 * @param block The block
  */
Blockly.Python['yadoms_log'] = function(block) {
    var value_name = Blockly.Python.valueToCode(block, 'LogContent', Blockly.Python.ORDER_ATOMIC) || '\'\'';
    var dropdown_loglevel = Blockly.Python.quote_(block.getFieldValue('LogLevel'));
    return 'yadoms.log(' + dropdown_loglevel + ',' + value_name + ')\n';
};


/**
 * Define the yadoms_sleep block (allow current rule to sleep)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_sleep'] = {
    init: function() {

        var durationUnitsEnum =[
            [$.t("blockly.global.durationUnits.seconds"), 'S'],
            [$.t("blockly.global.durationUnits.minutes"), 'M'],
            [$.t("blockly.global.durationUnits.hours"), 'H'],
            [$.t("blockly.global.durationUnits.days"), 'D']
        ];

        this.setHelpUrl('http://www.example.com/');
        this.setColour(20);
        this.appendValueInput("Time")
            .setCheck("Number")
            .appendField( $.t("blockly.blocks.yadoms_sleep.title") );
        this.appendDummyInput()
            .appendField(new Blockly.FieldDropdown(durationUnitsEnum), "timeUnit");
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip('');
    }
};



/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python['yadoms_sleep'] = function(block) {
    var value_time = Blockly.JavaScript.valueToCode(block, 'Time', Blockly.JavaScript.ORDER_ATOMIC);
    var dropdown_name = block.getFieldValue('timeUnit');

    var valueInSeconds = value_time;
    switch(dropdown_name)
    {
        case 'D':
            valueInSeconds = value_time * 60 * 60 * 24;
            break;
        case 'H':
            valueInSeconds = value_time * 60 * 60;
            break;
        case 'M':
            valueInSeconds = value_time * 60;
            break;
        case 'S':
            valueInSeconds = value_time;
            break;
    }

    return 'time.sleep(' + valueInSeconds + ')';
};






/**
 * Define a simple "if then" block (not mutable)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_controls_if'] = {
    /**
     * Block for if/elseif/else condition.
     * @this Blockly.Block
     */
    init: function() {
        this.setHelpUrl(Blockly.Msg.CONTROLS_IF_HELPURL);
        this.setColour(210);
        this.appendValueInput('IF0')
            .setCheck('Boolean')
            .appendField("Conditions");
        this.appendStatementInput('DO0')
            .appendField("Actions");
        this.setPreviousStatement(false);
        this.setNextStatement(false);

        // Assign 'this' to a variable for use in the tooltip closure below.
        var thisBlock = this;
        this.setTooltip(function() {
            if (!thisBlock.elseifCount_ && !thisBlock.elseCount_) {
                return Blockly.Msg.CONTROLS_IF_TOOLTIP_1;
            } else if (!thisBlock.elseifCount_ && thisBlock.elseCount_) {
                return Blockly.Msg.CONTROLS_IF_TOOLTIP_2;
            } else if (thisBlock.elseifCount_ && !thisBlock.elseCount_) {
                return Blockly.Msg.CONTROLS_IF_TOOLTIP_3;
            } else if (thisBlock.elseifCount_ && thisBlock.elseCount_) {
                return Blockly.Msg.CONTROLS_IF_TOOLTIP_4;
            }
            return '';
        });
        this.elseifCount_ = 0;
        this.elseCount_ = 0;
    }

};

/**
 * Define the python generation function for yadoms_controls_if block
 * @param block The block
 */
Blockly.Python['yadoms_controls_if'] = function(block) {
    var argument = Blockly.Python.valueToCode(block, 'IF0', Blockly.Python.ORDER_NONE) || 'False';
    var branch = Blockly.Python.statementToCode(block, 'DO0') || Blockly.Python.PASS;
    return 'if ' + argument + ':\n' + branch;
};






/**
 * Define a block for sending simple notifications (message)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_notification_simple'] = {
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(0);

        //recipient list is static (do not need to update it on each dropdown click)
        var recipientList = Blockly.Yadoms.LoadRecipients_();
        if(recipientList == null || recipientList == undefined || recipientList.length == 0) {
            this.setDisabled(true);
            recipientList = [["invalid" , "invalid"]];
        }

        this.appendDummyInput()
            .appendField("Notifier")
            .appendField(new Blockly.FieldDropdown(recipientList), "recipient");
        this.appendValueInput("Message")
            .setCheck("String")
            .appendField("Message");
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip('');
    }
};


/**
 * Define a block for sending advanced notifications (subject, message)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_notification_advanced'] = {
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(0);

        //recipient list is static (do not need to update it on each dropdown click)
        var recipientList = Blockly.Yadoms.LoadRecipients_();
        if(recipientList == null || recipientList == undefined || recipientList.length == 0) {
            this.setDisabled(true);
            recipientList = [["invalid" , "invalid"]];
        }
        this.appendDummyInput()
            .appendField("Notifier")
            .appendField(new Blockly.FieldDropdown(recipientList), "recipient");
        this.appendValueInput("Subject")
            .setCheck("String")
            .appendField("Sujet");
        this.appendValueInput("Message")
            .setCheck("String")
            .appendField("Message");
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip('');
    }
};



/**
 *
 *
 *
 *  Validation
 *
 *
 *  This feature check is a workspace is completely filled
 *
 *
 *
 */


/**
 * Create the validation subclass
 * @constructor
 */
Blockly.Validation = function() {
};

/**
 * Define the currently list of "error validated" blocks
 * @type {Array}
 */
Blockly.Validation.listErroneousBlocks = [];


/**
 * Define a block as invalid
 * @param block The block to invalidate
 */
Blockly.Validation.setBlockInvalid = function(block) {
    if(block != null && block != undefined && $.isFunction(block.addError)) {
        block.addError();
        Blockly.Validation.listErroneousBlocks.push(block);
    }
};

/**
 * Clear the invalidated blocks list
 */
Blockly.Validation.clearInvalidBlocks =  function() {
    $.each(Blockly.Validation.listErroneousBlocks, function(index, value) {
        if(value != null && value != undefined &&$.isFunction(value.removeError)) {
            value.removeError();
        }
    });

    //clear the list
    Blockly.Validation.listErroneousBlocks.splice(0,Blockly.Validation.listErroneousBlocks.length);
};

/**
 * Check if a block is valid (all visible inputs are filled)
 * @param block The block to check
 * @returns {boolean} true if block is valid, false if invalid
 */
Blockly.Validation.isBlockValid =  function(block) {

    if(block != null && block != undefined) {
        var currentBlockValid = true;
        $.each(block.inputList, function(index, subBlock) {
            if(subBlock.connection != null && subBlock.isVisible() ){
                if(subBlock.connection.targetConnection == null || subBlock.connection.targetConnection == undefined) {
                    Blockly.Validation.setBlockInvalid(subBlock.sourceBlock_);
                    return currentBlockValid = false;
                } else {
                    if(!Blockly.Validation.isBlockValid(subBlock.connection.targetConnection.sourceBlock_))
                        return currentBlockValid = false;
                }
            }
        });
        return currentBlockValid;
    }
    return false;
};


/**
 * Add the error shpae to the block
 */
Blockly.BlockSvg.prototype.addError = function() {
    Blockly.addClass_(/** @type {!Element} */ (this.svgGroup_),
        'blocklySelectedError');
    // Move the selected block to the top of the stack.
    if(!isNullOrUndefined(this.svgGroup_.parentNode))
        this.svgGroup_.parentNode.appendChild(this.svgGroup_);
};

/**
 * Remove the error shape of a block
 */
Blockly.BlockSvg.prototype.removeError = function() {
    Blockly.removeClass_(/** @type {!Element} */ (this.svgGroup_),
        'blocklySelectedError');
};

/**
 * Initialize the workspace to allow validation
 * @param maxTopBlocks The maximum number of topblocks allowed
 * @constructor
 */
Blockly.Validation.Init = function(maxTopBlocks) {
    if(maxTopBlocks != undefined && $.isNumeric(maxTopBlocks))
        Blockly.Validation.maxTopBlocks_ = maxTopBlocks;

    Blockly.addChangeListener( function() {
        Blockly.Validation.clearInvalidBlocks();
    });
};

/**
 * The number of allowed top blocks
 * @type {number}
 * @private
 */
Blockly.Validation.maxTopBlocks_ = 1;

/**
 * Start a workspace validation
 * @param workspace The workspace to validate
 * @param callback The callback for the validation result
 */
Blockly.Validation.validate = function(workspace, callback) {

    if(workspace.topBlocks_.length > Blockly.Validation.maxTopBlocks_) {

        $.each(workspace.topBlocks_, function(index, value) {
            Blockly.Validation.setBlockInvalid(value);
        });
        callback(false, "Only " + Blockly.Validation.maxTopBlocks_ + " max top block is allowed");
    }
    else {
        var result = true;
        $.each(workspace.topBlocks_, function(index, topBlock) {
            if(!Blockly.Validation.isBlockValid(topBlock))
                result = false;
        });

        if(result)
            callback(true, "Workspace is valid");
        else
            callback(false, "Workspace is not valid");
    }
};

/**
 * Start the mainWorkspace validation
 * @param callback The callback for the validation result
 */
Blockly.Validation.validateMainWorkspace = function(callback) {
    Blockly.Validation.validate(Blockly.mainWorkspace, callback);
};




