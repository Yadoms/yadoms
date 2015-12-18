
/**
 * Define the python generation function for yadoms_date_weekDay block
 * @param block The block
 */
Blockly.Python["yadoms_date_weekDay"] = function (block) {
    //see https://docs.python.org/2/library/datetime.html#timedelta-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

    var code = "scriptUtilities.next_weekday(date.today(), " + block.getWeekDay() +")";
    return [code, Blockly.Python.ORDER_ATOMIC];
};
