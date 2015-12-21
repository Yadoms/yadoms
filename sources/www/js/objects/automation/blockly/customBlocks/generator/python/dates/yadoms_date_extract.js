Blockly.Python['yadoms_date_extract'] = function(block) {
    //see https://docs.python.org/2/library/datetime.html#datetime-objects
    Blockly.Yadoms.Python.AddDateTimeFunctions();

	
  var chosenDate = Blockly.Python.valueToCode(block, block.valueInputName_, Blockly.Python.ORDER_ATOMIC) || '0';
  
  var chosenValue = block.getFieldValue(block.dropdownName_);
  
  var code = "";
  
  switch(chosenValue) {
	case "date" : 
		code = chosenDate + ".date()";
		break;

	case "time" : 
		code = chosenDate + ".time()";
		break;

	case "year" : 
		code = chosenDate + ".year";
		break;

	case "month" : 
		code = chosenDate + ".month";
		break;

	case "day" : 
		code = chosenDate + ".day";
		break;

	case "hour" : 
		code = chosenDate + ".hour";
		break;

	case "minute" : 
		code = chosenDate + ".minute";
		break;

	case "weekDay" : 
		code = chosenDate + ".weekday()";
		break;

	default:
		code = chosenDate;
  }
  
  return [code, Blockly.Python.ORDER_ATOMIC];
};
