/**
 * Define the date block (allow to choose an absolute date)
 * @param block The block
 * @return {*[]}
 */
 Blockly.Blocks['yadoms_date_date'] = {
  /**
   * The name of hour input
   */
  dateFieldName_ : "dateField",

  /**
   *Block initialization function
   */
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldDate(), this.dateFieldName_);
    this.setInputsInline(true);
    this.setOutput(true, "date");
    this.setColour(Blockly.Yadoms.Dates.date.HUE);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_date.tooltip", { defaultValue: "" }));
    this.setHelpUrl(Blockly.Yadoms.GenerateHelpUrl(this.type));
  },

  getDate : function() {
      return this.getFieldValue(this.dateFieldName_);
  }
};
