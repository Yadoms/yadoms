
/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_date_timespan"] = function (block) {
    //see https://docs.python.org/2/library/datetime.html#timedelta-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var hours = Blockly.Python.valueToCode(block, block.hourInputName_, Blockly.Python.ORDER_ATOMIC);
    var minutes = Blockly.Python.valueToCode(block, block.minuteInputName_, Blockly.Python.ORDER_ATOMIC);


    var code = "timedelta(hours=" + hours + ", minutes=" + minutes + ")";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
