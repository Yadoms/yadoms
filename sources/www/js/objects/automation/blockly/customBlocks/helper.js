
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
    thisBlock.emptyDeviceDropDownItem_ = [$.t("blockly.chooseDeviceHelper"), "__inv@lid__"];
    thisBlock.emptyKeywordDropDownItem_ = [$.t("blockly.chooseKeywordHelper"), "__inv@lid__"];
    // ReSharper disable JoinDeclarationAndInitializerJs
    var keywordDd;
    // ReSharper restore JoinDeclarationAndInitializerJs

    var deviceDd = new Blockly.FieldDropdown(function () {
        var deviceList = Blockly.Yadoms.LoadDevices_(onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);
        if (deviceList == null || deviceList.length === 0) {
            //thisBlock.setDisabled(true);
            deviceList = [thisBlock.emptyDeviceDropDownItem_];
        } else {
            deviceList.splice(0, 0, thisBlock.emptyDeviceDropDownItem_);
        }
        return deviceList;
    }, function (device) {
        if (!thisBlock.disabled) {

            if (!Blockly.Yadoms.data.devices[device]) {
                keywordDd.refresh([thisBlock.emptyKeywordDropDownItem_]);
                thisBlock.selectedPlugin_ = null;
            } else {
                //update keyword dropdown
                var keywordList = Blockly.Yadoms.LoadKeywords_(device, onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);

                var toSelect = undefined;
                if (keywordList.length === 1)
                    toSelect = keywordList[0][1];

                keywordList.splice(0, 0, thisBlock.emptyKeywordDropDownItem_);
                keywordDd.refresh(keywordList, toSelect);

                //update selectedPlugin value
                thisBlock.selectedPlugin_ = Blockly.Yadoms.GetPluginMatchingDevice_(device);
            }
        }
    });


    keywordDd = new Blockly.FieldDropdown(function () {
        var keywordList = Blockly.Yadoms.LoadKeywords_(deviceDd.getValue(), onlyWritableKeywords, allowedKeywordTypes, allowedKeywordCapacities);
        if (keywordList == null || keywordList.length === 0) {
            //thisBlock.setDisabled(true);
            keywordList = [thisBlock.emptyKeywordDropDownItem_];
        } else {
            keywordList.splice(0, 0, thisBlock.emptyKeywordDropDownItem_);
        }

        return keywordList;
    }, function (keyword) {
        if (!thisBlock.disabled) {

            var yadomsTypeName = "";


            if (Blockly.Yadoms.data.keywords[keyword]) {
                if (!isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo) && !isNullOrUndefined(Blockly.Yadoms.data.keywords[keyword].typeInfo.name))
                    yadomsTypeName = Blockly.Yadoms.data.keywords[keyword].typeInfo.name;

                var type = Blockly.Yadoms.GetBlocklyType_(Blockly.Yadoms.data.keywords[keyword].type, yadomsTypeName);

                if (inputType && inputType.type === Blockly.INPUT_VALUE) {

                    //if the input is connected to shadow block, then just remove it
                    var connection = inputType.connection;
                    var connectionIsNull = connection && !connection.targetConnection;
                    if (!connectionIsNull) {
                        var childBlock = connection.targetBlock();
                        if (childBlock) {
                            if (childBlock.isShadow()) {
                                childBlock.dispose();
                            }
                        }
                    }

                    inputType.setCheck(type);

                    //if connection is empty, add good default block
                    if (!connection.targetConnection && Blockly.Yadoms.isLoadingFromXml === false) {
                        var newChildBlock = Blockly.Yadoms.GetDefaultBlock_(Blockly.Yadoms.data.keywords[keyword], workspace);
                        if (newChildBlock) {
                            newChildBlock.setShadow(true);
                            newChildBlock.initSvg();
                            newChildBlock.render();
                            connection.connect(newChildBlock.outputConnection);
                        }
                    }
                }


                if (callbackKeywordSelectionChanged) {
                    callbackKeywordSelectionChanged(keyword, type);
                }
            } else {
                //not defined keyword
                if (inputType && inputType.type === Blockly.INPUT_VALUE) {
                    inputType.setCheck(null); //any type allowed
                }
            }

            thisBlock.isValid();
        }
    });

    //add event fire value changed
    deviceDd.setValue = function (newValue) {
        if (!Blockly.Yadoms.data.devices[newValue]) {
            newValue = thisBlock.emptyDeviceDropDownItem_[1]; //use default value
        }

        var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
        if (!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
            this.changeHandler_(this.getValue());
        return result;
    };


    //add event fire value changed
    keywordDd.setValue = function (newValue) {
        if (!Blockly.Yadoms.data.keywords[newValue]) {
            newValue = thisBlock.emptyKeywordDropDownItem_[1]; //use default value
        }

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

    thisBlock.isValid = function () {
        var deviceValid = thisBlock.getSelectedDevice() !== thisBlock.emptyDeviceDropDownItem_[1];
        if (deviceDd.textElement_) {
            if (!deviceValid) {
                Blockly.addClass_(deviceDd.textElement_, "blocklyValidationDropdownError");
            } else {
                Blockly.removeClass_(deviceDd.textElement_, "blocklyValidationDropdownError");
            }
        }
        var keywordValid = thisBlock.getSelectedKeyword() !== thisBlock.emptyKeywordDropDownItem_[1];
        if (keywordDd.textElement_) {
            if (!keywordValid) {
                Blockly.addClass_(keywordDd.textElement_, "blocklyValidationDropdownError");
            } else {
                Blockly.removeClass_(keywordDd.textElement_, "blocklyValidationDropdownError");
            }
        }
        return deviceValid && keywordValid;
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
    thisBlock.isValid();
    return result;
};
