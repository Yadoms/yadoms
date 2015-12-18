/**
 * Define the operations between date/hour and TimeSpan
 * @param block The block
 * @return {*[]}
 */
 Blockly.Blocks['yadoms_date_arithmetic'] = {
  /**
   * The name of operator field
   */
  operatorFieldName_ : "operator",

  /**
   * The name of A input
   */
  inputNameA_ : "A",

  /**
   * The name of B input
   */
  inputNameB_ : "B",

  init: function() {
    var operators =
        [[Blockly.Msg.MATH_ADDITION_SYMBOL, 'ADD'],
         [Blockly.Msg.MATH_SUBTRACTION_SYMBOL, 'MINUS']];

    this.appendValueInput(this.inputNameA_)
        .setCheck(["time", "datetime", "timespan"]);
    this.appendValueInput(this.inputNameB_)
        .setCheck("timespan")
        .appendField(new Blockly.FieldDropdown(operators), this.operatorFieldName_);
    this.setInputsInline(true);
    this.setOutput(true, ["time", "datetime", "timespan"]);
    this.setColour(160);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_arithmetic.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
  }
};

