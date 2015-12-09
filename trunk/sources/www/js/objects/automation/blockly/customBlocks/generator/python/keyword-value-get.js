/**
 * Define the python generation function for keyword-value-get block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["keyword-value-get"] = function (block) {
    var keywordId = block.getSelectedKeyword();
	var command = "yApi.readKeyword(" + keywordId + ")";
    var code = Blockly.Yadoms.Python.cast(keywordId, command);
    return [code, Blockly.Python.ORDER_ATOMIC];
};
