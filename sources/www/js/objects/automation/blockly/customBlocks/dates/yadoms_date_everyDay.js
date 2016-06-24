/**
 * Define a block which target a precise hour every day
 * @param block The block
 * @return {*[]}
 */
 Blockly.Blocks['yadoms_date_everyDay'] = {
  init: function() {
    this.appendDummyInput()
        .appendField($.t("blockly.blocks.dates.yadoms_date_everyDay.title", { defaultValue: "Every day" }));
    this.setInputsInline(true);
    this.setOutput(true, "date");
    this.setColour(Blockly.Yadoms.Dates.date.HUE);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_everyDay.tooltip", { defaultValue: "" }));
    this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
  }
};