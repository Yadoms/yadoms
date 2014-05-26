/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, name, pluginName, configuration, enabled) {
   assert(id !== undefined, "id of a pluginInstance must be defined");
   assert(name !== undefined, "name of a pluginInstance must be defined");
   assert(pluginName !== undefined, "pluginName of a pluginInstance must be defined");
   assert(configuration !== undefined, "configuration of a pluginInstance must be defined");
   assert(enabled !== undefined, "enabled of a pluginInstance must be defined");

   this.id = id;
   this.name = name;
   this.pluginName = pluginName;
   this.configuration = configuration;
   this.enabled = enabled;
   this.lastRunningStatus = null;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, name: *, pluginName: *, configuration: *, enabled: *}}
 */
PluginInstance.prototype.toJSON = function () {
   return {
      id : this.id,
      name: this.name,
      pluginName: this.pluginName,
      configuration: this.configuration,
      enabled: this.enabled
   };
};
