/**
 * Define the python generation function for yadoms_notification_simple block
 * @param block The block
 * @return {string}
 */
Blockly.Python["yadoms_notification_simple"] = function (block) {
    var dropdownKeyword = block.getSelectedKeyword();
    var recipientId = block.recipientDd.getValue();
    var message = Blockly.Python.valueToCode(block, block.inputValueName, Blockly.Python.ORDER_RELATIONAL) || "0";
    return "yApi.sendNotification(" + dropdownKeyword + "," + recipientId + ", " + message + ")\n";
};

