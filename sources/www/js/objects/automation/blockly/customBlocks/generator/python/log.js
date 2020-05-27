/**
 * Define the python generation function for yadoms_log block
 * @param block The block
  */
Blockly.Python["yadoms_log"] = function (block) {
    var valueName = Blockly.Python.valueToCode(block, "LogContent", Blockly.Python.ORDER_ATOMIC) || "''";
    return "print (" + valueName + ")\n";
};

