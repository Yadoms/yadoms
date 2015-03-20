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
AutomationEditorBlockly.prototype.getDOMObject = function() {
   return ("<div id=\"" + this.uuid + "\"></div>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorBlockly.prototype.applyScript = function() {
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
AutomationEditorBlockly.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   this.rule = rule;
   this.editor.getSession().setMode("ace/mode/" + rule.type);
   this.editor.setValue(decodeURIComponent(rule.code));
   this.editor.gotoLine(editor.session.getLength());
};

/**
 * Permit to the object to run a custom validator engine
 */
AutomationEditorBlockly.prototype.validate = function() {
   return true;
};

/**
 * Get the current rule after edition
 */
AutomationEditorBlockly.prototype.getRule = function() {
   this.rule.code = this.editor.getValue();
   return this.rule;
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