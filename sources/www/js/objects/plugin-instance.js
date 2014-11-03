/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, name, type, configuration, autoStart, category) {
   assert(!isNullOrUndefined(name), "name of a pluginInstance must be defined");
   assert(!isNullOrUndefined(type), "type of a pluginInstance must be defined");
   assert(!isNullOrUndefined(configuration), "configuration of a pluginInstance must be defined");
   assert(!isNullOrUndefined(autoStart), "autoStart of a pluginInstance must be defined");
   assert(!isNullOrUndefined(category), "category of a pluginInstance must be defined");

   this.id = id;
   this.name = name;
   this.type = type;
   this.configuration = configuration;
   this.autoStart = autoStart;
   this.lastRunningStatus = null;
   this.category = category;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, name: *, type: *, configuration: *, autoStart: *}}
 */
PluginInstance.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      type: this.type,
      configuration: this.configuration,
      autoStart: this.autoStart,
      category: this.category
   };
};
