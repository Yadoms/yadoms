/**
 * Define the python generation function for yadoms_wait_for_keywords block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait_for_keywords"] = function (block) {
    var order = Blockly.Python.ORDER_RELATIONAL;

    //list all keyword id in a list
    var kwList = Blockly.Yadoms.Python.listToStringArray(block.getKeywordList());

    //get the output variable
    debugger;
    var outVar;
    if (block.mutationData_.storeInVariable === true) {
        outVar = block.getFieldValue("outVar");
    } else {
        outVar = block.generateVariable_("keywordValue");
    }

    var endOfLoopVar = block.generateVariable_("endOfLoop");
    var waitForEventResultVar = block.generateVariable_("waitForEventResult");
    var keywordIdVar = block.generateVariable_("keywordId");

    var listenForDateTime = "False";
    if ($.inArray("yadoms_wait_for_keywords_mutator_datetime_change", this.mutationData_.additionalBlocks) !== -1 ||
        $.inArray("yadoms_wait_for_keywords_mutator_datetime_become", this.mutationData_.additionalBlocks) !== -1) {
        listenForDateTime = "True";
    }


    //generate the waitForAcquisitions call
    var code = waitForEventResultVar + " = yApi.waitForEvent(" + kwList + ", " + listenForDateTime + ")\n";

    code += keywordIdVar + " = " + waitForEventResultVar + ".getKeywordId()\n";
    code += outVar + " = " + waitForEventResultVar + ".getValue()\n";
    
    //for each keyword 
    var i;
    for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {

        if (!this.mutationData_.additionalBlocks[i] ||
            this.mutationData_.additionalBlocks[i] === "yadoms_wait_for_keywords_mutator_store_in_variable")
            continue;
        
        //get the keywordId
        var keyId = block.getFieldValue("keywordDd" + i); //construct the argument (= if/elif condition)

        //create the filrst condition if it is the matching keyword
        var condition = "";

        //append specific conditions
        var operator;
        var argument1;
        switch (this.mutationData_.additionalBlocks[i]) {
            case "yadoms_wait_for_keywords_mutator_change":
                //nothing to add, watching anychanges
                condition = waitForEventResultVar + ".getType() == 1 and " +keywordIdVar + " == " + keyId;
                break;

            case "yadoms_wait_for_keywords_mutator_become":
                //add the become if
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                var keywordId = block.getFieldValue("keywordDd" + i);
				var argument0 = Blockly.Yadoms.Python.cast(keywordId, outVar);
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == 1 and " + keywordIdVar + " == " + keyId;
                condition += " and " + argument0 + " " + operator + " " + argument1;
                break;

            case "yadoms_wait_for_keywords_mutator_datetime_change":
                condition = waitForEventResultVar + ".getType() == 2";
                break;

            case "yadoms_wait_for_keywords_mutator_datetime_become":
                operator = Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition = waitForEventResultVar + ".getType() == 2";
                condition += " and scriptUtilities.strToDateTime(" + outVar + ") " + operator + " " + argument1;
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

