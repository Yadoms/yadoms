/**
 * Define the python generation function for yadoms_wait_for_event block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait_for_event"] = function (block) {
    var order = Blockly.Python.ORDER_RELATIONAL;

	//specify that this python generation will use "scriptUtilities.py"
	Blockly.Yadoms.Python.AddUtilitiesFunctions();
	
	var code = "";
	
	var keywords = block.getKeywordList();
	var capacities = block.getCapacityList();

    //list all keyword id in a list
    var kwList = Blockly.Yadoms.Python.listToStringArray(block.getKeywordList());
	var totalKeywordsToListen = block.generateVariable_("totalKeywordsToListen");
	code += totalKeywordsToListen + " = " + kwList + "\n";
	
    //list all capacities in a list
	var capacitiesVar = {};
	$.each(block.getCapacityList(), function(index, capacityName) {
		code += "# Get list of keywords for capacity : " + capacityName + "\n";
		capacitiesVar[index] = block.generateVariable_("capacityKeywords" + "_" + capacityName.replace(/[^A-Z0-9]+/ig, "_") + "_" + index);
		code += capacitiesVar[index] + " = yApi.getKeywordsByCapacity('" + capacityName + "')\n";
		code += "# Append the capacity's keywords to the complete list (excluding duplicates)\n";
		code += totalKeywordsToListen + " = " + totalKeywordsToListen + " + list(set(" + capacitiesVar[index] + ") - set(" + totalKeywordsToListen +"))\n";
	});
	
	
    //get the output variable
    var outVar = block.generateVariable_("outVar");
    var endOfLoopVar = block.generateVariable_("endOfLoop");
    block.setBreakVariableName(block.generateVariable_("wantToBreak"));
    block.setContinueVariableName(block.generateVariable_("wantToContinue"));
    var waitForEventResultVar = block.generateVariable_("waitForEventResult");
    var keywordIdVar = block.generateVariable_("keywordId");

    var listenForDateTime = "False";
	$.each(this.mutationData_.additionalBlocks, function(index, blockInfo) {
		if(blockInfo.type === "yadoms_wait_for_event_mutator_datetime_change" ||
		   blockInfo.type === "yadoms_wait_for_event_mutator_datetime_become")
		   listenForDateTime = "True";
	});


    //generate the waitForAcquisitions call
    code += waitForEventResultVar + " = yApi.waitForEvent(" + totalKeywordsToListen + ", " + listenForDateTime + ")\n";

    code += keywordIdVar + " = " + waitForEventResultVar + ".getKeywordId()\n";
    code += outVar + " = " + waitForEventResultVar + ".getValue()\n";
    
    //for each keyword 
    var i;
    for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {

        //get the keywordId
        var keyId = block.getFieldValue("keywordDd" + i); //construct the argument
        var capacity = block.getFieldValue("capacityDd" + i); //construct the argument

        //create the filrst condition if it is the matching keyword
        var condition = "";

        //append specific conditions
        var operator;
        var argument1;
		var currentBlockType = this.mutationData_.additionalBlocks[i].type || this.mutationData_.additionalBlocks[i];

		//make pyhton this current event result
		var oldVar = Blockly.Python.lastWaitForEventResultVar;
		var oldKeywordId = Blockly.Python.lastWaitForEventResultKeywordId;
		Blockly.Python.lastWaitForEventResultVar = waitForEventResultVar;
		Blockly.Python.lastWaitForEventResultKeywordId = keyId;

		if(!Blockly.Python.lastWaitForEventResultKeywordId && capacity && Blockly.Yadoms.data.capacities[capacity])
			Blockly.Python.lastWaitForEventResultKeywordId = Blockly.Yadoms.data.capacities[capacity].id;		
		
        switch (currentBlockType) {
			case "yadoms_wait_for_event_mutator_change" : 
				//nothing to add, watching anychanges
				condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " +keywordIdVar + " == " + keyId;
				break;

			case "yadoms_wait_for_event_mutator_become" : 
				//add the become if
				operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
				var argument0 = Blockly.Yadoms.Python.castToPython(keyId, outVar);

				argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";

				condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " + keywordIdVar + " == " + keyId;
				condition += " and " + argument0 + " " + operator + " " + argument1;
				break;
				
			case "yadoms_wait_for_event_mutator_keyword" : 
				console.error("Should not enter here");
				if(parseBool(this.mutationData_.additionalBlocks[i].condition)) {
					//add the become if
					operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
					var argument0 = Blockly.Yadoms.Python.castToPython(keyId, outVar);
					argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
					condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " + keywordIdVar + " == " + keyId;
					condition += " and " + argument0 + " " + operator + " " + argument1;
				} else {
					//nothing to add, watching anychanges
					condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " +keywordIdVar + " == " + keyId;
				}
				break;
			
            case "yadoms_wait_for_event_mutator_capacity_change":
				//nothing to add, watching anychanges
				condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " +keywordIdVar + " in " + capacitiesVar[i];
                break;

            case "yadoms_wait_for_event_mutator_capacity_become":
				operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
				//retreive keyword associated to the capacity (the first one), just to know the type of data
				var argument0 = Blockly.Yadoms.Python.castToPython(Blockly.Yadoms.data.capacities[capacity].id, outVar);
				argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
				condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " + keywordIdVar + " in " + capacitiesVar[i];
				condition += " and " + argument0 + " " + operator + " " + argument1;
                break;

				
            case "yadoms_wait_for_event_mutator_capacity":
				console.error("Should not enter here");
				if(parseBool(this.mutationData_.additionalBlocks[i].condition)) {
					operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
					//retreive keyword associated to the capacity (the first one), just to know the type of data
					var argument0 = Blockly.Yadoms.Python.castToPython(Blockly.Yadoms.data.capacities[capacity].id, outVar);
					argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
					condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " + keywordIdVar + " in " + capacitiesVar[i];
					condition += " and " + argument0 + " " + operator + " " + argument1;
				} else {
					//nothing to add, watching anychanges
					condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_KEYWORD and " +keywordIdVar + " in " + capacitiesVar[i];
				}
                break;

            case "yadoms_wait_for_event_mutator_datetime_change":
                condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_DATETIME";
                break;

            case "yadoms_wait_for_event_mutator_datetime_become":
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == scriptUtilities.WAITFOREVENT_DATETIME";
                condition += " and scriptUtilities.strToDateTime(" + outVar + ") " + operator + " scriptUtilities.toDatetime(" + argument1 + ")";
                break;

            default:
                condition = "False"; //disable if for invalid mutation block (should never occurs)
                break;
        }
        
        //get the statement to execute in this case 
        var statement = Blockly.Python.statementToCode(block, "additionalInput_part2_" + i) || Blockly.Python.PASS;

		//restore previous value (allow wait-for-event imbrication)
		Blockly.Python.lastWaitForEventResultVar = oldVar;
		Blockly.Python.lastWaitForEventResultKeywordId = oldKeywordId;

		
        //append code
        code += "if " + condition + ":\n" + statement + Blockly.Python.INDENT + endOfLoopVar + " = True\n";
        //code += Blockly.Python.prefixLines(branch, Blockly.Python.INDENT) || Blockly.Python.PASS;
    }


    var loop = "# Wait for event block -------> START\n";

    loop += endOfLoopVar + " = False\n";
    loop += block.getBreakVariableName() + " = False\n";
    loop += block.getContinueVariableName() + " = False\n";

    code = Blockly.Python.prefixLines(code, Blockly.Python.INDENT) || Blockly.Python.PASS;
    loop += "while " + endOfLoopVar + " != True :\n" + code + "\n";

    loop += "# Manage break/continue inside waitForEvents\n";
    loop += "if " + block.getBreakVariableName() + ":\n";
    loop += Blockly.Python.INDENT + 'break\n';
    loop += "if " + block.getContinueVariableName() + ":\n";
    loop += Blockly.Python.INDENT + 'continue\n';
    loop += "# Wait for event block -------> END\n";

    return loop;
};

