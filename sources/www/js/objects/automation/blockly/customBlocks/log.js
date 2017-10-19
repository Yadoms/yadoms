

/**
 * Define the yadoms_log block (allow logging into yaomds.log)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks["yadoms_log"] = {
    init: function () {
		this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
        this.setColour(Blockly.Blocks.texts.HUE);
        this.appendValueInput("LogContent")
            .appendField($.t("blockly.blocks.yadoms_log.title"));
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip($.t("blockly.blocks.yadoms_log.tooltip", { defaultValue: "" }));
    }
};
