/**
 * Define the python generation function for yadoms_wait_for_event_result block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_wait_for_event_result"] = function (block) {
	var code = "";
	
	switch(block.getChoice()) {
		case "value":
			code = Blockly.Python.lastWaitForEventResultVar + ".getValue()";
			break;
		case "deviceName":
			code = Blockly.Python.lastWaitForEventResultVar + ".getDeviceName()"; //TODO: implement getDeviceName (or any get device info in yApiScript)
			code = "";
			break;
	}
	
	return [code, Blockly.Python.ORDER_ATOMIC];
};

