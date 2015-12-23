
Blockly.Blocks['infinite-loop'] = {
  /**
   * Block for an infinite loop.
   * @this Blockly.Block
   */
  init: function() {
    this.setColour(Blockly.Blocks.loops.HUE);
    this.appendStatementInput('DO')
        .appendField($.t("blockly.blocks.infinite-loop.title"));
    this.setPreviousStatement(true);
    this.setNextStatement(true);
        this.setTooltip($.t("blockly.blocks.infinite-loop.tooltip", { defaultValue: "" }));
    this.setHelpUrl("http://www.example.com/");
  }
};
