/**
 * Define the python generation function for keyword-value-set block
 * @param block The block
 * @return {string}
 */
Blockly.Python["keyword-value-set"] = function (block) {
    var dropdownKeyword = block.getSelectedKeyword();
    var value = Blockly.Python.valueToCode(block, block.inputValueName, Blockly.Python.ORDER_RELATIONAL) || "0";
    return "yApi.writeKeyword(" + dropdownKeyword + ", \"" + value + "\")\n";
};


