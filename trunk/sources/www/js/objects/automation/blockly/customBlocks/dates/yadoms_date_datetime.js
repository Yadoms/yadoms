/**
 * Define a block which targets a date and a time
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_date_datetime'] = {
  /**
   * The date input name
   */
  dateInputName_ : "dateInput",
  /**
   * The date input name
   */
  hourInputName_ : "hourInput",

  /**
   *Block initialization function
   */
  init: function() {
    this.appendValueInput(this.dateInputName_)
        .appendField($.t("blockly.blocks.dates.yadoms_date_datetime.date", { defaultValue: "date" }))
        .setCheck("date");
      this.appendDummyInput()
          .appendField($.t("blockly.blocks.dates.yadoms_date_datetime.time", { defaultValue: "time" }));
    this.appendValueInput(this.hourInputName_)
        .setCheck("time");
    this.setInputsInline(true);
    this.setOutput(true, "datetime");
    this.setColour(160);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_datetime.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
  }
};
