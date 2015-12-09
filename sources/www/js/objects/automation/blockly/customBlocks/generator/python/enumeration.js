
/**
 * Define the python generation function for yadoms_enumeration block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_enumeration"] = function (block) {
    var code = Blockly.Python.quote_(block.getSelectedEnumValue());
    return [code, Blockly.Python.ORDER_ATOMIC];
};

