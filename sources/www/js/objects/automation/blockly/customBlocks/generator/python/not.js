
/**
 * Define the python generation function for yadoms_logic_negate block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['yadoms_logic_negate'] = function(block) {
  // Negation.
  var argument0 = Blockly.Python.valueToCode(block, 'BOOL',
      Blockly.Python.ORDER_LOGICAL_NOT) || 'True';
  var code = 'not ' + argument0;
  return [code, Blockly.Python.ORDER_LOGICAL_NOT];
};