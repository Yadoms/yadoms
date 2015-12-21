/**
 * Define the hour block (allow to choose an hour)
 * @param block The block
 * @return {*[]}
 */
 Blockly.Blocks['yadoms_date_time'] = {
  /**
   * The name of hour input
   */
  hourInputName_ : "hourInput",
  /**
   * The name of minute input
   */
  minuteInputName_ : "minuteInput",

  /**
   *Block initialization function
   */
  init: function() {
    this.appendValueInput(this.hourInputName_)
        .setCheck("Number");
    this.appendValueInput(this.minuteInputName_)
        .setCheck("Number")
        .appendField($.t("blockly.blocks.dates.yadoms_date_time.hour", { defaultValue: "h" }));
    this.appendDummyInput()
        .appendField($.t("blockly.blocks.dates.yadoms_date_time.minute", { defaultValue: "min" }));
    this.setInputsInline(true);
    this.setOutput(true, "time");
    this.setColour(Blockly.Yadoms.Dates.time.HUE);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_time.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
  }
};
