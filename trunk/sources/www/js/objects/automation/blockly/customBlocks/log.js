

/**
 * Define the yadoms_log block (allow logging into yaomds.log)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks["yadoms_log"] = {
    init: function () {
        this.setHelpUrl("http://www.example.com/");
        this.setColour(160);
        this.appendValueInput("LogContent")
            .appendField($.t("blockly.blocks.yadoms_log.title"));
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip($.t("blockly.blocks.yadoms_log.tooltip", { defaultValue: "" }));
    }
};

/**
 * Define the python generation function for yadoms_log block
 * @param block The block
  */
Blockly.Python["yadoms_log"] = function (block) {
    var valueName = Blockly.Python.valueToCode(block, "LogContent", Blockly.Python.ORDER_ATOMIC) || "''";
    return "print " + valueName + "\n";
};

