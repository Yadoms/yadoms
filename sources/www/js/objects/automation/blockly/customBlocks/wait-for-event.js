/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
 
 
Blockly.Blocks['yadoms_wait_for_event'] = {
  /**
   * Block for sorting a list.
   * @this Blockly.Block
   */
  init: function() {
    this.jsonInit({
       "message0": $.t("blockly.blocks.yadoms_wait_for_event.title"),
       "nextStatement": true,
       "previousStatement": true,
       "colour": Blockly.Yadoms.blockColour.HUE,
       "tooltip": $.t("blockly.blocks.yadoms_wait_for_event.tooltip"),
       "helpUrl": Blockly.Yadoms.GenerateHelpUrl("yadoms_wait_for_event"),
       "mutator": "yadoms_wait_for_event_mutator",
       "inputsInline" : false,
       "extensions": ["yadoms_wait_for_event_extension"]       
    });
    
    this.mutationData_ = { additionalBlocks: [] };
  }
}


 Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_MUTATOR_MIXIN = {
    mutationData_: { additionalBlocks: [] },   

    /**
     * Create XML to represent whether the number of mutator blocks to add
     * @return {Element} XML storage element.
     * @this Blockly.Block
     */
    mutationToDom: function () {
        if (!this.mutationData_) {
            return null;
        }
		var self= this;
        var container = document.createElement("mutation");
        $.each(this.mutationData_.additionalBlocks, function (index, blockInfo) {
            var addBlock = document.createElement("additional" + index);
            addBlock.setAttribute("type", blockInfo.type);
			addBlock.setAttribute("condition", blockInfo.condition);
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
        this.mutationData_.additionalBlocks = [];
        for (var i = 0, childNode; childNode = xmlElement.childNodes[i]; i++) {
			if (childNode.nodeName.toLowerCase().startsWith("additional")) {
				
				//childNode.getAttribute("condition")
                this.mutationData_.additionalBlocks.push( { type : childNode.getAttribute("type"), condition : childNode.getAttribute("condition") });
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

		//take STACK element
		connection =topBlock.getInput("STACK").connection;

		var self = this;
        $.each(this.mutationData_.additionalBlocks, function (index, blockInfo) {
            if (blockInfo && blockInfo.type && blockInfo.type !== "") {
                var additionalBlock = workspace.newBlock(self.adaptBlockType(blockInfo.type));
                additionalBlock.initSvg();
				if(blockInfo.condition && (blockInfo.condition === "true" || blockInfo.condition === true))
					additionalBlock.checkCondition();
                connection.connect(additionalBlock.previousConnection);
                connection = additionalBlock.nextConnection;
            } else {
                console.error("Blockly : yadoms_wait_for_event : decompose : unknown block type : " + blockInfo.type);
            }
        });

        return topBlock;
    },
    

    /**
     * Reconfigure this block based on the mutator dialog's components.
     * @param {!Blockly.Block} containerBlock Root block in mutator.
     * @this Blockly.Block
     */
    compose: function (containerBlock) {
        //This method analyse the mutator dialog result, then recreate the real block
        //so it takes sure clauseBlock (= mutator topblock stack = all additional keywords)
        //foreach additional keyword block found, just append needed blocks

        //remove any inputs
        this.removeAllInputs_();
        this.clearValidationFunction();
        
        //reset mutation data
        this.mutationData_.additionalBlocks = [];

        // Rebuild the block's optional inputs.
        var clauseBlock = containerBlock.getInputTargetBlock("STACK");
		
        var additionalBlockCount = 0;
        
        while (clauseBlock) {
            switch (clauseBlock.type.toLowerCase()) {
				//keep this case for backward compatibility
                case "yadoms_wait_for_event_mutator_store_in_variable":
                    break;
					
                case "yadoms_wait_for_event_mutator_keyword":
					if($.isFunction(clauseBlock.hasCondition) && clauseBlock.hasCondition()) {
						this.mutationData_.additionalBlocks.push( { type : "yadoms_wait_for_event_mutator_become", condition : true});
						this.appendKeywordSelectorStatementBecome_(additionalBlockCount, clauseBlock.part1DeviceId_, clauseBlock.part1KeywordId_, clauseBlock.part1Operator_, clauseBlock.part1Connection_, clauseBlock.part2Connection_);
					}
					else {
						this.mutationData_.additionalBlocks.push( { type : "yadoms_wait_for_event_mutator_change", condition : false});
						this.appendKeywordSelectorStatementChange_(additionalBlockCount, clauseBlock.part1DeviceId_, clauseBlock.part1KeywordId_, clauseBlock.part2Connection_);
					}
					additionalBlockCount++;
					break;
					
                case "yadoms_wait_for_event_mutator_capacity":
					if($.isFunction(clauseBlock.hasCondition) && clauseBlock.hasCondition()) {
						this.mutationData_.additionalBlocks.push( { type : "yadoms_wait_for_event_mutator_capacity_become", condition : true});
						this.appendCapacityStatementBecome_(additionalBlockCount, clauseBlock.part1Capacity_, clauseBlock.part1Operator_, clauseBlock.part1Connection_, clauseBlock.part2Connection_);
					}
					else {	
						this.mutationData_.additionalBlocks.push( { type : "yadoms_wait_for_event_mutator_capacity_change", condition : false});
						this.appendCapacityStatementChange_(additionalBlockCount, clauseBlock.part1Capacity_, clauseBlock.part2Connection_);
					}
					additionalBlockCount++;
					break;
					
                case "yadoms_wait_for_event_mutator_datetime_change":
					this.mutationData_.additionalBlocks.push( { type : clauseBlock.type, condition : false});
                    this.appendDatetimeStatementChange_(additionalBlockCount, clauseBlock.part2Connection_);
                    additionalBlockCount++;
                    break;

                case "yadoms_wait_for_event_mutator_datetime_become":
					this.mutationData_.additionalBlocks.push( { type : clauseBlock.type, condition : false});
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
            if (block && block.isShadow()) {
				try {
					block.dispose();
				}catch(e) {
					try {
						block.dispose();
					}catch(e) {
					}
				}
				
			}
        });

        //manage warning display
        this.setWarningText(null);

        if (additionalBlockCount === 0) {
            this.setWarningText($.t("blockly.blocks.yadoms_wait_for_event.mutator.error.noAddtitionalBlock"));
        }
    },
    

    /**
     * Store pointers to any connected child blocks.
     * @param {!Blockly.Block} containerBlock Root block in mutator.
     * @this Blockly.Block
     */
    saveConnections: function (containerBlock) {
        var clauseBlock = containerBlock.getInputTargetBlock("STACK");
		var i = 0;
		while (clauseBlock) {
			var inputPart1;
			var inputPart2;
			switch (clauseBlock.type.toLowerCase()) {
				case "yadoms_wait_for_event_mutator_keyword":
				
					clauseBlock.part1DeviceId_ = this.getFieldValue("deviceDd" + i);
					clauseBlock.part1KeywordId_ = this.getFieldValue("keywordDd" + i);
					if($.isFunction(clauseBlock.hasCondition) && clauseBlock.hasCondition()) {
						//save deviceId, KeywordId, operator, value
						inputPart1 = this.getInput("additionalInput_part1_" + i);
						clauseBlock.part1Operator_ = this.getFieldValue("operatorDd" + i);
						clauseBlock.part1Connection_ = this.getTargetConnectionIfNotShadow_(inputPart1); 
					}

				
					//save statement
					inputPart2 = this.getInput("additionalInput_part2_" + i);
					clauseBlock.part2Connection_ = this.getTargetConnectionIfNotShadow_(inputPart2);

					i++;
					break;



				case "yadoms_wait_for_event_mutator_capacity":
				
					//save capacity
					clauseBlock.part1Capacity_ = this.getFieldValue("capacityDd" + i);

					if($.isFunction(clauseBlock.hasCondition) && clauseBlock.hasCondition()) {
						//save deviceId, KeywordId, operator, value
						inputPart1 = this.getInput("additionalInput_part1_" + i);
						clauseBlock.part1Capacity_ = this.getFieldValue("capacityDd" + i);
						clauseBlock.part1Operator_ = this.getFieldValue("operatorDd" + i);
						clauseBlock.part1Connection_ = this.getTargetConnectionIfNotShadow_(inputPart1); 
					}

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

				default:
					throw 'Unknown block type.';
			}
			
			clauseBlock = clauseBlock.nextConnection &&
				clauseBlock.nextConnection.targetBlock();
		}
    },
    
    /**
     * Update the block depending on the mutation
     * @this Blockly.Block
     */
    updateShape_: function () {
        if (this.mutationData_) {
            this.updateAdditionalInputs_(this.mutationData_.additionalBlocks);
        } else {
            this.updateAdditionalInputs_();
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
            $.each(inputList, function (index, inputInfo) {
                switch(inputInfo.type.toLowerCase()) {
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
        var fdd = new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_);
        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, false, ["numeric", "string", "bool", "datetime", "enum"], undefined, function (keyword, type) { self.onKeywordChange_(no, keyword, type); }, "deviceDd" + no, "keywordDd" + no, bValueInput, "", self.workspace)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredType.become"))
        .appendField(fdd, "operatorDd"+no);
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
				try {
					bValueInput.connection.targetBlock().dispose();
				}catch(e) {
					try {
						bValueInput.connection.targetBlock().dispose();
					}catch(e) {
					}
				}
            }

            bValueInput.connection.connect(valueConnection);
        }

        this.appendInputPart2_(no, statementConnection);
        
        //dropdown must be initialized before returing this function
        fdd.init();
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
		
		if (capacity) {
            var capacityDd = this.getField("capacityDd" + no);
            capacityDd.setValue(capacity);
        }
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
        var fdd = new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_);
        Blockly.Yadoms.ConfigureBlockForYadomsCapacitySelection(this, function (newCapacity, type) { self.onCapacityChange_(no, newCapacity, type); }, "capacityDd" + no, bValueInput, "", self.workspace)
        .appendField($.t("blockly.blocks.yadoms_wait_for_event.triggeredCapacityType.become"))
        .appendField(fdd, "operatorDd"+no);
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
				try {
					bValueInput.connection.targetBlock().dispose();
				}catch(e) {
					try {
						bValueInput.connection.targetBlock().dispose();
					}catch(e) {
					}
				}
            }

            bValueInput.connection.connect(valueConnection);
        }

        this.appendInputPart2_(no, statementConnection);
        
        //dropdown must be initialized before returing this function
        fdd.init();
    },

	
	/**
     * Append a date/time change block
     * @param {} no The input number
     * @param {} statementConnection The statemenet connection (can be null/undefined)
     * @private 
     */
    appendDatetimeStatementChange_: function (no, statementConnection) {
        this.appendDummyInput("additionalInput_part1_" + no)
            .appendField($.t("blockly.blocks.yadoms_wait_for_event.caseDateTimeEveryMinute"))
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
       
      var fdd = new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_);
       
      var bValueInput = this.appendValueInput("additionalInput_part1_" + no)
                            .setCheck(["datetime", "time", "date"])
                            .appendField($.t("blockly.blocks.yadoms_wait_for_event.caseDateTimeCondition"))
                            .appendField(fdd, "operatorDd" + no);

        bValueInput.setForceNewlineInput(true);

        if (operator) {
            var opDd = this.getField("operatorDd" + no);
            if(opDd)
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
        
        //dropdown must be initialized before returning
        fdd.init();
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
	 * Function which translate old block types to new ones
	 */
	adaptBlockType : function(type) {
		switch(type) {
			case "yadoms_wait_for_event_mutator_change":
			case "yadoms_wait_for_event_mutator_become":
				return "yadoms_wait_for_event_mutator_keyword";
			
			case "yadoms_wait_for_event_mutator_capacity_change":
			case "yadoms_wait_for_event_mutator_capacity_become":
				return "yadoms_wait_for_event_mutator_capacity";
				
			default:
				//do not make translation
				return type;
		}
	},
	

    /**
     * Remove any inputs
     * @private 
     */
    removeAllInputs_: function () {
		this.removeAllAdditionalInputs_();

        //remove any shadow item on workspace
        $.each(this.workspace.topBlocks_, function (index, block) {
            if (block && block.isShadow()) {
				try {
					block.dispose();
				}catch(e) {
					try {
						block.dispose();
					}catch(e) {
					}
				}
			}
        });
    },

    /**
     * Tells if an indexed input is configured with a condition
     * @param {Number} inputNumber The indexed input number
     * @return {Boolean} true is the input[inputNumber] is configured with a condition
     */
	hasConditionForInput : function(inputNumber) {
	   return this.getField("operatorDd" + inputNumber) !== null;
	},
	
	
    getTargetConnectionIfNotShadow_: function (input) {

        if (input && input.connection) {
            if (input.connection.targetBlock() && !input.connection.targetBlock().isShadow())
                return input.connection.targetConnection;
        }
        return undefined;
    },
    
    validationArray: [],

	 /**
     * Add a validation function (usefull for mutators subblocks)
     * @param {Function} fct : The function to add
     */    
    addValidationFunction: function(fct) {
       this.validationArray.push(fct);
    },
	 /**
     * Clear all validation functions
     */    
    clearValidationFunction: function(fct) {
       this.validationArray = [];
    },
	 /**
     * Check if current block is valid (ask for every registered functions = ask for every mutator case)
     */    
    isValid: function () {
       if(this.validationArray) {
          for(var i in this.validationArray) {
              if ($.isFunction(this.validationArray[i])) {
                  var bvr = this.validationArray[i]();
                  if(bvr.isValid === false) {
                     return bvr;
                  }
              }          
          }
       }
       return {
			isValid : true
		 };       
    },

    
    /**
     * When something changes on the block, check if any enumeration block has been connected, and then update types
     */
    onchange: function () {
        //browse all become inputs
        //if a connected block is an enuemration, then update enum matching the selected keyword
        var self = this;
        $.each(this.mutationData_.additionalBlocks, function (index, blockInfo) {
            if (blockInfo.condition || blockInfo === "yadoms_wait_for_event_mutator_become" || blockInfo === "yadoms_wait_for_event_mutator_capacity_become") {
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

            //ensure selected operator is still available for current possible ones
            var availableValue = undefined;
            selectedOperators.forEach(function(op) {
                if(oldValue === op[1])
                    availableValue = op[1];
            });

            operatorDropDown.setValue(availableValue || selectedOperators[0][1]);
            //operatorDropDown.updateTextNode_();
        }
    }    
 };
 

 
Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_PUBLIC_MIXIN = { 
     setBreakVariableName: function(v) {
        this.wantToBreak = v;
     },
     setContinueVariableName: function(v) {
        this.wantToContinue = v;
     },
     getBreakVariableName: function() {
        return this.wantToBreak;
     },
     getContinueVariableName: function() {
        return this.wantToContinue ;
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

        //create variable
        workspace.createVariable(name);
        
        Blockly.Yadoms.InternalTemporaryVariable.push(name);
        
        
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
       return workspace.getVariable(varName) != null;
    },

    /**
       * Return all variables referenced by this block.
       * @return {!Array.<string>} List of variable names.
       * @this Blockly.Block
       */
    getVars: function () {
        return [this.getFieldValue("outVar"), this.getFieldValue("outDeviceVar")];
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
        if (this.getFieldValue("outDeviceVar")) {
            if (Blockly.Names.equals(oldName, this.getFieldValue("outDeviceVar"))) {
                this.setFieldValue(newName, "outDeviceVar");
            }
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
        var capacityArray = {};

        var i;
        var capacity;
        for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {
            capacity = this.getFieldValue("capacityDd" + i);
            if (capacity) {
                //check if not already added to list
                if ($.inArray(capacity, capacityArray) === -1) {
                    capacityArray[i] = capacity;
                }
            }
        }
        return capacityArray;
    }    
};
 
 Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_ONCHANGE_MIXIN = {

 };

 
 /**
 * "logic_compare" extension function. Corrects direction of operators in the
 * dropdown labels, and adds type left and right side type checking to
 * "logic_compare" blocks.
 * @this Blockly.Block
 * @package
 * @readonly
 */
Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_EXTENSION = function() {
  // Add onchange handler to ensure types are compatable.
  this.mixin(Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_ONCHANGE_MIXIN);
  this.mixin(Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_PUBLIC_MIXIN, true);
};

 



Blockly.Blocks["yadoms_wait_for_event_base"] = {
    /**
     * Mutator block for if container.
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput()
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.title"));
			
        this.appendStatementInput("STACK");

        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.tooltip"));
        this.contextMenu = false;
    }
};



Blockly.Blocks["yadoms_wait_for_event_mutator_keyword"] = {
	mutatorTypeCheckboxField : "mutatorTypeCheckbox",
    /**
     * Mutator block for a keyword new acquisition
     * @this Blockly.Block
     */
    init: function () {
		var checkboxValue = 'FALSE';
		if(this.initValue) {
			checkboxValue = 'TRUE';
		}
		this.checkBox = new Blockly.FieldCheckbox(checkboxValue);
		
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput()
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.keyword.title"));
		this.appendDummyInput()
			.appendField("   ")
			.appendField(this.checkBox, this.mutatorTypeCheckboxField)
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.keyword.condition"));
			
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.keyword.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    },
	

    /**
     * Tells if the condition checkbox is checked
     */
	hasCondition : function() {
		return this.getFieldValue(this.mutatorTypeCheckboxField) === "TRUE";
	},
	/**
     * Setup the condition
     */
	checkCondition : function() {
		this.initValue = true;
		if(this.checkBox) {
			this.checkBox.setValue('TRUE');
		}		
	}
};

Blockly.Blocks["yadoms_wait_for_event_mutator_capacity"] = {
	mutatorTypeCheckboxField : "mutatorTypeCheckbox",
    /**
     * Mutator block for a capacity new acquisition
     * @this Blockly.Block
     */
    init: function () {
		var checkboxValue = 'FALSE';
		if(this.initValue) {
			checkboxValue = 'TRUE';
		}

		this.checkBox = new Blockly.FieldCheckbox(checkboxValue);
		
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput()
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacity.title"));
		this.appendDummyInput()
			.appendField("   ")
			.appendField(this.checkBox, this.mutatorTypeCheckboxField)
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacity.condition"));
			
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.capacity.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    },
	

    /**
     * Tells if the condition checkbox is checked
     */
	hasCondition : function() {
		return this.getFieldValue(this.mutatorTypeCheckboxField) === "TRUE";
	},
	/**
     * Setup the condition
     */
	checkCondition : function() {
		this.initValue = true;
		if(this.checkBox) {
			this.checkBox.setValue('TRUE');
		}
	}
};

Blockly.Blocks["yadoms_wait_for_event_mutator_datetime"] = {
	mutatorTypeDropdownField : "mutatorTypeCheckbox",
    /**
     * Mutator block for a datetime
     * @this Blockly.Block
     */
    init: function () {
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendDummyInput()
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetime.title"));
		this.appendDummyInput()
			.appendField("   ")
			.appendField(new Blockly.FieldCheckbox('FALSE'), this.mutatorTypeCheckbox)
			.appendField($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetime.condition"));
			
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.datetime.tooltip"));
        this.contextMenu = false;
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
    },
	

    /**
     * Get the chosen type of dropdown
     */
	getChosenType : function() {
		return this.getFieldValue(this.mutatorTypeDropdownField);
	}
};

 


Blockly.defineBlocksWithJsonArray([ // Mutator blocks. Do not extract.
  // Mutator block for datet/time change container.
  {
    "type": "yadoms_wait_for_event_mutator_datetime_change",
    "message0": $.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeChange.title"),
    "previousStatement": true,
    "nextStatement": true,
    "enableContextMenu": false,
    "colour": Blockly.Yadoms.blockColour.HUE,
    "tooltip": $.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeChange.tooltip")
  },

  // Mutator block for date/time matching condition container.
  {
    "type": "yadoms_wait_for_event_mutator_datetime_become",
    "message0": $.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeBecome.title"),
    "previousStatement": true,
    "nextStatement": true,
    "enableContextMenu": false,
    "colour": Blockly.Yadoms.blockColour.HUE,
    "tooltip": $.t("blockly.blocks.yadoms_wait_for_event.mutator.datetimeBecome.tooltip")
  }
]);

Blockly.Extensions.register('yadoms_wait_for_event_extension',
  Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_EXTENSION);
 
 Blockly.Extensions.registerMutator('yadoms_wait_for_event_mutator',
    Blockly.Yadoms.YADOMS_WAIT_FOR_EVENT_MUTATOR_MIXIN, null,
    ['yadoms_wait_for_event_mutator_keyword',
     'yadoms_wait_for_event_mutator_capacity',
     'yadoms_wait_for_event_mutator_datetime_change',
     'yadoms_wait_for_event_mutator_datetime_become']);
     