/** @module AutomationRule class */

/**
 * Creates an instance of AutomationRule
 * @constructor
 */
function AutomationRule(id, name, description, interpreter, editorName, model, content, configuration, enabled, state, errorMessage, startDate, stopDate) {
   assert(!isNullOrUndefined(id), "id of a AutomationRule must be defined");
   assert(!isNullOrUndefined(name), "name of a AutomationRule must be defined");
   assert(!isNullOrUndefined(description), "description of a AutomationRule must be defined");
   assert(!isNullOrUndefined(interpreter), "interpreter of a AutomationRule must be defined");
   assert(!isNullOrUndefined(editorName), "editor of a AutomationRule must be defined");
   assert(!isNullOrUndefined(model), "actions of a AutomationRule must be defined");
   assert(!isNullOrUndefined(content), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(configuration), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(enabled), "enabled of a AutomationRule must be defined");

   this.id = id;
   this.name = name;
   this.description = description;
   this.interpreter = interpreter;
   this.editorName = editorName;
   this.editor = null;
   this.model = model;
   this.content = content;
   this.configuration = configuration;
   this.enabled = enabled;
   this.state = state;
   this.errorMessage = errorMessage;
   this.startDate = startDate;
   this.stopDate = stopDate;
   this.code = "";
   this.codeHasBeenDownloaded = false;
   this.editorHasBeenDownloaded = false;
}

AutomationRule.prototype.toJSON = function () {
   return {
      id : this.id,
      name: this.name,
      description: this.description,
      interpreter: this.interpreter,
      editor: this.editorName,   //we provide only the name of the editor and not the full object
      model: this.model,
      content: this.content,
      configuration: this.configuration,
      enabled: this.enabled
      //we don't take fields that are filled by the server
   };
};

/**
 * Download the editor attached to the rule
 */
AutomationRule.prototype.downloadEditor = function (callback) {
   assert($.isFunction(callback), "callback must be a function");
   var self = this;
   if (!self.editorHasBeenDownloaded) {
      AutomationEditorManager.getByName(self.editorName, function (editor) {
         //if there is an error getting editor
         if (!editor) {
            notifyError($.t("objects.generic.errorEditing", {objectName: "automation rule"}));
            return;
         }
         self.editor = editor;
         self.editorHasBeenDownloaded = true;
         callback();
      });
   }
   else {
      callback();
   }
};
