/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks["yadoms_wait_for_event"] = {
    init: function () {


        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.titleLine1"));

        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.tooltip"));
        this.setHelpUrl("http://www.example.com/");
        this.setInputsInline(true);
        this.setMutator(new Blockly.Mutator(["yadoms_wait_for_event_mutator_store_in_variable",
                                            "yadoms_wait_for_event_mutator_change",
                                            "yadoms_wait_for_event_mutator_become",
											"yadoms_wait_for_event_mutator_capacity_change",
											"yadoms_wait_for_event_mutator_capacity_become",
                                            "yadoms_wait_for_event_mutator_datetime_change",
                                            "yadoms_wait_for_event_mutator_datetime_become"]));

        /**
         * Member which stores the mutation data
        */
        this.mutationData_ = {
            storeInVariable: false,
            additionalBlocks: []
        };
    },





    /**
     * Update the variable store option
     * @param {Boolean} showInput if true the input is shown, else the input is deleted if exists
     * @this Blockly.Block
     * @private
     */
    updateStoreVariableInput_: function (showInput, varName) {
        var input = this.getInput("storeVariableInput");

        if (showInput === true) {
            if (input == null) {
                this.appendDummyInput("storeVariableInput")
					.appendField($.t("blockly.blocks.yadoms_wait_for_event.titleLine2"))
					.appendField(" ")
					.appendField(new Blockly.FieldVariable(this.generateVariable_(varName)), "outVar")
					.setForceNewlineInput(true);
            }
        } else {
            if (input !== null) {
                this.removeInput(input);
            }
        }
    },

    /**
     * Remoe all additional inputs
     * @private
     */
    removeAllAdditionalInputs_: function () {
        //remove all additional inputs
        var i = 0;
        var inputToDelete = this.getInput("additionalInput_part1_" + i);
        while (inputToDelete != null) {

            //delete part1
            this.removeInput("additionalInput_part1_" + i);

            //delete part2
            inputToDelete = this.getInput("additionalInput_part2_" + i);
            if (inputToDelete != null) {
                this.removeInput("additionalInput_part2_" + i);
            }

            i++;
            inputToDelete = this.getInput("additionalInput_part1_" + i);
        }
    },

    /**
     * Update inputs
     * @param {Array[String]} inputList The additional inputs to show
     * @this Blockly.Block
     * @private
     */
    updateAdditionalInputs_: function (inputList) {
        var self = this;
        this.removeAllAdditionalInputs_();
        if (inputList != undefined) {
            $.each(inputList, function (index, inputToShow) {
                switch(inputToShow.toLowerCase()) {
                    case "yadoms_wait_for_event_mutator_change":
                        self.appendKeywordSelectorStatementChange_(index);
                        break;
                    case "yadoms_wait_for_event_mutator_become":
                        self.appendKeywordSelectorStatementBecome_(index);
                        break;

                    case "yadoms_wait_for_event_mutator_capacity_change":
                        self.appendCapacityStatementChange_(index);
                        break;

                    case "yadoms_wait_for_event_mutator_capacity_become":
                        self.appendCapacityStatementBecome_(index);
                        break;

                    case "yadoms_wait_for_event_mutator_datetime_change":
                        self.appendDatetimeStatementChange_(index);
                        break;

                    case "yadoms_wait_for_event_mutator_datetime_become":
                        self.appendDatetimeStatementBecome_(index);
                        break;

                }
            });
        }
    },

    /**
     * Add a keyword change block
     * @param {Number} no The input number 
     * @param {Number} devId The selected device id (can be null/undefined)
     * @param {Number} keyId  The selected keyword id (can be null/undefined)
     * @param {Blockly.Connection} statementConnection 
     * @private 
     */
    appendKeywordSelectorStatementChange_ : function(no, devId, keyId, statementConnection) {
        var bDummyInput = this.appendDummyInput("additionalInput_part1_" + no).appendField($.t("blockly.blocks.yadoms_wait_for_event.case"));
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, false, ["numeric", "string", "bool", "nodata", "enum", "datetime"], undefined, undefined, "deviceDd" + no, "keywordDd" + no, bDummyInput)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredType.change"));
		bDummyInput.setForceNewlineInput(true);
        if (devId) {
            var devDd = this.getField("deviceDd" + no);
            devDd.setValue(devId);
        }
        if (keyId) {
            var keyDd = this.getField("keywordDd" + no);
            keyDd.setValue(keyId);
        }

        this.appendInputPart2_(no, statementConnection);
    },

    /**
     * When something changes on the block, check if any enumeration block has been connected, and then update types
     */
    onchange: function () {
        //browse all become inputs
        //if a connected block is an enuemration, then update enum matching the selected keyword
        var self = this;
        $.each(this.mutationData_.additionalBlocks, function (index, blockType) {
            if (blockType === "yadoms_wait_for_event_mutator_become" || blockType === "yadoms_wait_for_event_mutator_capacity_become") {
                var input = self.getInput("additionalInput_part1_" + index);
                if (input && input.connection) {
                    var block = input.connection.targetBlock();
                    if (block) {
                        if (block.type === "yadoms_enumeration") {
                            var keywordValue = self.getFieldValue("keywordDd" + index);
                            if (keywordValue) {
                                block.updateEnumeration(keywordValue);
                            }
                        }
                    }
                }
            }
        });
    },

    /**
     * Add a keyword become block
     * @param {} no The input number
     * @param {} devId The selected device id (can be null/undefined)
     * @param {} keyId  The selected keyword id (can be null/undefined)
     * @param {} operator The selected operator (can be null/undefined)
     * @param {} valueConnection The value connection (can be null/undefined)
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendKeywordSelectorStatementBecome_: function (no, devId, keyId, operator, valueConnection, statementConnection) {
        var bValueInput = this.appendValueInput("additionalInput_part1_" + no).appendField($.t("blockly.blocks.yadoms_wait_for_event.case"));
        var self = this;
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, false, ["numeric", "string", "bool", "datetime", "enum"], undefined, function (keyword, type) { self.onKeywordChange_(no, keyword, type); }, "deviceDd" + no, "keywordDd" + no, bValueInput, "", self.workspace)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredType.become")).appendField(new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_), "operatorDd"+no);
		bValueInput.setForceNewlineInput(true);
		
        if (devId) {
            var devDd = this.getField("deviceDd" + no);
            devDd.setValue(devId);
        }
        if (keyId) {
            var keyDd = this.getField("keywordDd" + no);
            keyDd.setValue(keyId);
        }

        if (operator) {
            var opDd = this.getField("operatorDd" + no);
            opDd.setValue(operator);
        }

        if (valueConnection) {

            if (bValueInput && bValueInput.connection && bValueInput.connection.targetBlock()) {
                bValueInput.connection.targetBlock().dispose();
            }

            bValueInput.connection.connect(valueConnection);
        }

        this.appendInputPart2_(no, statementConnection);
    },

    /**
     * Append a capacity change block
     * @param {} no The input number
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendCapacityStatementChange_: function (no, capacity, statementConnection) {
        var bValueInput = this.appendDummyInput("additionalInput_part1_" + no).appendField($.t("blockly.blocks.yadoms_wait_for_event.caseCapacity"));
		var self = this;
		Blockly.Yadoms.ConfigureBlockForYadomsCapacitySelection(this, function (newCapacity, type) { self.onCapacityChange_(no, newCapacity, type); }, "capacityDd" + no, bValueInput, "", self.workspace)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredCapacityType.change"));
		bValueInput.setForceNewlineInput(true);
        this.appendInputPart2_(no, statementConnection);
    },   


    /**
     * Add a capacity become block
     * @param {} no The input number
     * @param {} capacity The selected capacity (can be null/undefined)
     * @param {} operator The selected operator (can be null/undefined)
     * @param {} valueConnection The value connection (can be null/undefined)
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendCapacityStatementBecome_: function (no, capacity, operator, valueConnection, statementConnection) {
        var bValueInput = this.appendValueInput("additionalInput_part1_" + no).appendField($.t("blockly.blocks.yadoms_wait_for_event.caseCapacity"));
        var self = this;
        Blockly.Yadoms.ConfigureBlockForYadomsCapacitySelection(this, function (newCapacity, type) { self.onCapacityChange_(no, newCapacity, type); }, "capacityDd" + no, bValueInput, "", self.workspace)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredCapacityType.become")).appendField(new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_), "operatorDd"+no);
		bValueInput.setForceNewlineInput(true);
		
        if (capacity) {
            var capacityDd = this.getField("capacityDd" + no);
            capacityDd.setValue(capacity);
        }
        if (operator) {
            var opDd = this.getField("operatorDd" + no);
            opDd.setValue(operator);
        }

        if (valueConnection) {

            if (bValueInput && bValueInput.connection && bValueInput.connection.targetBlock()) {
                bValueInput.connection.targetBlock().dispose();
            }

            bValueInput.connection.connect(valueConnection);
        }

        this.appendInputPart2_(no, statementConnection);
    },

	
	/**
     * Append a date/time change block
     * @param {} no The input number
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendDatetimeStatementChange_: function (no, statementConnection) {
        this.appendDummyInput("additionalInput_part1_" + no).appendField($.t("blockly.blocks.yadoms_wait_for_event.case"))
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.datetime"))
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredType.change"))
			.setForceNewlineInput(true);

        this.appendInputPart2_(no, statementConnection);
    },

    /**
     * Append a date/time become block
     * @param {} no The input number
     * @param {} operator The previously saved operator value (can be null/undefined)
     * @param {} valueConnection The value connection (can be null/undefined)
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendDatetimeStatementBecome_: function (no, operator, valueConnection, statementConnection) {
        var bValueInput = this.appendValueInput("additionalInput_part1_" + no)
            .setCheck(["datetime", "time", "date"])
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.case"))
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.datetime"))
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredType.become"))
            .appendField(new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_), "operatorDd" + no);

        bValueInput.setForceNewlineInput(true);

        if (operator) {
            var opDd = this.getField("operatorDd" + no);
            opDd.setValue(operator);
        }

        if (bValueInput) {
            if (valueConnection) {
                bValueInput.connection.connect(valueConnection);
            } else if(Blockly.Yadoms.isLoadingFromXml === false) {
               var newChildBlock = this.workspace.newBlock('yadoms_date_datetime');
                if (newChildBlock) {
                    newChildBlock.setShadow(true);
                    newChildBlock.initSvg();
                    newChildBlock.render();
                    bValueInput.connection.connect(newChildBlock.outputConnection);
                }
            }
        }

        this.appendInputPart2_(no, statementConnection);
    },

    /**
     * Create the statement input for a block
     * @param {} no The input number
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendInputPart2_ : function(no, statementConnection) {
        var inputPart2 = this.appendStatementInput("additionalInput_part2_" + no);
        inputPart2.appendField("    ")
           .setAlign(Blockly.ALIGN_RIGHT)
           .appendField($.t("blockly.blocks.yadoms_wait_for_event.do"));

        if (statementConnection) {
            inputPart2.connection.connect(statementConnection);
        }
        
    },

    /**
     * When keyword changes, update check and operator
     * @param {Number} no The input number
     */
    onKeywordChange_: function (no, keywordId, type) {
      this.updateOperator_(no, type);
    },
	
    /**
     * When capacity changes, update check and operator
     * @param {Number} no The input number
     */
    onCapacityChange_: function (no, capacity, type) {
      this.updateOperator_(no, type);
    },

    /**
    * Method which update the operator list, according to the connected types
    */
    updateOperator_ : function (no, newCheck) {
        var selectedOperators;
        if (newCheck === "String") {
            selectedOperators = Blockly.Yadoms.StringOperators_;
        } else if (newCheck === "Boolean") {
            selectedOperators = Blockly.Yadoms.BooleanOperators_;
        } else if (newCheck === "Number") {
            selectedOperators = Blockly.Yadoms.NumberOperators_;
        } else if (newCheck === "Any" || newCheck == null) {
            selectedOperators = Blockly.Yadoms.NumberOperators_;
        } else {
            //specific types
            selectedOperators = Blockly.Yadoms.BooleanOperators_;
        }

        var operatorDropDown = this.getField("operatorDd" + no);
        if (operatorDropDown) {
            //before replacing menuGenerator, get value to restore it after menu update
            var oldValue = operatorDropDown.getValue();
            operatorDropDown.menuGenerator_ = selectedOperators;
            operatorDropDown.setValue(oldValue || selectedOperators[0][1]);
            operatorDropDown.updateTextNode_();
        }
    },

    /**
     * Generate a unique variable name
     * @return {String} The variable name
     * @this Blockly.Block
     * @private
     */
    generateVariable_: function (defaultVarName) {
        //retreive the workspace
        var workspace;
        if (this.isInFlyout) {
            workspace = this.workspace.targetWorkspace;
        } else {
            workspace = this.workspace;
        }

        //get the default name
        var name = defaultVarName || $.t("blockly.blocks.yadoms_wait_for_event.defaultVarName");

        //if variable is already used anywhere, then append an integer until finding a non used name
        if (this.isVariableInUse_(workspace, name)) {
            var offset = 0;
            while (this.isVariableInUse_(workspace, name + offset)) {
                offset++;
            }
            name += offset;
        }

        //rename it to our chosen name
        Blockly.Variables.renameVariable(name, name, workspace);
        return name;
    },

    /**
     * Check if a variable if already used
     * @param {Workspace} workspace The workspace
     * @param {String} varName The variable name to check
     * @return {Boolean} true if the variable is already in use in the workspace
     * @this Blockly.Block
     * @private
     */
    isVariableInUse_: function (workspace, varName) {
        var variableList = Blockly.Variables.allVariables(workspace);
        var inUse = false;
        for (var i = 0; i < variableList.length; i++) {
            if (variableList[i] === varName) {
                inUse = true;
                break;
            }
        }
        return inUse;
    },

    /**
       * Return all variables referenced by this block.
       * @return {!Array.<string>} List of variable names.
       * @this Blockly.Block
       */
    getVars: function () {
        return [this.getFieldValue("outVar")];
    },
    /**
     * Notification that a variable is renaming.
     * If the name matches one of this block's variables, rename it.
     * @param {string} oldName Previous name of variable.
     * @param {string} newName Renamed variable.
     * @this Blockly.Block
     */
    renameVar: function (oldName, newName) {
        if (this.getFieldValue("outVar")) {
            if (Blockly.Names.equals(oldName, this.getFieldValue("outVar"))) {
                this.setFieldValue(newName, "outVar");
            }
        }
    },

    /**
     * Update the block depending on the mutation
     * @this Blockly.Block
     */
    updateShape_: function () {
        if (this.mutationData_) {
            this.updateStoreVariableInput_(this.mutationData_.storeInVariable);
            this.updateAdditionalInputs_(this.mutationData_.additionalBlocks);
        } else {
            this.updateStoreVariableInput_(false);
            this.updateAdditionalInputs_();
        }
    },

    /**
     * Create XML to represent whether the number of mutator blocks to add
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function () {
        if (!this.mutationData_) {
            return null;
        }
        var container = document.createElement("mutation");
        container.setAttribute("storeInVariable".toLowerCase(), this.mutationData_.storeInVariable);

        $.each(this.mutationData_.additionalBlocks, function (index, blockType) {
            var addBlock = document.createElement("additional" + index);
            addBlock.setAttribute("type", blockType);
            container.appendChild(addBlock);
        });

        return container;
    },

    /**
     * Parse XML to restore the mutators blocks'.
     * @param {!Element} xmlElement XML storage element.
     * @this Blockly.Block
     */
    domToMutation: function (xmlElement) {
        this.mutationData_.storeInVariable = (xmlElement.getAttribute("storeInVariable".toLowerCase()) === "true");
        this.mutationData_.additionalBlocks = [];

        for (var i = 0, childNode; childNode = xmlElement.childNodes[i]; i++) {
			if (childNode.nodeName.toLowerCase().startsWith("additional")) {
                this.mutationData_.additionalBlocks.push(childNode.getAttribute("type"));
            }
        }

        this.updateShape_();
    },

    /**
    * Populate the mutator's dialog with this block's components.
    * @param {!Blockly.Workspace} workspace Mutator's workspace.
    * @return {!Blockly.Block} Root block in mutator.
    * @this Blockly.Block
    */
    decompose: function (workspace) {
       var topBlock = workspace.newBlock("yadoms_wait_for_event_base");
        topBlock.initSvg();

        var connection = topBlock.getInput("STACK").connection;

        if (this.mutationData_.storeInVariable === true) {
           var additionalBlock = workspace.newBlock("yadoms_wait_for_event_mutator_store_in_variable");
            additionalBlock.initSvg();
            connection.connect(additionalBlock.previousConnection);
            connection = additionalBlock.nextConnection;
        }

        $.each(this.mutationData_.additionalBlocks, function (index, blockType) {
            if (blockType != null && blockType !== "") {
               var additionalBlock = workspace.newBlock(blockType);
                additionalBlock.initSvg();
                connection.connect(additionalBlock.previousConnection);
                connection = additionalBlock.nextConnection;
            } else {
                console.error("Blockly : yadoms_wait_for_event : decompose : unknown block type : " + blockType);
            }
        });

        return topBlock;
    },

    /**
     * Remove any inputs
     * @private 
     */
    removeAllInputs_: function () {
        var inputVar = this.getInput("storeVariableInput");
        if (inputVar)
            this.removeInput("storeVariableInput");
        this.removeAllAdditionalInputs_();

        //remove any shadow item on workspace
        $.each(this.workspace.topBlocks_, function (index, block) {
            if (block && block.isShadow())
                block.dispose();
        });
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

        //remove any inputs
        this.removeAllInputs_();

        //reset mutation data
        this.mutationData_.storeInVariable = false;
        this.mutationData_.additionalBlocks = [];

        // Rebuild the block's optional inputs.
        var clauseBlock = containerBlock.getInputTargetBlock("STACK");
        var storeInVariableMoreThanOnce = false;
        var additionalBlockCount = 0;
        
        while (clauseBlock) {
            switch (clauseBlock.type.toLowerCase()) {
                case "yadoms_wait_for_event_mutator_store_in_variable":
                    if (this.mutationData_.storeInVariable === true)
                        storeInVariableMoreThanOnce = true;
                    this.mutationData_.storeInVariable = true;
                    this.updateStoreVariableInput_(this.mutationData_.storeInVariable, clauseBlock.storeVariableName_);
                    break;

                case "yadoms_wait_for_event_mutator_change":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendKeywordSelectorStatementChange_(additionalBlockCount, clauseBlock.part1DeviceId_, clauseBlock.part1KeywordId_, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                case "yadoms_wait_for_event_mutator_become":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendKeywordSelectorStatementBecome_(additionalBlockCount, clauseBlock.part1DeviceId_, clauseBlock.part1KeywordId_, clauseBlock.part1Operator_, clauseBlock.part1Connection_, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;
					
                case "yadoms_wait_for_event_mutator_capacity_change":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendCapacityStatementChange_(additionalBlockCount, clauseBlock.part1Capacity_, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                case "yadoms_wait_for_event_mutator_capacity_become":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendCapacityStatementBecome_(additionalBlockCount, clauseBlock.part1Capacity_, clauseBlock.part1Operator_, clauseBlock.part1Connection_, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                case "yadoms_wait_for_event_mutator_datetime_change":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendDatetimeStatementChange_(additionalBlockCount, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                case "yadoms_wait_for_event_mutator_datetime_become":
                    this.mutationData_.additionalBlocks.push(clauseBlock.type);
                    this.appendDatetimeStatementBecome_(additionalBlockCount, clauseBlock.part1Operator_, clauseBlock.part1Connection_, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                default:
                    throw "Unknown block type.";
            }
            clauseBlock = clauseBlock.nextConnection &&
                clauseBlock.nextConnection.targetBlock();
        }

        //remove any shadow item on workspace
        $.each(this.workspace.topBlocks_, function(index, block) {
            if (block && block.isShadow())
                block.dispose();
        });

        //manage warning display
        this.setWarningText(null);

        if (storeInVariableMoreThanOnce === true) {
            this.setWarningText($.t("blockly.blocks.yadoms_wait_for_event.mutator.error.storeInVariableMoreThanOne"));
        }

        if (additionalBlockCount === 0) {
            this.setWarningText($.t("blockly.blocks.yadoms_wait_for_event.mutator.error.noAddtitionalBlock"));
        }
    },

    getTargetConnectionIfNotShadow_: function (input) {

        if (input && input.connection) {
            if (input.connection.targetBlock() && !input.connection.targetBlock().isShadow())
                return input.connection.targetConnection;
        }
        return undefined;
    },
    /**
     * Store pointers to any connected child blocks.
     * @param {!Blockly.Block} containerBlock Root block in mutator.
     * @this Blockly.Block
     */
    saveConnections: function (containerBlock) {
        var clauseBlock = containerBlock.getInputTargetBlock('STACK');
        var i = 0;
        while (clauseBlock) {
            var inputPart1;
            var inputPart2;
            switch (clauseBlock.type.toLowerCase()) {
                case "yadoms_wait_for_event_mutator_change":
                    //save deviceId, KeywordId
                    clauseBlock.part1DeviceId_ = this.getFieldValue("deviceDd" + i);
                    clauseBlock.part1KeywordId_ = this.getFieldValue("keywordDd" + i);
                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2);

                    i++;
                    break;

                case "yadoms_wait_for_event_mutator_become":
                    //save deviceId, KeywordId, operator, value
                    inputPart1 = this.getInput("additionalInput_part1_" + i);

                    clauseBlock.part1DeviceId_ = this.getFieldValue("deviceDd" + i);
                    clauseBlock.part1KeywordId_ = this.getFieldValue("keywordDd" + i);
                    clauseBlock.part1Operator_ = this.getFieldValue("operatorDd" + i);
                    clauseBlock.part1Connection_ = this.getTargetConnectionIfNotShadow_(inputPart1); 

                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2); 
                    i++;
                    break;

                case "yadoms_wait_for_event_mutator_capacity_change":
                    //save deviceId, KeywordId
                    clauseBlock.part1Capacity_ = this.getFieldValue("capacityDd" + i);
                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2);

                    i++;
                    break;

                case "yadoms_wait_for_event_mutator_capacity_become":
                    //save deviceId, KeywordId, operator, value
                    inputPart1 = this.getInput("additionalInput_part1_" + i);

                    clauseBlock.part1Capacity_ = this.getFieldValue("capacityDd" + i);
                    clauseBlock.part1Operator_ = this.getFieldValue("operatorDd" + i);
                    clauseBlock.part1Connection_ = this.getTargetConnectionIfNotShadow_(inputPart1); 

                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2); 
                    i++;
                    break;
					
                case "yadoms_wait_for_event_mutator_datetime_change":
                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2);
                    i++;
                    break;

                case "yadoms_wait_for_event_mutator_datetime_become":
                    //save operator, connection
                    inputPart1 = this.getInput("additionalInput_part1_" + i);

                    clauseBlock.part1Operator_ = this.getFieldValue("operatorDd" + i);
                    clauseBlock.part1Connection_ = this.getTargetConnectionIfNotShadow_(inputPart1);

                    //save statement
                    inputPart2 = this.getInput("additionalInput_part2_" + i);
                    clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2);
                    i++;
                    break;


                case 'yadoms_wait_for_event_mutator_store_in_variable':
                    //save variable name
                    clauseBlock.storeVariableName_ = this.getFieldValue("outVar");
                    break;
                default:
                    throw 'Unknown block type.';
            }
            clauseBlock = clauseBlock.nextConnection &&
                clauseBlock.nextConnection.targetBlock();
        }
    },

    /**
    * Get the list of keywordId in a python compatible list
    * @returns {Array of Number} The list of keyword (ie: [1,5,6] )
    */
    getKeywordList: function () {
        //list all keyword id in a list
        var keywodArray = [];

        var i;
        var keyId;
        for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {
            keyId = this.getFieldValue("keywordDd" + i);
            if (keyId) {
                //check if not already added to list
                if ($.inArray(keyId, keywodArray) === -1) {
                    keywodArray.push(keyId);
                }
            }
        }
        return keywodArray;
    },    
	
	/**
    * Get the list of capacities in a python compatible list
    * @returns {Array of Number} The list of capacities name (ie: [temeprature,powerLvel,rssi] )
    */
    getCapacityList: function () {
        //list all capacities in a list
        var capacityArray = [];

        var i;
        var capacity;
        for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {
            capacity = this.getFieldValue("capacityDd" + i);
            if (capacity) {
                //check if not already added to list
                if ($.inArray(capacity, capacityArray) === -1) {
                    capacityArray.push(capacity);
                }
            }
        }
        return capacityArray;
    }

};


