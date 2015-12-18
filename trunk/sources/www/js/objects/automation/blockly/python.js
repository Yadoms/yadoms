/**
 * Get the blockly Python output
 * @return The python output
 * @private 
 */
Blockly.Yadoms.GetPythonOutput_ = function() {
    //get python code
    Blockly.Yadoms.Python.clearAdditionalImports();

    var pythonCode = Blockly.Python.workspaceToCode();
    var pythonCodeIndented = Blockly.Python.prefixLines(pythonCode, "\t");
    var completedPythonCode = "# yMain is the script entry point, called by Yadoms\n";

    if (Blockly.Yadoms.Python.HasDateTimeFunctions()) {
        completedPythonCode += Blockly.Yadoms.Python.GenerateDateTimePythonCode_();
    } else {
        completedPythonCode += "import time\n";
    }

    completedPythonCode += "\n";
    completedPythonCode += "def yMain(yApi):\n";
    completedPythonCode += "\tprint ('Script started')\n";
    completedPythonCode += pythonCodeIndented;
    return completedPythonCode;
};


