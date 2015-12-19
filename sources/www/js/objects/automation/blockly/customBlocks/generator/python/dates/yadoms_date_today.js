/**
 * Define the python generation function for yadoms_date_today block
 * @param block The block
 */
Blockly.Python["yadoms_date_today"] = function (block) {
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var code = "date.today()";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
