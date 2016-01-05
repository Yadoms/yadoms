
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


Blockly.Yadoms.Enumerations = {
    mutators: [],

    initialize : function() {

        $.each(Blockly.Yadoms.data.enumerations, function (index, enumItem) {
            var blockName = "yadoms_enum_mutator_" + enumItem.name;

            Blockly.Blocks[blockName] = {
                init: function () {
                    this.setColour(Blockly.Yadoms.blockColour.HUE);
                    this.appendDummyInput().appendField(enumItem.name);
                    this.appendStatementInput("STACK");
                    this.setPreviousStatement(true);
                    this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.tooltip"));
                    this.contextMenu = false;
                }
            }

            Blockly.Yadoms.Enumerations.mutators.push(blockName);
        });
    }
};




Blockly.Blocks["yadoms_enumeration"] = {
    enumerationDropDownName: "enumerationList",
    
    init: function () {
        this.currentEnumerationTypeName = "";
        var enumDropDown = new Blockly.FieldDropdown([[$.t("blockly.blocks.yadoms_enumeration.title"), "enumeration"]]);

        this.setHelpUrl("http://www.example.com/");
        this.setColour(Blockly.Yadoms.blockColour.HUE);
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

        this.setMutator(new Blockly.Mutator([]));
    },

    updateEnumeration: function (keywordValue, deviceValue, pluginValue) {
        //check keyword value is not null
        if (!isNullOrUndefined(keywordValue)) {

            //check that keyword data is available
            if (!pluginValue) {
                pluginValue = Blockly.Yadoms.data.getPluginIdFromKeywordId(keywordValue);
            }
            
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
                            var trad = $.t("plugins/" + pluginData.type + ":enumerations." + typeInfo.name + "." + value, { defaultValue: pluginData.package.enumerations[typeInfo.name][value] });
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
    },




    /**
     * Create XML to represent whether the number of mutator blocks to add
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function () {
        var container = document.createElement("mutation");
        return container;
    },

    /**
     * Parse XML to restore the mutators blocks'.
     * @param {!Element} xmlElement XML storage element.
     * @this Blockly.Block
     */
    domToMutation: function (xmlElement) {
    },

    /**
    * Populate the mutator's dialog with this block's components.
    * @param {!Blockly.Workspace} workspace Mutator's workspace.
    * @return {!Blockly.Block} Root block in mutator.
    * @this Blockly.Block
    */
    decompose: function (workspace) {
        var topBlock = Blockly.Block.obtain(workspace, "yadoms_enumeration_mutator");
        topBlock.initSvg();
        return topBlock;
    },

    /**
     * Reconfigure this block based on the mutator dialog's components.
     * @param {!Blockly.Block} containerBlock Root block in mutator.
     * @this Blockly.Block
     */
    compose: function (containerBlock) {
        //This method analyse the mutator dialog result, then recreate the real blick
        //so it takes sur clauseBlock (= mutator topblock stack = all additional keywords)
        //foreach additional keyword block found, just append needed blocks
        var keyword = containerBlock.getSelectedKeyword();
        var device = containerBlock.getSelectedDevice();
        var plugin = containerBlock.getSelectedPlugin();
        this.updateEnumeration(keyword, device, plugin);
    }
};


Blockly.Blocks["yadoms_enumeration_mutator"] = {
    deviceDropDownName: "Device",
    keywordDropDownName: "Keyword",

    /**
     * Mutator block for if container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_enumeration.mutator.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_enumeration.mutator.tooltip"));
        this.contextMenu = false;
        var thisBlock = this;
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(thisBlock, false, ["enum"], undefined, undefined, this.deviceDropDownName, this.keywordDropDownName);
    }
};