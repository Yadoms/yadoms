/**
 * Define the python generation function for infinite-loop block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['infinite-loop'] = function(block) {
  // Do while/until loop.
  var branch = Blockly.Python.statementToCode(block, 'DO');
  branch = Blockly.Python.addLoopTrap(branch, block.id) || Blockly.Python.PASS;
  return 'while True:\n' + branch;
};
