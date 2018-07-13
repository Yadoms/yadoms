/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function Device(id, pluginId, name, friendlyName, model, type, configuration, blacklist, details) {
   assert(id !== undefined, "id of a Device must be defined");
   assert(pluginId !== undefined, "pluginId of a Device must be defined");
   assert(name !== undefined, "name of a Device must be defined");
   assert(friendlyName !== undefined, "friendlyName of a Device must be defined");
      assert(model !== undefined, "model of a Device must be defined");

   this.id = id;
   this.pluginId = pluginId;
   this.name = name;
   this.friendlyName = friendlyName;
   this.model = model;
   this.attachedPlugin = undefined;
   this.keywords = undefined;
   this.type = type;
   this.configuration = configuration;
   this.blacklist = blacklist;
   this.details = details;
}

Device.prototype.toJSON = function () {
   return {
      id : this.id,
      pluginId: this.pluginId,
      name: this.name,
      friendlyName: this.friendlyName,
      model: this.model,
      configuration: this.configuration,
      type: this.type,
	   blacklist : this.blacklist
   };
};
