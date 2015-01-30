/**

Keyword value 
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#r53dn5


Keyword setter
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#rttajf

Operator
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#g3gzx9

Type switch
https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#3tdeuk

*/

 // var switchType = [["on", "ON"], ["off", "OFF"]];




//function ConfigureYadomsKeywordSetBlock(self, pluginDropDownName, deviceDropDownName, keywordDropDownName, valueInputName, data) {
//	ConfigureYadomsKeywordCommonBlock(self, pluginDropDownName, deviceDropDownName, keywordDropDownName, function(type) {
//	  if(type == undefined)
//		  self.getInput(valueInputName).setCheck("null");
//	  else
//		  self.getInput(valueInputName).setCheck(type);
//	}, data)
//}
//
//function ConfigureYadomsKeywordOutputBlock(self, pluginDropDownName, deviceDropDownName, keywordDropDownName, data) {
//	ConfigureYadomsKeywordCommonBlock(self, pluginDropDownName, deviceDropDownName, keywordDropDownName, function(type) {
//
//	  if(type == undefined)
//		  self.changeOutput("null");
//	  else
//		  self.changeOutput(type);
//	}, data)
//}
//
//function UpdateBlockColour(self, type) {
//	  if(type == "String")
//		  self.setColour(160);
//	  else if(type == "Boolean")
//		  self.setColour(210);
//	  else if(type == "Number")
//		  self.setColour(230);
//	  else if(type == "null" ||  type == null || type == undefined)
//		  self.setColour(230);
//     else
//        self.setColour(20);
//}

//Blockly.FieldDropdown.prototype.updateDropDown = function (newList, superKey1, superKey2) {
//		this.menuGenerator_ = newList;
//		this.setValue(newList[0][1]);
//		this.updateTextNode_();
//		this.changeHandler_(this.getValue(), superKey1, superKey2);
//
//}

//function ConfigureYadomsKeywordCommonBlock(self, pluginDropDownName, deviceDropDownName, keywordDropDownName, callbackOnTypeChanged, data) {
//
//	//self.getField_(pluginDropDownName).changeHandler_ = function(selectedPluginKey) {
//	//	/*var deviceList = [];
//	//	for(var device in data[selectedPluginKey].devices) {
//	//		deviceList.push([data[selectedPluginKey].devices[device].friendlyName, device]);
//	//	}
//	//	var deviceDropDown = self.getField_(deviceDropDownName);
//    //  deviceDropDown.updateDropDown(deviceList, selectedPluginKey);*/
////
//    //}
//    //
////
//   //elf.getField_(deviceDropDownName).changeHandler_ = function(selectedDeviceKey, selectedPluginKey) {
//    //    /* if(selectedPluginKey == undefined)
//	//	  selectedPluginKey = self.getField_(pluginDropDownName).getValue();
//    //
//    //  var keywordList = [];
//    //  for(var keyword in data[selectedPluginKey].devices[selectedDeviceKey].keywords) {
//    //     keywordList.push([data[selectedPluginKey].devices[selectedDeviceKey].keywords[keyword].friendlyName, keyword]);
//    //  }
//    //  var keywordDropDown = self.getField_(keywordDropDownName);*/
////
//    //  //keywordDropDown.updateDropDown(keywordList, selectedDeviceKey, selectedPluginKey);
//    //
//    //}
//    //
//    //self.getField_(keywordDropDownName).changeHandler_ = function(selectedKeywordKey, selectedDeviceKey, selectedPluginKey) {
//	//  /*if(selectedPluginKey == undefined)
//	//	selectedPluginKey = self.getField_(pluginDropDownName).getValue();
//	//  if(selectedDeviceKey == undefined)
//	//	selectedDeviceKey = self.getField_(deviceDropDownName).getValue();
//    //
//	//  var type = data[selectedPluginKey].devices[selectedDeviceKey].keywords[selectedKeywordKey].expectedType;
//	//  callbackOnTypeChanged(type);
//	//
//    // UpdateBlockColour(self, type);*/
//    //};
//
//    //load plugins
//    var pluginDropDown = self.getField_(pluginDropDownName);
//    var pluginList = [[]];
//    //pluginDropDown.updateDropDown(pluginList);
//
//    PluginInstanceManager.getAll(function (list) {
//        $.each(list, function (key, value) {
//            pluginList.push([value.name, value.id]);
//        });
//        pluginDropDown.updateDropDown(pluginList);
//    }, true);
//}



