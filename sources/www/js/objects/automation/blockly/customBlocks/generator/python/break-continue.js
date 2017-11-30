/**
 * Define an override of official Blockly controls_flow_statements (beak/continue)
 * @param block The block
 * @return {*[]}
 */
Blockly.Python['controls_flow_statements'] = function(block) {
   var code = '';
   /*
   AIM:
      if the first 'loop' parent block is wait4event (just set a variable before breaking), else use the classic behavior
   */
   var loopFound = false;
   
   var testingblock = block;
   while(!loopFound && testingblock) {
      var parentBlock = testingblock.getSurroundParent();
      if(parentBlock) {
         if (Blockly.Constants.Loops.CONTROL_FLOW_IN_LOOP_CHECK_MIXIN.LOOP_TYPES.indexOf(parentBlock.type) != -1) {
            loopFound = true;
         } else if(parentBlock.type === 'yadoms_wait_for_event') {
           switch (block.getFieldValue('FLOW')) {
             case 'BREAK':
               code += parentBlock.getBreakVariableName() + ' = True\n';
               break;
             case 'CONTINUE':
               code += parentBlock.getContinueVariableName() + ' = True\n';
               break;
           }         
           //do not set loopFound to True, because in case of multiple imbrication of wait4event, we need to continue breaking all wait4event blocks
         } else {
            //other block, do nothing
         }
         testingblock = parentBlock;
      } else {
         loopFound = true;
      }
   }
  
   
  // Flow statements: continue, break.
  switch (block.getFieldValue('FLOW')) {
    case 'BREAK':
      code += 'break\n';
      break;
    case 'CONTINUE':
      code += 'continue\n';
      break;
    default:
      throw 'Unknown flow statement.';
  }
  return code;
};
