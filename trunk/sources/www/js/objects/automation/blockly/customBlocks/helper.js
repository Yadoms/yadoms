
/**
 * Configure a custom block by adding the dropdowns allowing selecting a keyword
 * @param thisBlock The block to configure
 * @param onlyWritableKeywords If true allow only keywords that could be written
 * @param allowedKeywordTypes Array of allowed keyword type ("numeric", "string", "bool", "json", "nodata"
 * @param allowedKeywordCapacities Array of allowed keyword capacities (undefined for all, "message",...=
 * @param callbackKeywordSelectionChanged A callback for keyword notification change
 * @param deviceDropDownName The name of the devices DropDown
 * @param keywordDropDownName The name of the keywords DropDown
 * @constructor
 */
Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection = function (thisBlock, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities, callbackKeywordSelectionChanged, deviceDropDownName, keywordDropDownName, inputType, inputName, workspace) {
    thisBlock.selectedPlugin_ = null;

    var keywordDd;

    var deviceDd = new Blockly.FieldDropdown(function () {
        var deviceList = Blockly.Yadoms.LoadDevices_(onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);
        if (deviceList == null || deviceList.length === 0) {
            thisBlock.setDisabled(true);
            deviceList = [[$.t("blockly.invalid"), "invalid"]];
        }
        return deviceList;
    }, function (device) {
        if (!thisBlock.disabled) {
            //update keyword dropdown
            var keywordList = Blockly.Yadoms.LoadKeywords_(device, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);
            keywordDd.refresh(keywordList);

            //update selectedPlugin value
            thisBlock.selectedPlugin_ = Blockly.Yadoms.GetPluginMatchingDevice_(device);
        }
    });


    keywordDd = new Blockly.FieldDropdown(function () {
        var keywordList = Blockly.Yadoms.LoadKeywords_(deviceDd.getValue(), onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);
        if (keywordList == null || keywordList.length === 0) {
            thisBlock.setDisabled(true);
            keywordList = [[$.t("blockly.invalid"), "invalid"]];
        }
        return keywordList;
    }, function (keyword) {
        if (!thisBlock.disabled) {

            var yadomsTypeName = "";
            if (!isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo) && !isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo.name))
                yadomsTypeName = Blockly.Yadoms.data.keywords[keyword].typeInfo.name;
			
            var type = Blockly.Yadoms.GetBlocklyType_(Blockly.Yadoms.data.keywords[keyword].type, yadomsTypeName);

			//automatic block creation
			/*
			if(inputType && inputType.type == Blockly.INPUT_VALUE) {
				if(workspace) {
					//if connection is empty, add good default block
					var childBlock = null;
					var connection = inputType.connection;
					var connectionIsNull = connection && !connection.targetConnection;
					
					if(connectionIsNull) {
						childBlock = Blockly.Yadoms.GetDefaultBlock_(Blockly.Yadoms.data.keywords[keyword], workspace);
						if(childBlock) {
							//childBlock.setFieldValue('NUM', 42);
							childBlock.initSvg();
							childBlock.render();
							if(!childBlock.data)
								childBlock.data = {};
							childBlock.data.automaticallyCreated = true;
							childBlock.onchange = function() {
								debugger;
								if(childBlock.data)
									childBlock.data.automaticallyCreated = false;
							}
							connection.connect(childBlock.outputConnection);
						}
					} else {
						childBlock = connection.targetBlock();
					}
					
					inputType.setCheck(type);
					
					if(childBlock) {
						if(connectionIsNull) {
								
							//in case the block has been disposed due to setCheck method, then just delete it
							if(!childBlock.getParent()) {
								childBlock.dispose();						
							}
						} else {
							//check if the initial child block is still connected
							if(!childBlock.getParent()) {
								if(childBlock.data && childBlock.data.automaticallyCreated === true) {
									childBlock.dispose();
								}								
							}							
						}
					}
				} else {
					inputType.setCheck(type);
				}
			}
			*/
			if(inputType && inputType.type == Blockly.INPUT_VALUE) {
				inputType.setCheck(type);
			}
			

            if (callbackKeywordSelectionChanged) {
                callbackKeywordSelectionChanged(keyword, type);
            }
        }
    });

    //add event fire value changed
    deviceDd.setValue = function (newValue) {
        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if (!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };


    //add event fire value changed
    keywordDd.setValue = function (newValue) {
        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if (!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };


    thisBlock.getSelectedKeyword = function () {
        return keywordDd.getValue();
    };
    thisBlock.getSelectedDevice = function () {
        return deviceDd.getValue();
    };
    thisBlock.getSelectedPlugin = function () {
        return thisBlock.selectedPlugin_;
    };

    var inputNameToUse = inputName;
    if (isNullOrUndefined(inputName)) {
        inputNameToUse = "YadomsSelection";
    }

    var result;
    if (inputType === "statement") {
        result = thisBlock.appendStatementInput(inputNameToUse)
            .appendField(deviceDd, deviceDropDownName)
            .appendField(keywordDd, keywordDropDownName);
    }
    else {
        if (!isNullOrUndefined(inputType)) {
            result = inputType
                .appendField(deviceDd, deviceDropDownName)
                .appendField(keywordDd, keywordDropDownName);
        } else {
            result = thisBlock.appendDummyInput(inputNameToUse)
                .appendField(deviceDd, deviceDropDownName)
                .appendField(keywordDd, keywordDropDownName);
        }
    }

    deviceDd.changeHandler_(deviceDd.getValue());
    return result;
};
