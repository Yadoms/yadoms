


/**
 * Define a condition block which is true when a keyword match a value (not a front value)
 * @type {{init: Function}}
 */
Blockly.Blocks["yadoms_wait"] = {
    valueInput_: "timeToWait",
    init: function() {
        this.setHelpUrl("http://www.example.com/");
        this.setColour(Blockly.Yadoms.Dates.datetime.HUE);

        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);

        this.appendValueInput(this.valueInput_)
			.setCheck(["timespan"])
            .appendField($.t("blockly.blocks.yadoms_wait.title"));
		this.setTooltip($.t("blockly.blocks.yadoms_wait.tooltip", { defaultValue: "" }));			
    }
};















