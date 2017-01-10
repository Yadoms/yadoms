/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function Plugin(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(!isNullOrUndefined(json.version), "json.version of a pluginInstance must be defined");

   this.package = json;
   //the only information we duplicate is the "type" field for lighter code
   this.type = json.type;
}

Plugin.prototype.getRecipientFields = function() {

   if (!isNullOrUndefined(this.package)) {
      if (!isNullOrUndefined(this.package.recipientFields)) {
         return this.package.recipientFields;
      }
   }
   return null;
};