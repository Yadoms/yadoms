


/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks["yadoms_wait"] = {
    valueInput_: "timeToWait",
    init: function() {
        this.setHelpUrl("http://www.example.com/");
        this.setColour(Blockly.Yadoms.Dates.datetime.HUE);

        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);

        this.appendValueInput(this.valueInput_)
			.setCheck(["timespan"])
            .appendField($.t("blockly.blocks.yadoms_wait.title"));
		this.setTooltip($.t("blockly.blocks.yadoms_wait.tooltip", { defaultValue: "" }));			
    }
};



















//
//
///**
// * Define a "FOR" mutator block for yadoms_logic_compare_is
// */
//Blockly.Blocks["yadoms_logic_compare_is_mutator_for"] = {
//    init: function () {
//        this.setColour(210);
//        this.setInputsInline(true);
//        this.setPreviousStatement(true, "null");
//        this.setNextStatement(false, "null");
//        this.setTooltip($.t("blockly.blocks.yadoms_logic_compare_is.for.tooltip", { defaultValue: "" }));
//        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_logic_compare_is.for.title_long"));
//        //noinspection JSValidateTypes
//        this.contextMenu = false;
//    }
//};
///**
// * Define a "AT" mutator block for yadoms_logic_compare_is
// */
//Blockly.Blocks["yadoms_logic_compare_is_mutator_at"] = {
//    init: function () {
//        this.setColour(210);
//        this.setInputsInline(true);
//        this.setPreviousStatement(true, "null");
//        this.setNextStatement(false, "null");
//        this.setTooltip($.t("blockly.blocks.yadoms_logic_compare_is.at.tooltip", { defaultValue: "" }));
//        this.appendDummyInput().appendField($.t("blockly.blocks.yadoms_logic_compare_is.at.title_long"));
//        //noinspection JSValidateTypes
//        this.contextMenu = false;
//    }
//};
///**
// * Define the base mutator block for yadoms_logic_compare_is
// */
//Blockly.Blocks["yadoms_logic_compare_is_mutator"] = {
//    init: function () {
//        this.setColour(Blockly.Blocks.logic.HUE);
//        this.appendDummyInput()
//            .appendField($.t("blockly.blocks.yadoms_logic_compare_is.title"));
//        this.appendStatementInput("STACK");
//        //noinspection JSValidateTypes
//        this.contextMenu = false;
//    }
//};
//
//
///**
// * Define a condition block which is true when a keyword match a value (not a front value)
// * @type {{init: Function}}
// */
//Blockly.Blocks["yadoms_logic_compare_is"] = {
//    unitsInputName: "units",
//    init: function () {
//        this.setHelpUrl("http://www.example.com/");
//        this.setColour(210);
//        this.appendDummyInput()
//            .appendField($.t("blockly.blocks.yadoms_logic_compare_is.title"));
//        this.appendValueInput("A");
//        this.appendValueInput("B").appendField(new Blockly.FieldDropdown(Blockly.Yadoms.NumberOperators_), "OP");
//        this.appendDummyInput().appendField(" ", "unit");
//
//        this.setInputsInline(true);
//        this.setOutput(true, "Boolean");
//
//        this.setMutator(new Blockly.Mutator(["yadoms_logic_compare_is_mutator_for", "yadoms_logic_compare_is_mutator_at"]));
//
//        var thisBlock = this;
//
//        this.durationUnitsEnum = [
//            [$.t("blockly.global.durationUnits.seconds"), "S"],
//            [$.t("blockly.global.durationUnits.minutes"), "M"],
//            [$.t("blockly.global.durationUnits.hours"), "H"],
//            [$.t("blockly.global.durationUnits.days"), "D"]
//        ];
//
//        this.setTooltip(function () {
//            var op = thisBlock.getFieldValue("OP");
//            var tooltips = {
//                'EQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_EQ,
//                'NEQ': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_NEQ,
//                'LT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LT,
//                'LTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_LTE,
//                'GT': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GT,
//                'GTE': Blockly.Msg.LOGIC_COMPARE_TOOLTIP_GTE
//            };
//            return $.t("blockly.blocks.yadoms_logic_compare_is.tooltip", { defaultValue: "", operand: tooltips[op] });
//        });
//
//
//        this.isForMutator = false;
//        this.isAtMutator = false;
//
//        /**
//         * Method which update the operator list, according to the connected types
//         */
//        this.updateOperator = function (newCheck) {
//            var selectedOperators;
//            if (newCheck === "String") {
//                selectedOperators = Blockly.Yadoms.StringOperators_;
//            } else if (newCheck === "Boolean") {
//                selectedOperators = Blockly.Yadoms.BooleanOperators_;
//            } else if (newCheck === "Number") {
//                selectedOperators = Blockly.Yadoms.NumberOperators_;
//            } else if (newCheck === "Any" || newCheck == null) {
//                selectedOperators = Blockly.Yadoms.NumberOperators_;
//            } else {
//                //specific types
//                selectedOperators = Blockly.Yadoms.BooleanOperators_;
//            }
//
//            var operatorDropDown = $.isFunction(this.getField_) ? this.getField_("OP") : this.getField("OP");
//
//            //before replacing menuGenerator, get value to restore it after menu update
//            var oldValue = operatorDropDown.getValue();
//            operatorDropDown.menuGenerator_ = selectedOperators;
//            operatorDropDown.setValue(oldValue || selectedOperators[0][1]);
//            operatorDropDown.updateTextNode_();
//        };
//
//        //apply operator update
//        thisBlock.updateOperator(null);
//
//        /**
//         * Update the block unit
//         * @param keyword The selected keyword
//         */
//        this.updateUnit = function (keyword) {
//            thisBlock.setFieldValue(" ", "unit");
//            if (!isNullOrUndefined(keyword) && !isNullOrUndefinedOrEmpty(keyword.units)) {
//                thisBlock.setFieldValue(" " + $.t(keyword.units), "unit");
//            }
//
//        };
//
//        /**
//         * Method which make type checks when one of connector changes
//         */
//        this.onchange = function () {
//
//            //check for A and B connected blocks
//            var inputValueA = thisBlock.getInput("A");
//            var inputValueB = thisBlock.getInput("B");
//            if (!inputValueA || !inputValueB) {
//                return;  // Blocks under construction, ignore.
//            }
//            var blockA = inputValueA.connection.targetBlock();
//            var blockB = inputValueB.connection.targetBlock();
//
//            var keywordValue = undefined;
//            var deviceValue = undefined;
//            var pluginValue = undefined;
//            if (blockA && blockA.type === "keyword-value-get") {
//                keywordValue = blockA.getSelectedKeyword();
//                deviceValue = blockA.getSelectedDevice();
//                pluginValue = blockA.getSelectedPlugin();
//            }
//            if (blockB && blockB.type === "keyword-value-get") {
//                keywordValue = blockB.getSelectedKeyword();
//                deviceValue = blockB.getSelectedDevice();
//                pluginValue = blockB.getSelectedPlugin();
//            }
//
//            //update yadoms_enumeration if any
//            if (blockA && blockB) {
//                if (blockA.type === "keyword-value-get" && blockB.type === "yadoms_enumeration") {
//                    blockB.updateEnumeration(keywordValue, deviceValue, pluginValue);
//                } else if (blockB.type === "keyword-value-get" && blockA.type === "yadoms_enumeration") {
//                    blockA.updateEnumeration(keywordValue, deviceValue, pluginValue);
//                }
//            }
//
//            //update units
//            if (keywordValue != undefined)
//                thisBlock.updateUnit(Blockly.Yadoms.data.keywords[keywordValue]);
//            else
//                thisBlock.updateUnit(undefined);
//
//
//            //update checks
//            if (blockA) {
//                if (inputValueB.connection.check_ !== blockA.outputConnection.check_) {
//                    inputValueB.setCheck(blockA.outputConnection.check_);
//                    thisBlock.updateOperator(blockA.outputConnection.check_);
//                }
//            } else if (blockB) {
//                if (inputValueA.connection.check_ !== blockB.outputConnection.check_) {
//                    inputValueA.setCheck(blockB.outputConnection.check_);
//                    thisBlock.updateOperator(blockB.outputConnection.check_);
//                }
//            } else {
//                inputValueA.setCheck(null);
//                inputValueB.setCheck(null);
//            }
//        };
//
//    },
//    isAtMutator: false,
//    isForMutator: false,
//    /**
//     * Create XML to represent whether the 'divisorInput' should be present.
//     * @return {Element} XML storage element.
//     * @this Blockly.Block
//     */
//    mutationToDom: function () {
//        var container = document.createElement("mutation");
//        container.setAttribute("for_mutator", this.isForMutator);
//        container.setAttribute("at_mutator", this.isAtMutator);
//        return container;
//    },
//    /**
//     * Parse XML to restore the 'divisorInput'.
//     * @param {!Element} xmlElement XML storage element.
//     * @this Blockly.Block
//     */
//    domToMutation: function (xmlElement) {
//        var forMutator = (xmlElement.getAttribute("for_mutator") === "true");
//        var atMutator = (xmlElement.getAttribute("at_mutator") === "true");
//        this.updateShape_(forMutator, atMutator);
//    },
//
//
//
//    updateShape_: function (forMutator, atMutator) {
//        // Disconnect all
//        if (this.isForMutator) {
//            if (this.getInput("duration")) {
//                this.removeInput("duration_text");
//                this.removeInput("duration");
//                this.removeInput("duration_unit");
//            }
//        }
//        if (this.isAtMutator) {
//            if (this.getInput("at")) {
//                this.removeInput("at_text");
//                this.removeInput("at");
//            }
//        }
//        this.isForMutator = false;
//        this.isAtMutator = false;
//
//
//
//        if (forMutator) {
//            this.appendDummyInput("duration_text").appendField($.t("blockly.blocks.yadoms_logic_compare_is.for.title"));
//            this.appendDummyInput("duration").appendField(new Blockly.FieldTextInput("0", Blockly.FieldTextInput.numberValidator), "duration").appendField(new Blockly.FieldDropdown(this.durationUnitsEnum), "durationUnit");
//            this.isForMutator = true;
//        } else if (atMutator) {
//            this.appendDummyInput("at_text").appendField($.t("blockly.blocks.yadoms_logic_compare_is.at.title"));
//            this.appendDummyInput("at")
//                .appendField(new Blockly.FieldTextInput("00:00", function (text) {
//                    if (text.match(/^([01]?[0-9]|2[0-3]):[0-5][0-9]/))
//                        return text;
//                    return "00:00";
//                }), "at");
//            this.isAtMutator = true;
//        }
//
//
//    },
//    decompose: function (workspace) {
//        var topBlock = Blockly.Block.obtain(workspace, "yadoms_logic_compare_is_mutator");
//        topBlock.initSvg();
//
//        var connection = topBlock.getInput("STACK").connection;
//
//        if (this.isForMutator) {
//            var forBlock = Blockly.Block.obtain(workspace, "yadoms_logic_compare_is_mutator_for");
//            forBlock.initSvg();
//            connection.connect(forBlock.previousConnection);
//        } else if (this.isAtMutator) {
//            var atBlock = Blockly.Block.obtain(workspace, "yadoms_logic_compare_is_mutator_at");
//            atBlock.initSvg();
//            connection.connect(atBlock.previousConnection);
//        }
//        return topBlock;
//    },
//    /**
//     * Reconfigure this block based on the mutator dialog's components.
//     * @param {!Blockly.Block} containerBlock Root block in mutator.
//     * @this Blockly.Block
//     */
//    compose: function (containerBlock) {
//        // Rebuild the block's optional inputs.
//
//        //get the mutator dialog result
//        var clauseBlock = containerBlock.getInputTargetBlock("STACK");
//
//        var forMutator = false;
//        var atMutator = false;
//        if (clauseBlock) {
//            if (clauseBlock.type === "yadoms_logic_compare_is_mutator_for") {
//                forMutator = true;
//            } else if (clauseBlock.type === "yadoms_logic_compare_is_mutator_at") {
//                atMutator = true;
//            }
//        }
//        this.updateShape_(forMutator, atMutator);
//    }
//};
//
//
//
///**
// * Define the python generation function for yadoms_logic_compare_is block
// * @param block The block
// * @return {*[]}
// */
//Blockly.Python["yadoms_logic_compare_is"] = function (block) {
//
//    if (block.isForMutator || block.isAtMutator) {
//        if (Blockly.Yadoms.CurrentGenerationBlockIsTimed_)
//            assert(false, "Python : yadoms_logic_compare_is : cannont manage multiple yadoms_logic_compare_is blocks in the same condition");
//    }
//
//    // Comparison operator.
//    var operators = {
//        'EQ': "==",
//        'NEQ': "!=",
//        'LT': "<",
//        'LTE': "<=",
//        'GT': ">",
//        'GTE': ">="
//    };
//
//    var operator = operators[block.getFieldValue("OP")];
//    var order = Blockly.Python.ORDER_RELATIONAL;
//    var code = "";
//    var argument0;
//    var argument1;
//    if (block.isForMutator) {
//        //TODO Python yadoms_logic_compare_is : FOR mutation generation not implemented");
//        var duration = block.getFieldValue("duration");
//        var durationUnit = block.getFieldValue("durationUnit");
//
//        var timeToCheck;
//        var intervallBetweenChecks;
//        switch (durationUnit) {
//            case "S":
//                timeToCheck = duration;
//                intervallBetweenChecks = 1; //1 sec
//                break;
//            case "M":
//                timeToCheck = duration * 60;
//                intervallBetweenChecks = 30; //30 sec
//                break;
//            case "H":
//                timeToCheck = duration * 60 * 60;
//                intervallBetweenChecks = 120; //2 min
//                break;
//            case "D":
//                timeToCheck = duration * 60 * 60 * 24;
//                intervallBetweenChecks = 1800; //30 min
//                break;
//        }
//
//        var startDate = new Date();
//        var endDate = new Date(Date.now() - (timeToCheck * 1000));
//
//        var completeCode = "";
//        Blockly.Yadoms.CurrentGenerationBlockIsTimed_ = true;
//        for (var dt = startDate; dt >= endDate; dt = new Date(dt.getTime() - (intervallBetweenChecks * 1000))) { //30sec
//            Blockly.Yadoms.CurrentGenerationBlockTime_ = dt;
//
//            if (completeCode !== "")
//                completeCode += " and ";
//            argument0 = Blockly.Python.valueToCode(block, "A", order) || "0";
//            argument1 = Blockly.Python.valueToCode(block, "B", order) || "0";
//            completeCode += "(" + argument0 + " " + operator + " " + argument1 + ")";
//        }
//        Blockly.Yadoms.CurrentGenerationBlockIsTimed_ = false;
//        code = "(" + completeCode + ")";
//
//    } else if (block.isAtMutator) {
//        Blockly.Yadoms.CurrentGenerationBlockIsTimed_ = true;
//        var time = block.getFieldValue("at");
//        Blockly.Yadoms.CurrentGenerationBlockTime_ = new Date();
//        Blockly.Yadoms.CurrentGenerationBlockTime_.setHours(time.split(":")[0]);
//        Blockly.Yadoms.CurrentGenerationBlockTime_.setMinutes(time.split(":")[1]);
//        Blockly.Yadoms.CurrentGenerationBlockTime_.setSeconds(0);
//        Blockly.Yadoms.CurrentGenerationBlockTime_.setMilliseconds(0);
//        argument0 = Blockly.Python.valueToCode(block, "A", order) || "0";
//        argument1 = Blockly.Python.valueToCode(block, "B", order) || "0";
//        code = argument0 + " " + operator + " " + argument1;
//
//        Blockly.Yadoms.CurrentGenerationBlockIsTimed_ = false;
//    } else {
//        argument0 = Blockly.Python.valueToCode(block, "A", order) || "0";
//        argument1 = Blockly.Python.valueToCode(block, "B", order) || "0";
//        code = argument0 + " " + operator + " " + argument1;
//    }
//
//
//    return [code, order];
//};


