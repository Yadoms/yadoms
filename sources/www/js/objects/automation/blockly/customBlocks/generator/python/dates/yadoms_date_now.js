/**
 * Define the python generation function for yadoms_date_now block
 * @param block The block
 */
Blockly.Python["yadoms_date_now"] = function (block) {
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var code = "datetime.now().replace(second=0, microsecond=0).time()";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
