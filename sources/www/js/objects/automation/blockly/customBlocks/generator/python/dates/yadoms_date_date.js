
/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_date_date"] = function (block) {
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var date = block.getDate();

    var dateSplit = date.split("-");

    var code = "";
    if(dateSplit.length >= 3) 
        code = " date(" + dateSplit[0] + ", " + dateSplit[1] + ", " + dateSplit[2] + ")";

    return [code, Blockly.Python.ORDER_ATOMIC];
};