Blockly.Blocks["yadoms_wait_for_event_base"] = {
    /**
     * Mutator block for if container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.title"));
        this.appendStatementInput("STACK");
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.tooltip"));
        this.contextMenu = false;
    }
};

Blockly.Blocks["yadoms_wait_for_event_mutator_store_in_variable"] = {
    /**
     * Mutator block "store in variable".
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.storeInVariable.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.storeInVariable.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};



Blockly.Blocks["yadoms_wait_for_event_mutator_change"] = {
    /**
     * Mutator block for if container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.change.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.change.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};

Blockly.Blocks["yadoms_wait_for_event_mutator_become"] = {
    /**
     * Mutator block for if container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.become.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.become.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};

Blockly.Blocks["yadoms_wait_for_event_mutator_capacity_change"] = {
    /**
     * Mutator block for all keywords matching a capacity.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacityChange.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacityChange.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};

Blockly.Blocks["yadoms_wait_for_event_mutator_capacity_become"] = {
    /**
     * Mutator block for all keywords matching a capacity.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacityBecome.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacityBecome.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};


Blockly.Blocks["yadoms_wait_for_event_mutator_datetime_change"] = {
    /**
     * Mutator block for datet/time change container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeChange.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeChange.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};

Blockly.Blocks["yadoms_wait_for_event_mutator_datetime_become"] = {
    /**
     * Mutator block for date/time matching condition container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeBecome.title"));
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeBecome.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    }
};

