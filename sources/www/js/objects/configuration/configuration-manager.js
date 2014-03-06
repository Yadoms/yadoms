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

   this.configurationSchema = configurationSchema;
   this.objectToConfigure = objectToConfigure;
   this.configurationValues = configurationValues;

   this.configurationHandlers = new Array();

   var self = this;
   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var idx = (self.configurationValues !== undefined) ? self.configurationValues.indexOf(key) : -1 ;
      var currentValue = (idx != -1) ? self.configurationValues[currentValue] : "";
      var handler = self.createParameterHandler(self.objectToConfigure, key, value, currentValue);
      self.configurationHandlers.push(handler);
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

   //we build the i18n context
   var i18nContext = objectToConfigure.name + ":configurationSchema.";

   switch (content.type.toLowerCase()) {
      case "int" :
            return new IntParameterHandler(i18nContext, name, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + name + " is unsupported");
         break;
   }
};

ConfigurationManager.prototype.getDOMObject = function() {
   var result = "<div class=\"control-group\">\n<div class=\"controls\">\n";
   $.each(this.configurationHandlers, function (key, value) {
      result += value.getDOMObject();
      result += "\n";
   });

   result += "</div>\n</div>\n";
   return result;
}