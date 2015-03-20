/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationLanguage(name, newFileTemplate) {
   assert(!isNullOrUndefined(name), "name of a AutomationLanguage must be defined");
   assert(!isNullOrUndefined(newFileTemplate), "newFileTemplate of a AutomationLanguage must be defined");

   this.name = name;
   this.newFileTemplate = newFileTemplate;
}