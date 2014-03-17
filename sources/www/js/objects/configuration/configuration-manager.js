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
      var handler = ConfigurationHelper.createParameterHandler(self.objectToConfigure, key, value, currentValue);
      self.configurationHandlers.push(handler);
   });

   $domContainer.append(this.getDOMObject());
}

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