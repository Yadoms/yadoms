


/**
 * Define the yadoms_sleep block (allow current rule to sleep)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks["yadoms_sleep"] = {
    init: function () {

        var durationUnitsEnum = [
            [$.t("blockly.global.durationUnits.seconds"), "S"],
            [$.t("blockly.global.durationUnits.minutes"), "M"],
            [$.t("blockly.global.durationUnits.hours"), "H"],
            [$.t("blockly.global.durationUnits.days"), "D"]
        ];

        this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.appendValueInput("Time")
            .setCheck("Number")
            .appendField($.t("blockly.blocks.yadoms_sleep.title"));
        this.appendDummyInput()
            .appendField(new Blockly.FieldDropdown(durationUnitsEnum), "timeUnit");
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip($.t("blockly.blocks.yadoms_sleep.tooltip", { defaultValue: "" }));

    }
};



/**
 * Define the python generation function for yadoms_sleep block
 * @param block The block
 */
Blockly.Python["yadoms_sleep"] = function (block) {
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

    return "time.sleep(" + valueInSeconds + ")\n";
};
