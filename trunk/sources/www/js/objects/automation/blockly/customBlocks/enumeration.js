
/*

Blockly.Blocks["yadoms_enumeration2"] = {
    enumerationTypeDropDownName: "enumerationTypeList",
    enumerationDropDownName: "enumerationList",
    currentEnumerationTypeName: "",
    init: function () {
        var thisBlock = this;
        var enumDropDown = new Blockly.FieldDropdown([[$.t("blockly.blocks.yadoms_enumeration.title"), "enumeration"]]);

        var enumTypeDropDown = new Blockly.FieldDropdown(function () {
            var enumList = [[]];

            $.each(Blockly.Yadoms.data.enumerations, function(index, enumType) {
                enumList.push([enumType.name, enumType.typeToSet]);
            });

            if (enumList.length === 0) {
                thisBlock.setDisabled(true);
                enumList = [[$.t("blockly.invalid"), "invalid"]];
            }
            return enumList;
        }, function(selectedType) {
            var enumValues = Blockly.Yadoms.data.enumerations[selectedType].values;
            if (enumValues.length === 0) {
                enumValues = [[$.t("blockly.invalid"), "invalid"]];
            }
            var enumDropDown = $.isFunction(this.getField_) ? this.getField_(this.enumerationDropDownName) : this.getField(this.enumerationDropDownName);
            enumDropDown.refresh(enumValues, enumDropDown.getValue());
        });

        this.setHelpUrl("http://www.example.com/");
        this.setColour(20);
        this.appendDummyInput()
            .appendField(enumTypeDropDown, this.enumerationTypeDropDownName)
            .appendField(enumDropDown, this.enumerationDropDownName);
        this.setOutput(true, "");
        this.setTooltip($.t("blockly.blocks.yadoms_enumeration.tooltip", { defaultValue: "" }));

        this.getSelectedEnumValue = function () {
            return enumDropDown.getValue();
        };

        //add event fire value changed
        enumDropDown.setValue = function (newValue) {
            var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
            if (!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
                this.changeHandler_(this.getValue());
            return result;
        };

    }

    /*,

    updateEnumeration: function (keywordValue, deviceValue, pluginValue) {
        //check keyword value is not null
        if (!isNullOrUndefined(keywordValue)) {

            //check that keyword data is available
            var pluginData = Blockly.Yadoms.data.plugins[pluginValue];
            var keywordData = Blockly.Yadoms.data.keywords[keywordValue];
            if (!isNullOrUndefined(keywordData) && keywordData.type.toUpperCase() === "enum".toUpperCase()) {
                var typeInfo = keywordData.typeInfo;
                if (!isNullOrUndefined(typeInfo) && !isNullOrUndefined(typeInfo.name) && !isNullOrUndefined(typeInfo.values)) {
                    if (this.currentEnumerationTypeName !== typeInfo.name) {

                        var typeToSet = "enum_" + typeInfo.name;

                        //make it compatible with older blockly version
                        $.isFunction(this.changeOutput) ? this.changeOutput(typeToSet) : this.outputConnection.setCheck(typeToSet);


                        //all is OK, this is a new enum, ask for translation
                        var translatedEnum = [];
                        $.each(typeInfo.values, function (index, value) {
                            var trad = $.t(pluginData.type + ":enumerations." + typeInfo.name + "." + value, { defaultValue: pluginData.package.enumerations[typeInfo.name][value] });
                            translatedEnum.push([trad, value]);
                        });
                        //apply new dropdown list
                        this.currentEnumerationTypeName = typeInfo.name;
                        var enumDropDown = $.isFunction(this.getField_) ? this.getField_(this.enumerationDropDownName) : this.getField(this.enumerationDropDownName);

                        enumDropDown.refresh(translatedEnum, enumDropDown.getValue());
                    }
                }
            }
        }
    }*/
//};








Blockly.Blocks["yadoms_enumeration"] = {
    enumerationDropDownName: "enumerationList",
    currentEnumerationTypeName: "",
    init: function () {
        var enumDropDown = new Blockly.FieldDropdown([[$.t("blockly.blocks.yadoms_enumeration.title"), "enumeration"]]);

        this.setHelpUrl("http://www.example.com/");
        this.setColour(20);
        this.appendDummyInput()
            .appendField(enumDropDown, this.enumerationDropDownName);
        this.setOutput(true, "");
        this.setTooltip($.t("blockly.blocks.yadoms_enumeration.tooltip", { defaultValue: "" }));

        this.getSelectedEnumValue = function () {
            return enumDropDown.getValue();
        };

        //add event fire value changed
        enumDropDown.setValue = function (newValue) {
            var result = Blockly.FieldDropdown.prototype.setValue.call(this, newValue);
            if (!isNullOrUndefined(this.changeHandler_) && $.isFunction(this.changeHandler_))
                this.changeHandler_(this.getValue());
            return result;
        };
    },

    updateEnumeration: function (keywordValue, deviceValue, pluginValue) {
        //check keyword value is not null
        if (!isNullOrUndefined(keywordValue)) {

            //check that keyword data is available
            var pluginData = Blockly.Yadoms.data.plugins[pluginValue];
            var keywordData = Blockly.Yadoms.data.keywords[keywordValue];
            if (!isNullOrUndefined(keywordData) && keywordData.type.toUpperCase() === "enum".toUpperCase()) {
                var typeInfo = keywordData.typeInfo;
                if (!isNullOrUndefined(typeInfo) && !isNullOrUndefined(typeInfo.name) && !isNullOrUndefined(typeInfo.values)) {
                    if (this.currentEnumerationTypeName !== typeInfo.name) {

                        var typeToSet = "enum_" + typeInfo.name;

                        //make it compatible with older blockly version
                        $.isFunction(this.changeOutput) ? this.changeOutput(typeToSet) : this.outputConnection.setCheck(typeToSet);


                        //all is OK, this is a new enum, ask for translation
                        var translatedEnum = [];
                        $.each(typeInfo.values, function (index, value) {
                            var trad = $.t(pluginData.type + ":enumerations." + typeInfo.name + "." + value, { defaultValue: pluginData.package.enumerations[typeInfo.name][value] });
                            translatedEnum.push([trad, value]);
                        });
                        //apply new dropdown list
                        this.currentEnumerationTypeName = typeInfo.name;
                        var enumDropDown = $.isFunction(this.getField_) ? this.getField_(this.enumerationDropDownName) : this.getField(this.enumerationDropDownName);

                        enumDropDown.refresh(translatedEnum, enumDropDown.getValue());
                    }
                }
            }
        }
    }
};
