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
                  "<div class=\"btn-group\" role=\"group\">" +
                     "<div class=\"dropdown btn-insert-keyword\">" +
                        "<a role=\"button\" data-toggle=\"dropdown\" class=\"btn btn-default\" data-target=\"#\">" +
                           "<span data-i18n=\"modals.dashboard.sub-windows.automation-center.editors.code.insertKeywordId\"></span>&nbsp;" +
                           "<span class=\"caret\"></span>" +
                        "</a>" +
                        "<ul class=\"plugin-list dropdown-menu\" role=\"menu\" aria-labelledby=\"dropdownMenu\">" +
                        "</ul>" +
                     "</div>" +
                  "</div>" +
                  "<div class=\"btn-group\" role=\"group\">" +
                     "<div class=\"dropdown btn-insert-recipient\">" +
                        "<a role=\"button\" data-toggle=\"dropdown\" class=\"btn btn-default\" data-target=\"#\">" +
                           "<span data-i18n=\"modals.dashboard.sub-windows.automation-center.editors.code.insertRecipientId\"></span>&nbsp;" +
                           "<span class=\"caret\"></span>" +
                        "</a>" +
                        "<ul class=\"recipient-list dropdown-menu\" role=\"menu\" aria-labelledby=\"dropdownMenu\">" +
                        "</ul>" +
                     "</div>" +
                  "</div>" +
               "</div>" +
               "<div id=\"" + this.editorUuid + "\" class=\"code-editor\"></div>" +
            "</div>");
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
      highlightSelectedWord: true
   });

   //we manage the insert keyword Id button
   var $insertKeywordId = $("div#" + this.getUuid() + " div.btn-insert-keyword");

   //we get all plugins
   PluginInstanceManager.getAll(function (list) {
      $.each(list, function (key, plugin) {
         var $pluginList = $insertKeywordId.find(".plugin-list");
         $pluginList.append("<li class=\"dropdown-submenu\" id=\"plugin-" + plugin.id + "\">" +
                              "<a tabindex=\"-1\">" + plugin.name + "</a>" +
                              "<ul class=\"dropdown-menu\">" +
                              "</ul>" +
                           "</li>");
      });
   }, true);

   DeviceManager.getAll(function (list) {
      $.each(list, function (deviceKey, device) {
         var $deviceList = $insertKeywordId.find("#plugin-" + device.pluginId + " .dropdown-menu");
         $deviceList.append("<li class=\"dropdown-submenu\" id=\"device-" + device.id + "\">" +
                              "<a tabindex=\"-1\">" + device.friendlyName + "</a>" +
                              "<ul class=\"dropdown-menu\">" +
                              "</ul>" +
                           "</li>");
      });
   }, true);

   KeywordManager.getAll(function (list) {
      $.each(list, function (keywordKey, keyword) {
         var $keywordList = $insertKeywordId.find("#device-" + keyword.deviceId + " .dropdown-menu");
         $keywordList.append("<li id=\"keyword-" + keyword.id + "\">" +
                              "<a tabindex=\"-1\" class=\"keyword\" keyword-id=\"" + keyword.id + "\">" +
                                 keyword.friendlyName +
                                 "<span class=\"pull-right\">(" + keyword.id + ")</span>" +
                              "</a>" +
                           "</li>");
      });

      //we manage click on items
      $insertKeywordId.find(".keyword").bind("click", function (src) {
         self.editor.insert($(src.currentTarget).attr("keyword-id"));
      });

   }, true);

   //we manage recipient list button
   $insertRecipientId = $("div#" + this.getUuid() + " div.btn-insert-recipient");
   $insertRecipientList = $insertRecipientId.find("ul.recipient-list");
   RecipientManager.getAll(function(list) {
      $.each(list, function (key, recipient) {
         $insertRecipientList.append("<li>" +
                                       "<a tabindex=\"-1\" class=\"recipient\" recipient-id=\"" + recipient.id + "\">" +
                                          recipient.firstName + "&nbsp;" + recipient.lastName +
                                          "<span class=\"pull-right\">(" + recipient.id + ")</span>" +
                                       "</a>" +
                                    "</li>");
      });

      //we manage click on items
      $insertRecipientList.find(".recipient").bind("click", function (src) {
         self.editor.insert($(src.currentTarget).attr("recipient-id"));
      });
   }, true);
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorCode.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   var self = this;
   self.rule = rule;
   self.editor.getSession().setMode("ace/mode/" + self.rule.interpreter.toLowerCase());

   //we get the code only if the rule exist server side
   if (rule.id != -1) {
      AutomationRuleManager.getCode(self.rule, function () {
         self.editor.setValue(rule.code);
         self.editor.gotoLine(0, 0, false);
      }, true);
   }
};

/**
 * Permit to fire an event when the modal is shown
 * @param rule
 */
AutomationEditorCode.prototype.onModalShown = function($modal) {
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