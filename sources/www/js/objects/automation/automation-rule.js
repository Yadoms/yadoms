/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRule(id, name, description, triggers, actions, enabled) {
   assert(id !== undefined, "id of a AutomationRule must be defined");
   assert(name !== undefined, "name of a AutomationRule must be defined");
   assert(description !== undefined, "description of a AutomationRule must be defined");
   assert(triggers !== undefined, "triggers of a AutomationRule must be defined");
   assert(actions !== undefined, "actions of a AutomationRule must be defined");
   assert(enabled !== undefined, "enabled of a AutomationRule must be defined");

   this.id = id;
   this.name = name;
   this.description = description;
   this.triggers = triggers;
   this.actions = actions;
   this.enabled = enabled;
}

Device.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      description: encodeURIComponent(this.description),
      triggers: this.triggers,
      actions: this.actions,
      enabled: this.enabled
   };
};
