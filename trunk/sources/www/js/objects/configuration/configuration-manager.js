/**
 * Created by Nicolas on 28/02/14.
 */

/**
 * Creates an instance of LazyLoaderManager
 * @constructor
 * @param modalPath
 */
function ConfigurationManager(objectToConfigure, configurationSchema, configurationValues) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(configurationSchema !== undefined, "configurationSchema must be defined");

   this.configrationSchema = configurationSchema;
   this.objectToConfigure = objectToConfigure;
   this.configurationValues = configurationValues;

   //we build ihm from configuration-schema and configurationValues
   this.generatedForm = "<div></div>";
   this.$generatedForm = $(this.generatedForm);

   this.configurationHandlers = new Array();

   //for each key in package
   $.each(configurationSchema, function (key, value) {
      var idx = configurationValues.indexOf(key);
      var currentValue = (idx != -1) ? configurationValues[currentValue] : "";
      var handler = this.prototype.createParameterHandler(objectToConfigure, key, value, currentValue);
      configurationHandlers.push(handler);
   });
}

/**
 * Factory of parameter Handler
 * can be recursive on special types
 * @param name
 * @param content
 */
ConfigurationManager.prototype.createParameterHandler = function (objectToConfigure, name, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(content.type !== undefined, "type field must be found in " + name + " parameter");
   switch (content.type.toLowerCase()) {
      case "int" :
            return new IntParameterHandler(objectToConfigure, name, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + name + " is unsupported");
         break;
   }
}

ConfigurationManager.prototype.getDOMObject = function() {
   var result = {};
   $.each(configurationHandlers, function (configHandler) {
      result += configHandler.getDOMObject();
      result += "\n\r";
   });
   return result;
}