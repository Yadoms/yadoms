
/**
 * Define the python generation function for yadoms_at block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait"] = function (block) {
	var chosenDate = Blockly.Python.valueToCode(block, block.valueInput_, Blockly.Python.ORDER_ATOMIC) || '0';
    var code = "yApi.wait(scriptUtilities.dateTimeOrTimeDeltaToString(" + chosenDate + "))\n";
    return code;
};

