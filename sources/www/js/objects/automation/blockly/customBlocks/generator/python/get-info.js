/**
 * Define the python generation function for yadoms_get_info block
 * @param block The block
  */
Blockly.Python["yadoms_get_info"] = function (block) {
    var val = block.getFieldValue(block.dropdownValueName_);
    var code = "yApi.getInfo(yApi." + val + ")";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
