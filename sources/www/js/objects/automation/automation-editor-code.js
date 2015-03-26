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
   self.editorUuid = createUUID();

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
AutomationEditorCode.prototype.getDOMStructure = function() {
   return ("<div id=\"" + this.uuid + "\" class=\"code-ide\">" +
               "<div class=\"btn-group\" role=\"group\">" +
                  "<button type=\"button\" class=\"btn btn-default btn-insert-keyword btn-sm\">" +
                     "<span data-i18n=\"modals.dashboard.sub-windows.automation-center.editors.code.insertKeywordId\"></span>&nbsp;" +
                     "<span class=\"caret\"></span>" +
                  "</button>" +
               "</div>" +
               "<div id=\"" + this.editorUuid + "\" class=\"code-editor\"></div>" +
            "</div>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorCode.prototype.applyScript = function() {
debugger;
   ace.require("ace/ext/language_tools");
   this.editor = ace.edit(this.editorUuid);
   this.editor.setTheme("ace/theme/chrome");

   this.editor.setOptions({
      enableBasicAutocompletion: true,
      enableSnippets: true,
      displayIndentGuides: true,
      highlightSelectedWord: true
   });

   //we manage the insert keyword Id button
   $insertKeywordId = $("div#getUuid .btn-insert-keyword");
   //TODO : fill the button
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorCode.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   var self = this;
   self.rule = rule;
   self.editor.getSession().setMode("ace/mode/" + self.rule.interpreter);

   //we get the code only if the rule exist server side
   if (rule.id != -1) {
      AutomationRuleManager.getCode(self.rule, function () {
         self.editor.setValue(rule.code);
      }, true);
   }
};

/**
 * Permit to update the current rule with editor content
 */
AutomationEditorCode.prototype.updateRule = function() {
   var self = this;
   self.rule.content = "";
   self.rule.code = self.editor.getValue();
};

/**
 * Permit to the object to run a custom validator engine
 */
AutomationEditorCode.prototype.validate = function() {
   return true;
};

/**
 * Permit to change the interpreter
 * @param newInterpreter
 */
AutomationEditorCode.prototype.setInterpreter = function(newInterpreter) {
   var found = false;
   $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
      if (value == newInterpreter)
         found = true;
   });

   if (found) {
      //we change the current interpreter
      this.rule.interpreter = newInterpreter;
      this.editor.getSession().setMode("ace/mode/" + this.rule.interpreter);
   }
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