function LoadPlugins() {

    var pluginList = [];
    PluginInstanceManager.getAll(function (list) {
        $.each(list, function (key, value) {
            pluginList.push([value.name, value.id]);
        });
    }, false);
    return pluginList;
}

function LoadDevices(selectedPluginId) {
    var deviceList = [];

    DeviceManager.getAllByInstanceId(selectedPluginId, function (list) {
        $.each(list, function (key, value) {
            deviceList.push([value.friendlyName, value.id]);
        });
    }, false);

    return deviceList;
}

function LoadKeywords(selectedDeviceId) {
    var keywordList = [];
    DeviceManager.getKeywordsByDeviceId(selectedDeviceId, function (list) {
        $.each(list, function (key, value) {
            keywordList.push([value.friendlyName, value.id]);
        });
    }, false);

    return keywordList;
}

/**
 * @return {string}
 */
function GetBlocklyType(yadomsKeywordType) {
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
}

function UpdateBlockColour(self, type) {
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
}

Blockly.FieldDropdown.prototype.refresh = function(data) {
    if(data != undefined && data.length >0) {
        this.menuGenerator_ = data;
        this.setValue(data[0][1]);
        this.updateTextNode_();
        this.changeHandler_(this.getValue());
    }
};


Blockly.Blocks['yadoms_keyword_value'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(20);

    var pluginDd, deviceDd, keywordDd;
    var thisBlock = this;

    //create the plugin dropdown
    pluginDd = new Blockly.FieldDropdown(function() {
          return LoadPlugins();
      }, function(plugin) {
          deviceDd.refresh(LoadDevices(plugin));
      });

      deviceDd = new Blockly.FieldDropdown(function() {
          return LoadDevices(pluginDd.getValue());
      }, function(device) {
          keywordDd.refresh(LoadKeywords(device));
      });

      keywordDd = new Blockly.FieldDropdown(function() {
          return LoadKeywords(deviceDd.getValue());
      }, function(keyword) {

          KeywordManager.get(keyword, function(keywordFromSrv) {
              var type = GetBlocklyType(keywordFromSrv.type);
              if(type == undefined)
                  thisBlock.changeOutput("null");
              else
                  thisBlock.changeOutput(type);
              UpdateBlockColour(thisBlock, type);
          }, true);
      });

      this.appendDummyInput()
          .appendField(pluginDd, "Plugin")
          .appendField(deviceDd, "Device")
          .appendField(keywordDd, "Keyword");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');

      //force full resfresh
      pluginDd.changeHandler_(pluginDd.getValue());

  }
};

/*
Blockly.JavaScript['yadoms_keyword_value'] = function(block) {
  var dropdown_plugin = block.getFieldValue('Plugin');
  var dropdown_device = block.getFieldValue('Device');
  var dropdown_keyword = block.getFieldValue('Keyword');
  // TODO: Assemble JavaScript into code variable.
  var code = null;
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};
*/


