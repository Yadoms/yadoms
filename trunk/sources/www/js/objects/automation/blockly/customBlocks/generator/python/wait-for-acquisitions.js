/**
 * Define the python generation function for yadoms_wait_for_keywords block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait_for_keywords"] = function (block) {
    var order = Blockly.Python.ORDER_RELATIONAL;

    var endOfLoopVar = block.generateVariable_("endOfLoop");

    //list all keyword id in a list
    var kwList = block.python_getKeywordList();

    //get the output variable
    var outVar;
    if (block.mutationData_.storeInVariable === true) {
        outVar = block.getFieldValue("outVar");
    } else {
        outVar = "keywordValue";
    }

    //generate the waitForAcquisitions call
    var code = "keywordId, " + outVar + " = yApi.waitForNextAcquisitions(" + kwList + ")\n";

    //for each keyword 
    var i;
    for (i = 0; i < this.mutationData_.additionalBlocks.length; i++) {

        if (!this.mutationData_.additionalBlocks[i] ||
            this.mutationData_.additionalBlocks[i] === "yadoms_wait_for_keywords_mutator_store_in_variable")
            continue;
        
        //get the keywordId
        var keyId = block.getFieldValue("keywordDd" + i); //construct the argument (= if/elif condition)

        //create the filrst condition if it is the matching keyword
        var condition = "keywordId == " + keyId;

        //append specific conditions
        switch (this.mutationData_.additionalBlocks[i]) {
            case "yadoms_wait_for_keywords_mutator_change":
                //nothing to add, watching anychanges
                break;

            case "yadoms_wait_for_keywords_mutator_become":
                //add the become if
                var operator =  Blockly.Yadoms.Python.getOperatorCode(block.getFieldValue("operatorDd" + i));
                var keywordId = block.getFieldValue("keywordDd" + i);
				var argument0 = Blockly.Yadoms.Python.cast(keywordId, outVar);
                var argument1 = Blockly.Python.valueToCode(block, "additionalInput_part1_" + i, order) || "0";
                condition += " and " + argument0 + " " + operator + " " + argument1;
                break;
        }
        

        //get the statement to execute in this case 
        var statement = Blockly.Python.statementToCode(block, "additionalInput_part2_" + i) || Blockly.Python.PASS;

        //append code
        code += "if " + condition + ":\n" + statement + Blockly.Python.INDENT + endOfLoopVar + " = True\n";
        //code += Blockly.Python.prefixLines(branch, Blockly.Python.INDENT) || Blockly.Python.PASS;
    }


    var loop = endOfLoopVar + " = False\n";

    code = Blockly.Python.prefixLines(code, Blockly.Python.INDENT) || Blockly.Python.PASS;
    loop += "while " + endOfLoopVar + " != True :\n" + code + "\n";

    

    return loop;
};

