Blockly.Blocks["yadoms_wait_for_event_result"] = {
   /**
    * The dropdown name
    */
   dropdownName_ : "dropdown",

    init: function () {

		this.dropDown = new Blockly.FieldDropdown([
													[$.t("blockly.blocks.yadoms_wait_for_event_result.value"), "value"],
													[$.t("blockly.blocks.yadoms_wait_for_event_result.deviceName"), "deviceName"],
													]);
													
        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_wait_for_event_result.title"))
			.appendField(this.dropDown, this.dropdownName_);

        this.setPreviousStatement(false, null);
        this.setNextStatement(false, null);
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event_result.tooltip"));
        this.setHelpUrl("http://www.example.com/");
        this.setInputsInline(true);
		this.setOutput(true);
    },
	
	getChoice : function() {
		return this.dropDown.getValue();
	}

};
