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


var NumberOperators = Blockly.RTL ? [
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

var StringOperators = Blockly.RTL ? [
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

var BooleanOperators = Blockly.RTL ? [
    ['=', 'EQ'],
    ['\u2260', 'NEQ']
] : [
    ['=', 'EQ'],
    ['\u2260', 'NEQ']
];

function LoadData() {

    var result = {
        plugins : {},
        devices : {},
        keywords : {},
        recipients : {}
    };

    PluginInstanceManager.getAll(function (list) {
        $.each(list, function (key, plugin) {
            result.plugins[key] = plugin;
        });
    }, false);

    DeviceManager.getAll(function (list) {
        $.each(list, function (deviceKey, device) {
            result.devices[device.id] = device;
        });
    }, false);

    KeywordManager.getAll(function (list) {
        $.each(list, function (keywordKey, keyword) {
            result.keywords[keyword.id] = keyword;
        });
    }, false);

    RecipientManager.list(function (list) {
        $.each(list, function (recipientKey, recipient) {
            result.recipients[recipient.id] = recipient;
        });
    }, false);

    return result;
}



Blockly.Yadoms = function() {
};

Blockly.Yadoms.Initialize = function() {
    Blockly.Yadoms.data = LoadData();
}



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
    console.log("Checking Plugin " + plugin.name + " ...");

    var currentPluginMatch = false;

    $.each(Blockly.Yadoms.data.devices, function(index, device) {
        if(device.pluginId == plugin.id) {
            if (Blockly.Yadoms.DeviceHasKeywordMatching_(device, canWrite, allowedKeywordTypes) == true) {
                currentPluginMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
    });

    if(currentPluginMatch)
        console.log("Plugin " + plugin.name + " MATCH");
    else
        console.log("Plugin " + plugin.name + " do not match");
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

    console.log("---- Checking Device " + device.name + " ...");
    var currentDeviceMatch = false;

    $.each(Blockly.Yadoms.data.keywords, function(index, keyword) {
        if(keyword.deviceId == device.id) {
            if (Blockly.Yadoms.KeywordMatching_(keyword, canWrite, allowedKeywordTypes) == true) {

                currentDeviceMatch = true;
                return false; //break the $.each, but do not return function
            }
        }
    });

    if(currentDeviceMatch)
        console.log("---- Device " + device.name + " MATCH");
    else
        console.log("---- Device " + device.name + " do not match");
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

    console.log("-------- Checking Keyword " + keyword.name + " ...");

    if(canWrite == true)
        console.log("-------- Checking Keyword " + keyword.name + " for writing");

    //filter on readonly
    if(canWrite && keyword.accessMode != "getset") {
        console.log("-------- Keyword " + keyword.name + " do not match : access mode is not getset for writting : " + keyword.accessMode );
        return false;
    }

    if(allowedKeywordTypes != null && allowedKeywordTypes != undefined) {
        var res = $.inArray(keyword.type, allowedKeywordTypes) != -1;
        if(res == true)
            console.log("-------- Keyword " + keyword.name + " MATCH type : " + keyword.type );
        else
            console.log("-------- Keyword " + keyword.name + " do not type : " + keyword.type );
        return res;
    }
    console.log("-------- Keyword " + keyword.name + " MATCH (no type filter)");
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
 * Return an array of plugins (Array: [name, id])
 * @param canWrite If true, only plugin with writable keywords are allowed
 * @param allowedKeywordTypes Array of allowed keyword types
 * @return {Array} The plugin array
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
 */
Blockly.Yadoms.GetBlocklyType_ = function (yadomsKeywordType) {
    switch(yadomsKeywordType.toLowerCase()){
        case "numeric":
            return "Number";
        case "string":
        case "json":
            return "String";
        case "bool":
            return "Boolean";
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
 * @constructor
 */
Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection = function(thisBlock, canWrite, allowedKeywordTypes, callbackKeywordSelectionChanged) {
    var pluginDd, deviceDd, keywordDd;

    //plugin list is static (do not need to update it on each dropdown click)
    var pList = Blockly.Yadoms.LoadPlugins_(canWrite, allowedKeywordTypes);
    if(pList == null || pList == undefined || pList.length == 0) {
        thisBlock.setDisabled(true);
        pList = [["invalid" , "invalid"]];
    }

    //create the plugin dropdown
    pluginDd = new Blockly.FieldDropdown(pList, function(plugin) {
        deviceDd.refresh(Blockly.Yadoms.LoadDevices_(plugin, canWrite, allowedKeywordTypes));
    });

    deviceDd = new Blockly.FieldDropdown(function() {
        return Blockly.Yadoms.LoadDevices_(pluginDd.getValue(), canWrite, allowedKeywordTypes);
    }, function(device) {
        keywordDd.refresh(Blockly.Yadoms.LoadKeywords_(device, canWrite, allowedKeywordTypes));
    });

    keywordDd = new Blockly.FieldDropdown(function() {
        return Blockly.Yadoms.LoadKeywords_(deviceDd.getValue(), canWrite, allowedKeywordTypes);
    }, function(keyword) {
        var type = Blockly.Yadoms.GetBlocklyType_(Blockly.Yadoms.data.keywords[keyword].type);
        Blockly.Yadoms.UpdateBlockColour_(thisBlock, type);
        callbackKeywordSelectionChanged(keyword, type);
    });

    thisBlock.appendDummyInput()
        .appendField(pluginDd, "Plugin")
        .appendField(deviceDd, "Device")
        .appendField(keywordDd, "Keyword");
};

Blockly.Yadoms.InitializeYadomsKeywordSelection = function(thisBlock) {

    var pluginDd = thisBlock.getField_("Plugin");
    if(pluginDd != null && pluginDd != undefined)
        pluginDd.changeHandler_(pluginDd.getValue());
};








/**
 * Dropdown refresh method
 * The function update the dropdown values, select the first one and call the change handler
 * -> Main blockly hack (not using only public memebers)
 * @param data
 */
Blockly.FieldDropdown.prototype.refresh = function(data) {
    if(data != undefined && data.length >0) {
        this.menuGenerator_ = data;
        this.setValue(data[0][1]);
        this.updateTextNode_();
        this.changeHandler_(this.getValue());
    }
};

/**
 * Define a block which allow to read a keyword value
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_keyword_value'] = {
    unitsInputName : "units",
    init: function () {

        //set custom block parameters
        this.setInputsInline(true);
        this.setOutput(true);
        this.setTooltip('');
        this.setHelpUrl('http://www.example.com/');

        var thisBlock = this;
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(thisBlock, false, ["numeric", "string", "bool"], function (keyword, keywordType) {
            if (keywordType == null || keywordType == undefined)
                thisBlock.changeOutput("null"); //any type allowed
            else
                thisBlock.changeOutput(keywordType);
            thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keyword]);
        });

        Blockly.Yadoms.InitializeYadomsKeywordSelection(thisBlock);
    },
    updateUnit : function(keyword) {
        var unitsExist = this.getInput(this.unitsInputName);
        if(unitsExist)
            this.removeInput(this.unitsInputName);

        if(!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units) ) {
            this.appendDummyInput(this.unitsInputName).appendField(keyword.units);
        }
    }
};


/**
 * Define a block which allow to change a keyword value
 * @type {{inputValueName: string, operatorValueName: string, init: Function, updateShape_: Function}}
 */
Blockly.Blocks['yadoms_affect_keyword'] = {
    inputValueName : "Value",
    operatorValueName : "operator",
    unitsInputName : "units",
  init: function() {
      this.setHelpUrl('http://www.example.com/');
      this.setInputsInline(true);
      this.setPreviousStatement(true, "null");
      this.setNextStatement(true, "null");
      this.setTooltip('');

      this.appendDummyInput()
          .appendField("Set");
      var thisBlock = this;
      Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, true, ["numeric", "string", "bool", "nodata"], function (keyword, keywordType) {
          if (keywordType == null || keywordType == undefined) {
              thisBlock.updateShape_(false);
          }
          else {
              thisBlock.updateShape_(true);
              thisBlock.getInput(thisBlock.inputValueName).setCheck(keywordType);
          }
          thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keyword]);
      });

      Blockly.Yadoms.InitializeYadomsKeywordSelection(thisBlock);
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
    }



};

/**
 * Define a condition block which is true when a keyword value reach a value
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_compare_become'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(210);
    this.appendDummyInput()
        .appendField("Become");
    this.appendValueInput("A");
    this.appendValueInput("B").appendField(new Blockly.FieldDropdown([['=', 'EQ']]), "OP");
    this.setInputsInline(false);
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
         selectedOperators = StringOperators;
      } else if(newCheck == "Boolean") {
         selectedOperators = BooleanOperators;
      } else if(newCheck == "Number") {
         selectedOperators = NumberOperators;
      } else if(newCheck == "µAny") {
         selectedOperators = NumberOperators;
      } else {
         //specific types
         selectedOperators = BooleanOperators;
      }

      var operatorDropDown = this.getField_("OP");
      operatorDropDown.menuGenerator_ = selectedOperators;
      operatorDropDown.setValue(selectedOperators[0][1]);
	  operatorDropDown.updateTextNode_();
   };

   //apply operator update
   thisBlock.updateOperator(null);


   /**
   * Method which make type checks when one of connector changes
   */
	thisBlock.onchange = function() {
       var inputA = this.getInput('A');
       var inputB = this.getInput('B');
       if (!inputA || !inputB) {
         return;  // Block under construction, ignore.
       }
       var blockA = inputA.connection.targetBlock();
       var blockB = inputB.connection.targetBlock();
       if (blockA) {
          if(inputB.connection.check_ != blockA.outputConnection.check_) {
            inputB.setCheck(blockA.outputConnection.check_);
            this.updateOperator(blockA.outputConnection.check_);
          }
       } else if (blockB) {
          if(inputA.connection.check_ != blockB.outputConnection.check_) {
            inputA.setCheck(blockB.outputConnection.check_);
            this.updateOperator(blockB.outputConnection.check_);
          }
       } else {
         inputA.setCheck(null);
         inputB.setCheck(null);
       }
   };
  }
};


/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_compare_is'] = {
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(210);
        this.appendDummyInput()
            .appendField("Is");
        this.appendValueInput("A");
        this.appendValueInput("B").appendField(new Blockly.FieldDropdown([['=', 'EQ']]), "OP");
        this.setInputsInline(false);
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
                selectedOperators = StringOperators;
            } else if(newCheck == "Boolean") {
                selectedOperators = BooleanOperators;
            } else if(newCheck == "Number") {
                selectedOperators = NumberOperators;
            } else if(newCheck == "µAny") {
                selectedOperators = NumberOperators;
            } else {
                //specific types
                selectedOperators = BooleanOperators;
            }

            var operatorDropDown = this.getField_("OP");
            operatorDropDown.menuGenerator_ = selectedOperators;
            operatorDropDown.setValue(selectedOperators[0][1]);
            operatorDropDown.updateTextNode_();
        };

        //apply operator update
        thisBlock.updateOperator(null);


        /**
         * Method which make type checks when one of connector changes
         */
        thisBlock.onchange = function() {
            var inputA = this.getInput('A');
            var inputB = this.getInput('B');
            if (!inputA || !inputB) {
                return;  // Block under construction, ignore.
            }
            var blockA = inputA.connection.targetBlock();
            var blockB = inputB.connection.targetBlock();
            if (blockA) {
                if(inputB.connection.check_ != blockA.outputConnection.check_) {
                    inputB.setCheck(blockA.outputConnection.check_);
                    this.updateOperator(blockA.outputConnection.check_);
                }
            } else if (blockB) {
                if(inputA.connection.check_ != blockB.outputConnection.check_) {
                    inputA.setCheck(blockB.outputConnection.check_);
                    this.updateOperator(blockB.outputConnection.check_);
                }
            } else {
                inputA.setCheck(null);
                inputB.setCheck(null);
            }
        };
    }
};
/**
 * Define a condition block which is true when a keyword match a value for a time (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_compare_is_for'] = {
    init: function() {

        var durationUnitsEnum =[
            ['heures', 'H'],
            ['minutes', 'M'],
            ['secondes', 'S'],
            ['jours', 'D']
        ];


        this.setHelpUrl('http://www.example.com/');
        this.setColour(210);
        this.appendDummyInput()
            .appendField("Is");
        this.appendValueInput("A");
        this.appendDummyInput()
            .appendField(new Blockly.FieldDropdown([['=', 'EQ']]), "OP");
        this.appendValueInput("B");
        this.appendDummyInput()
            .appendField("For");
        this.appendValueInput("duration")
            .setCheck("Number");
        this.appendDummyInput()
            .appendField(new Blockly.FieldDropdown(durationUnitsEnum), "durationUnit");


        this.setInputsInline(false);
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
                selectedOperators = StringOperators;
            } else if(newCheck == "Boolean") {
                selectedOperators = BooleanOperators;
            } else if(newCheck == "Number") {
                selectedOperators = NumberOperators;
            } else if(newCheck == "µAny") {
                selectedOperators = NumberOperators;
            } else {
                //specific types
                selectedOperators = BooleanOperators;
            }

            var operatorDropDown = this.getField_("OP");
            operatorDropDown.menuGenerator_ = selectedOperators;
            operatorDropDown.setValue(selectedOperators[0][1]);
            operatorDropDown.updateTextNode_();
        };

        //apply operator update
        thisBlock.updateOperator(null);


        /**
         * Method which make type checks when one of connector changes
         */
        thisBlock.onchange = function() {
            var inputA = this.getInput('A');
            var inputB = this.getInput('B');
            if (!inputA || !inputB) {
                return;  // Block under construction, ignore.
            }
            var blockA = inputA.connection.targetBlock();
            var blockB = inputB.connection.targetBlock();
            if (blockA) {
                if(inputB.connection.check_ != blockA.outputConnection.check_) {
                    inputB.setCheck(blockA.outputConnection.check_);
                    this.updateOperator(blockA.outputConnection.check_);
                }
            } else if (blockB) {
                if(inputA.connection.check_ != blockB.outputConnection.check_) {
                    inputA.setCheck(blockB.outputConnection.check_);
                    this.updateOperator(blockB.outputConnection.check_);
                }
            } else {
                inputA.setCheck(null);
                inputB.setCheck(null);
            }
        };
    }
};


//
//Blockly.Blocks['switch_list'] = {
//  init: function() {
//    this.setHelpUrl('http://www.example.com/');
//    this.setColour(20);
//    this.appendDummyInput()
//        .appendField(new Blockly.FieldDropdown(switchType), "switch_type");
//    this.setOutput(true, "switch_type");
//    this.setTooltip('');
//  }
//};
//



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




