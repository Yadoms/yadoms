Blockly.Yadoms.Python = function () {
};

/**
 * Get the code for reading a keyword from its id
 * @param {Number} keywordId The target keyword id
 * @return {String} The resulting python code
 */
Blockly.Yadoms.Python.cast = function(keywordId, command) {
	var code = command;
	if(keywordId && command) {
		//retreive the keyword information, to apply cast if needed
		var keyword = Blockly.Yadoms.data.keywords[keywordId];
		if(keyword && keyword.type) {
			switch (keyword.type) {
				case "numeric":
					code = "float(" + command + ")";
					break;
				case "boolean":
					code = "bool(" + command + ")";
					break;
				default:
					break;
			}
		}
	}
	return code;
};

/**
 * Association table between blockly operators to python code
 * @private
 */
Blockly.Yadoms.Python.operators_ = {
	'EQ': "==",
	'NEQ': "!=",
	'LT': "<",
	'LTE': "<=",
	'GT': ">",
	'GTE': ">="
};

/**
 * Get the code for an operator
 * @param {String} operator The operator from blockly code
 * @return {String} The resulting python code
 */
Blockly.Yadoms.Python.getOperatorCode = function(operator) {
	if(operator) {
		return Blockly.Yadoms.Python.operators_[operator];
	}
	return null;
};