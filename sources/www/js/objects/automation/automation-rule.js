/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRule(id, name, description, type, model, content, configuration, enabled) {
   assert(!isNullOrUndefined(id), "id of a AutomationRule must be defined");
   assert(!isNullOrUndefined(name), "name of a AutomationRule must be defined");
   assert(!isNullOrUndefined(description), "description of a AutomationRule must be defined");
   assert(!isNullOrUndefined(type), "type of a AutomationRule must be defined");
   assert(!isNullOrUndefined(model), "actions of a AutomationRule must be defined");
   assert(!isNullOrUndefined(content), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(configuration), "content of a AutomationRule must be defined");
   assert(!isNullOrUndefined(enabled), "enabled of a AutomationRule must be defined");

   this.id = id;
   this.name = name;
   this.description = description;
   this.type = type;
   this.model = model;
   this.content = content;
   this.configuration = configuration;
   this.enabled = enabled;
}

AutomationRule.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      description: encodeURIComponent(this.description),
      type: this.type,
      model: this.model,
      content: this.content,
      configuration: this.configuration,
      enabled: this.enabled
   };
};
