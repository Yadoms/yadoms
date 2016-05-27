


/**
 * Define a logical negation block
 * @type {{init: Function}}
 */
Blockly.Blocks['yadoms_logic_negate'] = {
  /**
   * Block for negation.
   * @this Blockly.Block
   */
  init: function() {
    this.appendValueInput("BOOL")
        .setCheck("Boolean")
        .appendField($.t("blockly.blocks.logic_negate.title", {defaultValue : Blockly.Msg.LOGIC_NEGATE_TITLE}));
    this.setOutput(true, "Boolean");
    this.setTooltip(Blockly.Msg.LOGIC_NEGATE_TOOLTIP);
    this.setHelpUrl(Blockly.Msg.LOGIC_NEGATE_HELPURL);
	this.setColour(Blockly.Blocks.logic.HUE);
  }
};











