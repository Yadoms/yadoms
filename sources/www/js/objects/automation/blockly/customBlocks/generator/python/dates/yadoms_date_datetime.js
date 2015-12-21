
/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_date_datetime"] = function (block) {
    //see https://docs.python.org/2/library/datetime.html#datetime-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var date = Blockly.Python.valueToCode(block, block.dateInputName_, Blockly.Python.ORDER_ATOMIC);
    var time = Blockly.Python.valueToCode(block, block.timeInputName_, Blockly.Python.ORDER_ATOMIC);

    var code = "datetime.combine(" + date + ", " + time + ")";

    return [code, Blockly.Python.ORDER_ATOMIC];
};


