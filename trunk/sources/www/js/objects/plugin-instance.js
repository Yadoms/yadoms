/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, address, protocol, hardwareIdentifier, name) {
   assert(id !== undefined, "id of a pluginInstance must be defined");
   assert(address !== undefined, "address of a pluginInstance must be defined");
   assert(protocol !== undefined, "protocol of a pluginInstance must be defined");
   assert(hardwareIdentifier !== undefined, "hardwareIdentifier of a pluginInstance must be defined");
   assert(name !== undefined, "name of a pluginInstance must be defined");

   this.id = id;
   this.address = address;
   this.protocol = protocol;
   this.hardwareIdentifier = hardwareIdentifier;
   this.name = name;
}
