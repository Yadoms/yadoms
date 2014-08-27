/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, name, type, configuration, autoStart) {
   assert(id !== undefined, "id of a pluginInstance must be defined");
   assert(name !== undefined, "name of a pluginInstance must be defined");
   assert(type !== undefined, "type of a pluginInstance must be defined");
   assert(configuration !== undefined, "configuration of a pluginInstance must be defined");
   assert(autoStart !== undefined, "autoStart of a pluginInstance must be defined");

   this.id = id;
   this.name = name;
   this.type = type;
   this.configuration = configuration;
   this.autoStart = autoStart;
   this.lastRunningStatus = null;
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
      autoStart: this.autoStart
   };
};
