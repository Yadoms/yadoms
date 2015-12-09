
/**
 * Get the blockly Python output
 * @return The python output
 * @private 
 */
Blockly.Yadoms.GetPythonOutput_ = function() {
    //get python code
    var pythonCode = Blockly.Python.workspaceToCode();
    var pythonCodeIndented = Blockly.Python.prefixLines(pythonCode, "\t");
    var completedPythonCode = "# yMain is the script entry point, called by Yadoms\n";
    completedPythonCode += "import time\n";
    completedPythonCode += "\n";
    completedPythonCode += "def yMain(yApi):\n";
    completedPythonCode += "\tprint ('Script started')\n";
    completedPythonCode += pythonCodeIndented;
    return completedPythonCode;
};