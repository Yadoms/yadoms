/**
 * The AutomationEditorBlockly implements IAutomationRuleEditor
 * This implementation permit to have a standard code GUI
 * @type {IAutomationRuleEditor}
 */
AutomationEditorBlockly.prototype = new IAutomationRuleEditor();
AutomationEditorBlockly.prototype.constructor = AutomationEditorCode;

AutomationEditorBlockly.getSupportedInterpreters = function() {
   return ["python", "dart", "javascript"];
};

/**
 * Ctor
 * @constructor
 */
function AutomationEditorBlockly(interpreters) {
   var self = this;
   self.uuid = createUUID();

   //we compare interpreters and getSupportedInterpreters() static method to keep only active supported interpreters
   self.activeSupportedInterpreters = [];
   $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
      $.each(interpreters, function (inputKey, inputValue) {
         if (value.toLowerCase() ==  inputValue.name.toLowerCase())
            self.activeSupportedInterpreters.push(inputValue);
      });
   });
}

/**
 * Obtain active supported interpreters of the editor
 */
AutomationEditorBlockly.prototype.getActiveSupportedInterpreters = function() {
   return this.activeSupportedInterpreters;
};

/**
 * Obtain Unique identifier of the editor
 */
AutomationEditorBlockly.prototype.getUuid = function() {
   return this.uuid;
};

/**
 * Obtain DOM structure to insert in editor's page
 */
AutomationEditorBlockly.prototype.getDOMStructure = function() {
   return ("<div id=\"" + this.uuid + "\"></div>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorBlockly.prototype.applyScript = function() {
   //TODO
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorBlockly.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   this.rule = rule;
   //TODO
};

/**
 * Permit to update the current rule with editor content
 */
AutomationEditorBlockly.prototype.updateRule = function() {
   //TODO
   rule.content = "";
   rule.code = "";
};

/**
 * Permit to the object to run a custom validator engine
 */
AutomationEditorBlockly.prototype.validate = function() {
   //TODO
   return true;
};

/**
 * Permit to change the interpreter
 * @param newInterpreter
 */
AutomationEditorBlockly.prototype.setInterpreter = function(newInterpreter) {
   var found = false;
   $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
      if (value == newInterpreter)
         found = true;
   });

   if (found) {
      this.activeSupportedInterpreters = newInterpreter;
   }
};

/**
 * Obtain class icon for the editor. ie "fa fa-code"
 * @returns {string}
 */
AutomationEditorBlockly.prototype.getClassIcon = function() {
   return "fa fa-puzzle-piece";
};

/**
 * Obtain name of editor
 * @returns {string}
 */
AutomationEditorBlockly.prototype.getName = function() {
   return "blockly";
};
