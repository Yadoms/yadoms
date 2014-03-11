/**
 * Created by Nicolas on 28/02/14.
 */

/**
 * Creates an instance of LazyLoaderManager
 * @constructor
 * @param modalPath
 */
function ConfigurationManager(objectToConfigure, $domContainer) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert($domContainer !== undefined, "$domContainer must be defined");

   this.objectToConfigure = objectToConfigure;
   this.configurationSchema = objectToConfigure.package.configurationSchema;
   this.configurationValues = objectToConfigure.configuration;
   this.$domContainer = $domContainer;
   this.configurationHandlers = new Array();

   var self = this;
   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var currentValue = self.configurationValues[key];
      var currentValue = (currentValue !== undefined) ? currentValue : "";
      var handler = self.createParameterHandler(self.objectToConfigure, key, value, currentValue);
      self.configurationHandlers.push(handler);
   });

   $domContainer.append(this.getDOMObject());
}

/**
 * Factory of parameter Handler
 * can be recursive on special types
 * @param name
 * @param content
 */
ConfigurationManager.prototype.createParameterHandler = function (objectToConfigure, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(content.type !== undefined, "type field must be found in " + paramName + " parameter");

   //we build the i18n context
   var i18nContext = objectToConfigure.name + ":configurationSchema.";

   switch (content.type.toLowerCase()) {
      case "int" :
            return new IntParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "decimal" :
         return new DecimalParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "enum" :
         return new EnumParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + paramName + " is unsupported");
         break;
   }
};

ConfigurationManager.prototype.getDOMObject = function() {
   var result = "";
   $.each(this.configurationHandlers, function (key, value) {
      result += value.getDOMObject();
      result += "\n";
   });

   result += "";
   return result;
}

ConfigurationManager.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   this.configurationValues = {};
   var self = this;
   $.each(this.configurationHandlers, function (key, value) {
      self.configurationValues[value.getParamName()] = value.getCurrentConfiguration();
   });
   return this.configurationValues;
}