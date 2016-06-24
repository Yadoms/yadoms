Blockly.Blocks["yadoms_wait_for_event_result"] = {
    init: function () {

		this.dropDown = new Blockly.FieldDropdown([
													[$.t("blockly.blocks.yadoms_wait_for_event_result.value"), "value"],
													[$.t("blockly.blocks.yadoms_wait_for_event_result.keywordName"), "keywordName"],
													[$.t("blockly.blocks.yadoms_wait_for_event_result.deviceName"), "deviceName"],
													[$.t("blockly.blocks.yadoms_wait_for_event_result.fullName"), "fullName"]
												  ]);
													
        this.appendDummyInput()
			.appendField(this.dropDown, "dropdown")
			.appendField($.t("blockly.blocks.yadoms_wait_for_event_result.title"));
        this.setPreviousStatement(false, null);
        this.setNextStatement(false, null);
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event_result.tooltip"));
        this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
        this.setInputsInline(true);
		this.setOutput(true);
    },
	
	getChoice : function() {
		return this.dropDown.getValue();
	}

};
