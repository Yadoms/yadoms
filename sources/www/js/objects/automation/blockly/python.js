/**
 * Get the blockly Python output
 * @return The python output
 * @private 
 */
Blockly.Yadoms.GetPythonOutput_ = function() {
    //get python code
    Blockly.Yadoms.Python.clearAdditionalImports();

    var pythonCode = Blockly.Python.workspaceToCode();
    var pythonCodeIndented = Blockly.Python.prefixLines(pythonCode, "   ");
    var completedPythonCode = "# yMain is the script entry point, called by Yadoms\n";

	completedPythonCode += Blockly.Yadoms.Python.GenerateDateTimePythonCode_();
    completedPythonCode += "\n";
    completedPythonCode += "def yMain(yApi):\n";
    completedPythonCode += "   print ('Script started')\n";
    completedPythonCode += pythonCodeIndented;
    return completedPythonCode;
};


