/**
 * The AutomationEditorCode implements IAutomationRuleEditor
 * This implementation permit to have a standard code GUI
 * @type {IAutomationRuleEditor}
 */
AutomationEditorCode.prototype = new IAutomationRuleEditor();
AutomationEditorCode.prototype.constructor = AutomationEditorCode;

AutomationEditorCode.getSupportedInterpreters = function() {
   return ["python"];
};

/**
 * Ctor
 * @param intrepreters : list of available interpreters
 * @constructor
 */
function AutomationEditorCode(interpreters) {
   var self = this;
   self.uuid = createUUID();

   //we compare interpreters and getSupportedInterpreters() static method to keep only active supported interpreters
   this.activeSupportedInterpreters = [];
   $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
      $.each(interpreters, function (inputKey, inputValue) {
         if (value.toLowerCase() ==  inputValue.name.toLowerCase())
            self.activeSupportedInterpreters.push(inputValue);
      });
   });
}

/**
 * Obtain active supported interpreters of the editor
 */
AutomationEditorCode.prototype.getActiveSupportedInterpreters = function() {
   return this.activeSupportedInterpreters;
};

/**
 * Obtain Unique identifier of the editor
 */
AutomationEditorCode.prototype.getUuid = function() {
   return this.uuid;
};

/**
 * Obtain DOM structure to insert in editor's page
 */
AutomationEditorCode.prototype.getDOMObject = function() {
   return ("<div id=\"" + this.uuid + "\"></div>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorCode.prototype.applyScript = function() {
   ace.require("ace/ext/language_tools");
   this.editor = ace.edit("automation-rule-editor");
   this.editor.setTheme("ace/theme/chrome");

   this.editor.setOptions({
      enableBasicAutocompletion: true,
      enableSnippets: true,
      displayIndentGuides: true,
      highlightSelectedWord: true
   });
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorCode.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   this.rule = rule;
   this.editor.getSession().setMode("ace/mode/" + rule.type);
   this.editor.setValue(decodeURIComponent(rule.code));
   this.editor.gotoLine(editor.session.getLength());
};

/**
 * Permit to the object to run a custom validator engine
 */
AutomationEditorCode.prototype.validate = function() {
   return true;
};

/**
 * Get the current rule after edition
 */
AutomationEditorCode.prototype.getRule = function() {
   this.rule.code = this.editor.getValue();
   return this.rule;
};

/**
 * Obtain class icon for the editor. ie "fa fa-code"
 * @returns {string}
 */
AutomationEditorCode.prototype.getClassIcon = function() {
   return "fa fa-code";
};

/**
 * Obtain name of editor
 * @returns {string}
 */
AutomationEditorCode.prototype.getName = function() {
   return "code";
};