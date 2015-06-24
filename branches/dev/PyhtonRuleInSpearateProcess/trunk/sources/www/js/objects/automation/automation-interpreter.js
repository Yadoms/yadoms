/** @module Device class */

/**
 * Creates an instance of AutomationInterpreter
 * @constructor
 */
function AutomationInterpreter(name) {
   assert(!isNullOrUndefined(name), "name of a AutomationInterpreter must be defined");

   this.name = name;
}