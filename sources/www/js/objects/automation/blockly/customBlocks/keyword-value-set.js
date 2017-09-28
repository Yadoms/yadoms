

/**
 * Define a block which allow to change a keyword value
 * @type {{inputValueName: string, operatorValueName: string, init: Function, updateShape_: Function}}
 */
Blockly.Blocks["keyword-value-set"] = {
    inputValueName: "Value",
    operatorValueName: "operator",
    unitsInputName: "units",
    deviceDropDownName: "Device",
    keywordDropDownName: "Keyword",
	titleFieldName: "titleField",
    inputValueShown: false,
    init: function () {
		this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip($.t("blockly.blocks.keyword-value-set.tooltip", { defaultValue: "" }));
        this.setColour(Blockly.Yadoms.blockColour.HUE);

        this.appendDummyInput()
            .appendField($.t("blockly.blocks.keyword-value-set.title"), this.titleFieldName);
        var thisBlock = this;

        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, true, ["numeric", "string", "bool", "nodata", "enum", "datetime"], undefined, function (keyword, keywordType) {
            if (keywordType == null) {

                var previousInput = thisBlock.getInput(thisBlock.inputValueName);
                if (previousInput) {
                    //remove any shadow item before removing input if already exists
                    //if the input is connected to shadow block, then just remove it
                    if (previousInput.connection && previousInput.connection.targetConnection) {
                        if (previousInput.connection.targetBlock() && previousInput.connection.targetBlock().isShadow()) {
							try {
								previousInput.connection.targetBlock().dispose();
							}catch(e) {
								try {
									previousInput.connection.targetBlock().dispose();
								}catch(e) {
								}
							}
                        }
                    }
                }

                thisBlock.updateShape_(false);
            }
            else {
                thisBlock.updateShape_(true);

                var input = thisBlock.getInput(thisBlock.inputValueName);

                //remove any shadow item before changing type check
                //if the input is connected to shadow block, then just remove it
                if (input.connection && input.connection.targetConnection) {
                    var childBlock = input.connection.targetBlock();
                    if (childBlock) {
                        if (childBlock.isShadow()) {
							try {
								childBlock.dispose();
							}catch(e) {
								try {
									childBlock.dispose();
								}catch(e) {
								}
							}
                        }
                    }
                }

                //update type check
                input.setCheck(keywordType);

                //if connection is empty, add good default block
                if (!input.connection.targetConnection && Blockly.Yadoms.isLoadingFromXml === false) {
                    var newChildBlock = Blockly.Yadoms.GetDefaultBlock_(Blockly.Yadoms.data.keywords[keyword], thisBlock.workspace);
                    if (newChildBlock) {
                        newChildBlock.setShadow(true);
                        newChildBlock.initSvg();
                        newChildBlock.render();
                        input.connection.connect(newChildBlock.outputConnection);
                    }					
					
                }
            }
            thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keyword]);
        }, thisBlock.deviceDropDownName, thisBlock.keywordDropDownName);

    },

    /**
     * Modify this block to have (or not have) an input for 'is divisible by'.
     * @param {boolean} extraInput True if this block has a extra input.
     * @private
     * @this Blockly.Block
     */
    updateShape_: function (extraInput) {

        var opExist = this.getInput(this.operatorValueName);
        var inExist = this.getInput(this.inputValueName);

        this.inputValueShown = extraInput;
        // Add or remove a Value Input.
        if (extraInput) {
            if (!opExist) {
                this.appendDummyInput(this.operatorValueName).setAlign(Blockly.ALIGN_CENTRE).appendField($.t("blockly.blocks.keyword-value-set.titleTo", { defaultValue: "=" }));
            }
            if (!inExist) {
                this.appendValueInput(this.inputValueName);
            }
			this.setFieldValue($.t("blockly.blocks.keyword-value-set.title"), this.titleFieldName);
        } else {
            if (inExist) {
                this.removeInput(this.inputValueName);
            }
            if (opExist) {
                this.removeInput(this.operatorValueName);
            }
			this.setFieldValue($.t("blockly.blocks.keyword-value-set.titleCall"), this.titleFieldName);
        }
    },

    updateUnit: function (keyword) {
        var unitsExist = this.getInput(this.unitsInputName);
        if (unitsExist)
            this.removeInput(this.unitsInputName);

        if (!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units)) {
            this.appendDummyInput(this.unitsInputName).appendField($.t(keyword.units));
        }
    },



    /**
     * Create XML to represent whether the 'divisorInput' should be present.
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function () {
        var container = document.createElement("mutation");
        //attribute name must be lower case
        container.setAttribute("input_shown", this.inputValueShown);
        return container;
    },
    /**
     * Parse XML to restore the 'divisorInput'.
     * @param {!Element} xmlElement XML storage element.
     * @this Blockly.Block
     */
    domToMutation: function (xmlElement) {
        //attribute name must be lower case
        var inputValueShown = (xmlElement.getAttribute("input_shown") === "true");

        this.updateShape_(inputValueShown);
    }


};

