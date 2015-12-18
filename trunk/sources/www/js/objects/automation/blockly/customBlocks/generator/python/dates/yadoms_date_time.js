
/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_date_time"] = function (block) {
    //see https://docs.python.org/2/library/datetime.html#time-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var hour = Blockly.Python.valueToCode(block, block.hourInputName_, Blockly.Python.ORDER_ATOMIC);
    var minute = Blockly.Python.valueToCode(block, block.minuteInputName_, Blockly.Python.ORDER_ATOMIC);

    var code = "time(" + hour + ", " + minute + ")";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
