/**
 * Define the python generation function for yadoms_date_sunrise block
 * @param block The block
  */
Blockly.Python["yadoms_date_sunrise"] = function (block) {
    //see https://docs.python.org/2/library/datetime.html#datetime-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var code = "scriptUtilities.strToTime(yApi.getInfo(yApi.kSunrise))";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
