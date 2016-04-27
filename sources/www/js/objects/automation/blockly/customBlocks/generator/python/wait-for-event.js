/**
 * Define the python generation function for yadoms_wait_for_event block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait_for_event"] = function (block) {
    var order = Blockly.Python.ORDER_RELATIONAL;

    //list all keyword id in a list
    var kwList = Blockly.Yadoms.Python.listToStringArray(block.getKeywordList());

    //list all capacities in a list
    var capacitiyList = Blockly.Yadoms.Python.listToStringArray(block.getCapacityList());

    //get the output variable
    var outVar;
    if (block.mutationData_.storeInVariable === true) {
        outVar = block.getFieldValue("outVar");
    } else {
        outVar = block.generateVariable_("keywordValue");
    }

    var endOfLoopVar = block.generateVariable_("endOfLoop");
    var waitForEventResultVar = block.generateVariable_("waitForEventResult");
    var keywordIdVar = block.generateVariable_("keywordId");
    var capacityVar = block.generateVariable_("capacity");

    var listenForDateTime = "False";
    if ($.inArray("yadoms_wait_for_event_mutator_datetime_change", this.mutationData_.additionalBlocks) !== -1 ||
        $.inArray("yadoms_wait_for_event_mutator_datetime_become", this.mutationData_.additionalBlocks) !== -1) {
        listenForDateTime = "True";
    }


    //generate the waitForAcquisitions call
    var code = waitForEventResultVar + " = yApi.waitForEvent(" + kwList + ", " + capacitiyList + ", " + listenForDateTime + ")\n";

    code += keywordIdVar + " = " + waitForEventResultVar + ".getKeywordId()\n";
    code += outVar + " = " + waitForEventResultVar + ".getValue()\n";
    code += capacityVar + " = " + waitForEventResultVar + ".getCapacity()\n";
    
    //for each keyword 
    var i;
    for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {

        if (!this.mutationData_.additionalBlocks[i] ||
            this.mutationData_.additionalBlocks[i] === "yadoms_wait_for_event_mutator_store_in_variable")
            continue;
        
        //get the keywordId
        var keyId = block.getFieldValue("keywordDd" + i); //construct the argument (= if/elif condition)
        var capacity = block.getFieldValue("capacityDd" + i); //construct the argument (= if/elif condition)

        //create the filrst condition if it is the matching keyword
        var condition = "";

        //append specific conditions
        var operator;
        var argument1;
        switch (this.mutationData_.additionalBlocks[i]) {
            case "yadoms_wait_for_event_mutator_change":
                //nothing to add, watching anychanges
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_KEYWORD and " +keywordIdVar + " == " + keyId;
                break;

            case "yadoms_wait_for_event_mutator_become":
                //add the become if
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                var keywordId = block.getFieldValue("keywordDd" + i);
				var argument0 = Blockly.Yadoms.Python.castToPython(keywordId, outVar);
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_KEYWORD and " + keywordIdVar + " == " + keyId;
                condition += " and " + argument0 + " " + operator + " " + argument1;
                break;

            case "yadoms_wait_for_event_mutator_capacity_change":
                //nothing to add, watching anychanges
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_CAPACITY and " +capacityVar + " == " + capacity;
                break;

            case "yadoms_wait_for_event_mutator_capacity_become":
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
				//retreive keyword associated to the capacity (the first one), just to know the type of data
				var keywordId = Blockly.Yadoms.data.capacities[capacity].id;
				var argument0 = Blockly.Yadoms.Python.castToPython(capacity, outVar);
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_CAPACITY and " + capacityVar + " == " + capacity;
                condition += " and " + argument0 + " " + operator + " " + argument1;
                break;
				
            case "yadoms_wait_for_event_mutator_datetime_change":
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_DATETIME";
                break;

            case "yadoms_wait_for_event_mutator_datetime_become":
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == WAITFOREVENT_DATETIME";
                condition += " and scriptUtilities.strToDateTime(" + outVar + ") " + operator + " scriptUtilities.toDatetime(" + argument1 + ")";
                break;

            default:
                condition = "False"; //disable if for invalid mutation block (should never occurs)
                break;
        }
        

        //get the statement to execute in this case 
        var statement = Blockly.Python.statementToCode(block, "additionalInput_part2_" + i) || Blockly.Python.PASS;

        //append code
        code += "if " + condition + ":\n" + statement + Blockly.Python.INDENT + endOfLoopVar + " = True\n";
        //code += Blockly.Python.prefixLines(branch, Blockly.Python.INDENT) || Blockly.Python.PASS;
    }


    var loop = "# Wait for event block -------> START\n";

    loop += endOfLoopVar + " = False\n";

    code = Blockly.Python.prefixLines(code, Blockly.Python.INDENT) || Blockly.Python.PASS;
    loop += "while " + endOfLoopVar + " != True :\n" + code + "\n";

    loop += "# Wait for event block -------> END\n";

    return loop;
};

