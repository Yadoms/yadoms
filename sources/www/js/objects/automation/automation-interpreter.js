/** @module Device class */

/**
 * Creates an instance of AutomationInterpreter
 * @constructor
 */
function AutomationInterpreter(name) {
   assert(!isNullOrUndefined(name), "name of a AutomationInterpreter must be defined");

   this.name = name;
   this.package = null;
   this.type = name.toLowerCase();
}

/**
 * Fill the detailed provided by its package.json file
 */
AutomationInterpreter.prototype.fillDetails = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(json.type == this.type, "json.type incompatible with type previously defined");
   assert(!isNullOrUndefined(json.version), "json.version of a pluginInstance must be defined");

   this.package = json;
}