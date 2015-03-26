/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRule(id, name, description, interpreter, editor, model, content, configuration, enabled, state, errorMessage, startDate, stopDate) {
   assert(!isNullOrUndefined(id), "id of a AutomationRule must be defined");
   assert(!isNullOrUndefined(name), "name of a AutomationRule must be defined");
   assert(!isNullOrUndefined(description), "description of a AutomationRule must be defined");
   assert(!isNullOrUndefined(interpreter), "interpreter of a AutomationRule must be defined");
   assert(!isNullOrUndefined(editor), "editor of a AutomationRule must be defined");
   assert(!isNullOrUndefined(model), "actions of a AutomationRule must be defined");
   assert(!isNullOrUndefined(content), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(configuration), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(enabled), "enabled of a AutomationRule must be defined");

   this.id = id;
   this.name = name;
   this.description = description;
   this.interpreter = interpreter;
   this.editor = editor;
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
}

AutomationRule.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      description: encodeURIComponent(this.description),
      interpreter: this.interpreter,
      editor: this.editor,
      model: this.model,
      content: this.content,
      configuration: this.configuration,
      enabled: this.enabled
      //we don't take fields that are filled by the server
   };
};
