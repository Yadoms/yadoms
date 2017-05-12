

Blockly.Blocks["yadoms_notification_simple"] = {
    inputValueName: "Value",
    operatorValueName: "operator",
    unitsInputName: "units",
    deviceDropDownName: "Device",
    keywordDropDownName: "Keyword",
    recipientDropDownName: "Recipient",
    inputValueShown: false,

    init: function () {

		this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
        this.setInputsInline(true);
        this.setPreviousStatement(true, "null");
        this.setNextStatement(true, "null");
        this.setTooltip($.t("blockly.blocks.yadoms_notification_simple.tooltip", { defaultValue: "" }));
        this.setColour(Blockly.Yadoms.blockColour.HUE);
        var thisBlock = this;



        //recipient list is static (do not need to update it on each dropdown click)
        var recipientList = Blockly.Yadoms.LoadRecipients_();
        if (recipientList == null || recipientList.length === 0) {
            this.setDisabled(true);
            recipientList = [[$.t("blockly.invalid"), "invalid"]];
        }

        thisBlock.recipientDd = new Blockly.FieldDropdown(recipientList);

        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_notification_simple.title"))
            .appendField(thisBlock.recipientDd, thisBlock.recipientDropDownName);

        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_notification_simple.by"));



        Blockly.Yadoms.ConfigureBlockForYadomsKeywordSelection(this, true, null, ["message"], function () {
        }, thisBlock.deviceDropDownName, thisBlock.keywordDropDownName);

        this.appendValueInput(thisBlock.inputValueName)
            .setCheck("String")
            .appendField($.t("blockly.blocks.yadoms_notification_simple.message"));

    }
};
