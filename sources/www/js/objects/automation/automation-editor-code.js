/**
 * The AutomationEditorCode implements IAutomationRuleEditor
 * This implementation permit to have a standard code GUI
 * @type {IAutomationRuleEditor}
 */
AutomationEditorCode.prototype = new IAutomationRuleEditor();
AutomationEditorCode.prototype.constructor = AutomationEditorCode;

AutomationEditorCode.getSupportedInterpreters = function() {
   return ["yPython27"];
};

/**
 * Get the ACE language denomination associated to an interpreter
 * @param interpreterName : name of the interpreter
 * @return the ACE language denomination supporter by this interpreter
 */
AutomationEditorCode.getAceLanguageFromInterpreterName = function(interpreterName) {
   switch(interpreterName) {
      case "yPython27": return "python";
   }
};

/**
 * Ctor
 * @param interpreters : list of available interpreters
 * @constructor
 */
function AutomationEditorCode(interpreters) {
   var self = this;
   self.uuid = createUUID();
   self.editorUuid = createUUID();
   self.helpUuid = createUUID();

   //we compare interpreters and getSupportedInterpreters() static method to keep only active supported interpreters
   this.activeSupportedInterpreters = [];
   $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
      if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value])) {
         self.activeSupportedInterpreters.push(interpreters[value]);
      }
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
 * Obtain the list of all required js files to load
 * @returns {Array{string}}
 */
AutomationEditorCode.prototype.getRequiredJsFiles = function() {
   return ["libs/ace/js/src-min/ace.js",
         "libs/ace/js/src-min/ext-language_tools.js"];
};


/**
 * Obtain DOM structure to insert in editor's page
 */
AutomationEditorCode.prototype.getDOMStructure = function() {
   return ("<div id=\"" + this.uuid + "\" class=\"code-ide\">" +
         "<div id=\"" + this.editorUuid + "\" class=\"code-editor\"></div>" +
         "<button type=\"button\" class=\"btn btn-info btn-sm code-help-button\" id=\"" + this.helpUuid + "\" data-i18n=\"modals.edit-automation-rule.show-help\">doc</button>") +
      "</div>";
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorCode.prototype.applyScript = function() {
   ace.require("ace/ext/language_tools");
   var self = this;

   this.editor = ace.edit(this.editorUuid);
   this.editor.setTheme("ace/theme/chrome");

   this.editor.setOptions({
      enableBasicAutocompletion: true,
      enableSnippets: true,
      displayIndentGuides: true,
      highlightSelectedWord: true,
      showPrintMargin : false
   });

   //we tab size and soft tabs (tabs replaces by spaces)
   this.editor.getSession().setTabSize(3);
   this.editor.getSession().setUseSoftTabs(true);

    //Bind help
   $("#" + self.helpUuid).unbind('click').bind('click', function () {
       self.showDoc();
   });
};


/**
 * Show the API documentation
 */
AutomationEditorCode.prototype.showDoc = function () {
    var url = "help.html";
    url += "?lang=" + Yadoms.systemConfiguration[ConfigurationManager.items.system.language].value;
    url += "&helpUrl=" + encodeURIComponent(this.getApiDocUrl());
    window.open(url);
};


/**
 * Get interpreter API doc URL
 * @return API doc URL
 */
AutomationEditorCode.prototype.getApiDocUrl = function () {
    return AutomationInterpreterManager.getInterpreterBaseUrl(this.rule.interpreter) + "/yScriptApiDoc.md";
};


/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorCode.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   var d = new $.Deferred();

   var self = this;
   self.rule = rule;

   self.editor.getSession().setMode("ace/mode/" + AutomationEditorCode.getAceLanguageFromInterpreterName(self.rule.interpreter));

   //we get the code only if the rule exist server side
   if (rule.id === -1) {
      // Rule id unknown, get code template
      AutomationRuleManager.getTemplateCode(self.rule)
      .done(function () {
         self.editor.setValue(rule.code);
         self.editor.gotoLine(0, 0, false);
         d.resolve();
      })
      .fail(d.reject);
   }
   else {
      AutomationRuleManager.getCode(self.rule)
      .done(function () {
         self.editor.setValue(rule.code);
         self.editor.gotoLine(0, 0, false);
         d.resolve();
      })
      .fail(d.reject);
   }

   return d.promise();
};

/**
 * Permit to fire an event when the modal is shown
 * @param rule
 */
AutomationEditorCode.prototype.onModalShown = function() {
   //nothing to do
};

/**
 * Permit to fire an event when the modal is hidden
 * @param rule
 */
AutomationEditorCode.prototype.onModalHidden = function () {
   //nothing to do
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
      this.editor.getSession().setMode("ace/mode/" + AutomationEditorCode.getAceLanguageFromInterpreterName(this.rule.interpreter));
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