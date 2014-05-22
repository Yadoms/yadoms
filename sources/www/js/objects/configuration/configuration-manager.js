/**
 * Created by Nicolas on 28/02/14.
 */

/**
 * Creates an instance of LazyLoaderManager
 * @param objectToConfigure
 * @param $domContainer
 * @constructor
 */
function ConfigurationManager(objectToConfigure, i18nNamespace, $domContainer) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(i18nNamespace !== undefined, "i18nNamespace must contain widget or plugin object");
   assert($domContainer !== undefined, "$domContainer must be defined");

   this.objectToConfigure = objectToConfigure;
   this.configurationSchema = objectToConfigure.package.configurationSchema;
   this.configurationValues = objectToConfigure.configuration;
   this.configurationHandlers = [];

   var self = this;

   //we build the i18n context
   var i18nContext = i18nNamespace + ":configurationSchema.";

   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var currentValue = self.configurationValues[key];
      var handler = ConfigurationHelper.createParameterHandler(self.objectToConfigure, i18nContext, key, value, currentValue);
      self.configurationHandlers.push(handler);
   });

   $domContainer.append(this.getDOMObject());
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
ConfigurationManager.prototype.getDOMObject = function () {
   var result = "";
   $.each(this.configurationHandlers, function (key, value) {
      result += value.getDOMObject();
      result += "\n";
   });

   result += "";
   return result;
};

/**
 * Get the current configuration in the form
 * @returns {object}
 */
ConfigurationManager.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   this.configurationValues = {};
   var self = this;
   $.each(this.configurationHandlers, function (key, value) {
      self.configurationValues[value.getParamName()] = value.getCurrentConfiguration();
   });
   return this.configurationValues;
};