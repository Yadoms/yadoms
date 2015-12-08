/**
 * Created by Nicolas on 28/02/14.
 */

/**
 * Creates an instance of LazyLoaderManager
 * @param configurationSchema
 * @param currentConfiguration
 * @param i18nNamespace
 * @param $domContainer
 * @constructor
 */
function ConfigurationControlManager(configurationSchema, currentConfiguration, i18nNamespace, $domContainer) {
   assert(configurationSchema !== undefined, "configurationSchema must contain widget or plugin object");
   assert(i18nNamespace !== undefined, "i18nNamespace must contain widget or plugin object");
   assert($domContainer !== undefined, "$domContainer must be defined");

   if (!currentConfiguration)
       currentConfiguration = {};

   this.configurationSchema = configurationSchema;
   this.configurationValues = currentConfiguration;
   this.configurationHandlers = [];

   var self = this;

   //we build the i18n context
   var i18nContext = i18nNamespace;

   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var currentValue = self.configurationValues[key];
      var handler = ConfigurationHelper.createParameterHandler(i18nContext, key, value, currentValue);
      if (!isNullOrUndefined(handler))
         self.configurationHandlers.push(handler);
   });

   $domContainer.append(this.getDOMObject());
   //after appending the dom object we can call the applyScript method to finish control instantiation
   this.applyScript();
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
ConfigurationControlManager.prototype.getDOMObject = function () {
   var result = "";
   $.each(this.configurationHandlers, function (key, value) {
      result += value.getDOMObject();
      result += "\n";
   });

   result += "";
   return result;
};

/**
 * Apply script after DOM object has been added to the page
 */
ConfigurationControlManager.prototype.applyScript = function () {
   //we apply script in each children
   $.each(this.configurationHandlers, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });
};

/**
 * Get the current configuration in the form
 * @returns {object}
 */
ConfigurationControlManager.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   this.configurationValues = {};
   var self = this;
   $.each(this.configurationHandlers, function (key, value) {
      self.configurationValues[value.getParamName()] = value.getCurrentConfiguration();
   });
   return this.configurationValues;
};