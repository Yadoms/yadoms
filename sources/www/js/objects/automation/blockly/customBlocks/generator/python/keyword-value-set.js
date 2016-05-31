/**
 * Define the python generation function for keyword-value-set block
 * @param block The block
 * @return {string}
 */
Blockly.Python["keyword-value-set"] = function (block) {
    var dropdownKeyword = block.getSelectedKeyword();
	
	var oldKeywordId = Blockly.Python.lastWaitForEventResultKeywordId;
	
	Blockly.Python.lastWaitForEventResultKeywordId = dropdownKeyword;
    var value = Blockly.Python.valueToCode(block, block.inputValueName, Blockly.Python.ORDER_RELATIONAL) || "0";
	
	Blockly.Python.lastWaitForEventResultKeywordId = oldKeywordId;
	
	var castValue = Blockly.Yadoms.Python.castFromPython(dropdownKeyword, value);
	
    return "yApi.writeKeyword(" + dropdownKeyword + ", str(" + castValue + "))\n";
};


