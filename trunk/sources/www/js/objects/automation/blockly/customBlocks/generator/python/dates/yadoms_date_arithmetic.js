Blockly.Python['yadoms_date_arithmetic'] = function(block) {
    //see https://docs.python.org/2/library/datetime.html#datetime-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

	// Basic arithmetic operators, and power.
  var OPERATORS = {
    'ADD': [' + ', Blockly.Python.ORDER_ADDITIVE],
    'MINUS': [' - ', Blockly.Python.ORDER_ADDITIVE]
  };
  var tuple = OPERATORS[block.getFieldValue(block.operatorFieldName_)];
  var operator = tuple[0];
  var order = tuple[1];
  var argument0 = Blockly.Python.valueToCode(block, block.inputNameA_, order) || '0';
  var argument1 = Blockly.Python.valueToCode(block, block.inputNameB_, order) || '0';
  
  //ensure date and time objects are converted to datetime
  argument0 = "scriptUtilities.toDatetime(" + argument0 + ")";
  argument1 = "scriptUtilities.toDatetime(" + argument1 + ")";
  
  var code = argument0 + operator + argument1;
  return [code, order];
};
