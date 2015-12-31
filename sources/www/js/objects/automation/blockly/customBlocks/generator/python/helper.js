Blockly.Yadoms.Python = function () {
};

/**
 * Get the code for casting a keyword data in the python compatible type
 * @param {Number} keywordId The target keyword id
 * @param {String} command The command (python code) which provide a keyword data (ie: yApi.readKeyword...)
 * @return {String} The resulting python code
 */
Blockly.Yadoms.Python.castToPython = function(keywordId, command) {
	var code = command;
	if(keywordId && command) {
		//retreive the keyword information, to apply cast if needed
		var keyword = Blockly.Yadoms.data.keywords[keywordId];
		if(keyword && keyword.type) {
			switch (keyword.type.toLowerCase()) {
				case "numeric":
					code = "float(" + command + ")";
					break;
				case "bool":
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
 * Get the code for casting a python value to a keyword type
 * @param {Number} keywordId The target keyword id
 * @param {String} command The command (python code) which provide python data to convert to yadoms keyword type
 * @return {String} The resulting python code
 */
Blockly.Yadoms.Python.castFromPython = function(keywordId, command) {
	var code = command;
	if(keywordId && command) {
		//retreive the keyword information, to apply cast if needed
		var keyword = Blockly.Yadoms.data.keywords[keywordId];
		if(keyword && keyword.type) {
			switch (keyword.type.toLowerCase()) {
				case "bool":
				case "boolean":
					code = "str(int(" + command + "))";
					break;
				default:
					code = "\"" + command + "\"";
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
* Convert an list into string array
* @param {Array of Number} list A list of values (ie: [1,5,6])
* @returns {String} The list of values (ie: "[1,5,6]" )
*/
Blockly.Yadoms.Python.listToStringArray = function (list) {
    //list all keyword id in a list
    var resultList = "[";

    for (var i = 0; i < list.length; i++) {
        resultList += list[i] + " ,";
    }

    if (resultList[resultList.length - 1] === ",") {
        resultList = resultList.substring(0, resultList.length - 2);
    }
    resultList += "]";
    return resultList;
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


Blockly.Yadoms.Python.addDateTimeFunctions_ = false;

Blockly.Yadoms.Python.clearAdditionalImports = function() {
    Blockly.Yadoms.Python.addDateTimeFunctions_ = false;
}

Blockly.Yadoms.Python.AddDateTimeFunctions = function() {
    Blockly.Yadoms.Python.addDateTimeFunctions_ = true;
}

Blockly.Yadoms.Python.HasDateTimeFunctions = function() {
    return Blockly.Yadoms.Python.addDateTimeFunctions_;
}


Blockly.Yadoms.Python.GenerateDateTimePythonCode_ = function() {
    var additionalCode = "# datetime helpers\n";
    additionalCode += "import scriptUtilities\n";
    additionalCode += "from datetime import datetime, date, time, timedelta\n";
    additionalCode += "\n";
    return additionalCode;
}
