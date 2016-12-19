/**
 * Define the python generation function for keyword-value-get block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["keyword-value-get"] = function (block) {
    Blockly.Yadoms.Python.AddUtilitiesFunctions();
    var keywordId = block.getSelectedKeyword();
    var keywordDefaultValue = Blockly.Yadoms.Python.getKeywordDefaultValue(keywordId)
    var command = "scriptUtilities.readKeywordValue(yApi, " + keywordId + ", "+ keywordDefaultValue + ")";
    var code = Blockly.Yadoms.Python.castToPython(keywordId, command);
    return [code, Blockly.Python.ORDER_ATOMIC];
};