Blockly.Blocks['yadoms_affect_keyword'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(290);
      var pluginDd, deviceDd, keywordDd;
      var thisBlock = this;

      var inputValueName = "Value";
      var operatorValueName = "operator";

      //create the plugin dropdown
      pluginDd = new Blockly.FieldDropdown(function() {
          return LoadPlugins();
      }, function(plugin) {
          deviceDd.refresh(LoadDevices(plugin));
      });

      deviceDd = new Blockly.FieldDropdown(function() {
          return LoadDevices(pluginDd.getValue());
      }, function(device) {
          keywordDd.refresh(LoadKeywords(device));
      });

      keywordDd = new Blockly.FieldDropdown(function() {
          return LoadKeywords(deviceDd.getValue());
      }, function(keyword) {

          KeywordManager.get(keyword, function(keywordFromSrv) {
              var type = GetBlocklyType(keywordFromSrv.type);

              if(type == null) {
                  thisBlock.getInput(operatorValueName).setVisible(false);
                  thisBlock.getInput(inputValueName).setVisible(false);
              }
              else {
                  thisBlock.getInput(operatorValueName).setVisible(true);
                  thisBlock.getInput(inputValueName).setVisible(true);

                  if(type == undefined)
                      thisBlock.getInput(inputValueName).setCheck("null");
                  else
                      thisBlock.getInput(inputValueName).setCheck(type);
              }
              UpdateBlockColour(thisBlock, type);
          }, true);
      });

      this.appendDummyInput()
          .appendField(pluginDd, "Plugin")
          .appendField(deviceDd, "Device")
          .appendField(keywordDd, "Keyword");
    this.appendDummyInput(operatorValueName)
        .setAlign(Blockly.ALIGN_CENTRE)
        .appendField("=");
    this.appendValueInput(inputValueName);
    this.setInputsInline(true);
    this.setPreviousStatement(true, "null");
    this.setNextStatement(true, "null");
    this.setTooltip('');
  }
};

/*
Blockly.JavaScript['yadoms_affect_keyword'] = function(block) {
  var dropdown_plugin = block.getFieldValue('Plugin');
  var dropdown_device = block.getFieldValue('Device');
  var dropdown_keyword = block.getFieldValue('Keyword');
  var value_value = Blockly.JavaScript.valueToCode(block, 'Value', Blockly.JavaScript.ORDER_ATOMIC);
  var variable_name = Blockly.JavaScript.variableDB_.getName(block.getFieldValue('NAME'), Blockly.Variables.NAME_TYPE);
  // TODO: Assemble JavaScript into code variable.
  var code = '...';
  return code;
};
*/


Blockly.Blocks['yadoms_logic_compare_become'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(230);
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



Blockly.Blocks['yadoms_logic_compare_is'] = {
    init: function() {
        this.setHelpUrl('http://www.example.com/');
        this.setColour(230);
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


Blockly.Validation = function() {
};

Blockly.Validation.listErroneousBlocks = [];


Blockly.Validation.setBlockInvalid = function(block) {
    if(block != null && block != undefined && $.isFunction(block.addError)) {
        block.addError();
        Blockly.Validation.listErroneousBlocks.push(block);
    }
};

Blockly.Validation.clearInvalidBlocks =  function() {
    $.each(Blockly.Validation.listErroneousBlocks, function(index, value) {
        if(value != null && value != undefined &&$.isFunction(value.removeError)) {
            value.removeError();
        }
    });

    //clear the list
    Blockly.Validation.listErroneousBlocks.splice(0,Blockly.Validation.listErroneousBlocks.length);
};

Blockly.Validation.isBlockValid =  function(block) {

    if(block != null && block != undefined) {
        var currentBlockValid = true;
        $.each(block.inputList, function(index, subBlock) {
            if(subBlock.connection != null){
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





Blockly.BlockSvg.prototype.addError = function() {
    Blockly.addClass_(/** @type {!Element} */ (this.svgGroup_),
        'blocklySelectedError');
    // Move the selected block to the top of the stack.
    this.svgGroup_.parentNode.appendChild(this.svgGroup_);
};

/**
 * Unselect this block.  Remove its highlighting.
 */
Blockly.BlockSvg.prototype.removeError = function() {
    Blockly.removeClass_(/** @type {!Element} */ (this.svgGroup_),
        'blocklySelectedError');
};


Blockly.Validation.Init = function(maxTopBlocks) {
    if(maxTopBlocks != undefined && $.isNumeric(maxTopBlocks))
        Blockly.Validation.maxTopBlocks_ = maxTopBlocks;

    Blockly.addChangeListener( function() {
        Blockly.Validation.clearInvalidBlocks();
    });
};

Blockly.Validation.maxTopBlocks_ = 1;

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


Blockly.Validation.validateMainWorkspace = function(callback) {
    Blockly.Validation.validate(Blockly.mainWorkspace, callback);
};