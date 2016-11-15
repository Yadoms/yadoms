/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_sleep"] = function (block) {
	Blockly.Yadoms.Python.AddSleepTimeFunctions();
	
    var valueTime = Blockly.Python.valueToCode(block, "Time", Blockly.Python.ORDER_ATOMIC);
    var dropdownName = block.getFieldValue("timeUnit");

    var valueInSeconds = valueTime;
    switch (dropdownName) {
        case "D":
            valueInSeconds = valueTime * 60 * 60 * 24;
            break;
        case "H":
            valueInSeconds = valueTime * 60 * 60;
            break;
        case "M":
            valueInSeconds = valueTime * 60;
            break;
        case "S":
            valueInSeconds = valueTime;
            break;
    }

    return "sleep(" + valueInSeconds + ")\n";
};
