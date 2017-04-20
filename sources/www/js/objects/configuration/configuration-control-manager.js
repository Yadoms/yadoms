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
   assert(i18nNamespace !== undefined, "i18nNamespace must contain widget or plugin object");
   assert($domContainer !== undefined, "$domContainer must be defined");

   if (!currentConfiguration)
       currentConfiguration = {};

   this.configurationSchema = configurationSchema || {};
   this.configurationValues = currentConfiguration;
   this.configurationHandlers = [];

   var self = this;

   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var currentValue = self.configurationValues[key];
      var handler = ConfigurationHelper.createParameterHandler(i18nNamespace, value.i18nKey, key, value, currentValue);
      if (!isNullOrUndefined(handler))
         self.configurationHandlers.push(handler);
   });

   $domContainer.append(this.getDOMObject());
   //after appending the dom object we can call the applyScript method to finish control instantiation
   this.applyScript();
}

ConfigurationControlManager.prototype.afterI18n = function() {
   $.each(this.configurationHandlers, function (key, value) {
      if ($.isFunction(value.afterI18n)) {
         value.afterI18n();
      }
   });
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
   var d= new $.Deferred();
   var self = this;
   self.configurationValues = {};
   var deferredArray =[];
   
   $.each(self.configurationHandlers, function (key, value) {
      var deferred = value.getCurrentConfiguration();
      deferredArray.push(deferred);
      deferred.done(function (currentConfiguration) {
         self.configurationValues[value.getParamName()] = currentConfiguration;
      });      
   });

   $.whenAll(deferredArray)
   .done(function() {
      d.resolve(self.configurationValues);
   });   
   
   return d.promise();   
};