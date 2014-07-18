/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function SystemConfigurationManager(){}

SystemConfigurationManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.section), "json.section must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");
   assert(!isNullOrUndefined(json.defaultValue), "json.defaultValue must be defined");
   assert(!isNullOrUndefined(json.description), "json.description must be defined");
   assert(!isNullOrUndefined(json.securityAccess), "json.securityAccess must be defined");
   assert(!isNullOrUndefined(json.lastModificationDate), "json.lastModificationDate must be defined");

   return new SystemConfigurationItem(json.section, json.name, json.value,
      json.defaultValue, json.description, json.securityAccess, json.lastModificationDate);
};

//Here is the list of items of system configuration
SystemConfigurationManager.items = {};
SystemConfigurationManager.items.developerMode = "developerMode";

SystemConfigurationManager.getAll = function(callback) {
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/configuration/system")
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.SystemConfigurationManager.errorDuringGettingSystemConfiguration"), JSON.stringify(data));
            callback(null);
            return;
         }

         var result = [];

         $.each(data.data.configuration, function(index, value) {
            var sci = SystemConfigurationManager.factory(value);
            result[sci.name] = sci;
         });

         callback(result);
      })
      .fail(function() {
         notifyError($.t("objects.SystemConfigurationManager.errorDuringGettingSystemConfiguration"));
         callback(null);
      });
}

/**
 * Get Page with its id in the page. return null if it's not exist
 * @param pageId
 * @returns {Page}
 */
SystemConfigurationManager.get = function(configurationName, callback) {
   assert(!isNullOrUndefined(configurationName), "configurationName must be defined");
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/configuration/system/" + configurationName)
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.SystemConfigurationManager.errorDuringGettingSystemConfiguration"), JSON.stringify(data));
            callback(null);
            return;
         }

         var result = SystemConfigurationManager.factory(data.data);
         callback(result);
      })
      .fail(function() {
         notifyError($.t("objects.SystemConfigurationManager.errorDuringGettingSystemConfiguration"));
         callback(null);
      });
};

SystemConfigurationManager.updateToServerSync = function(systemConfigurationItem) {
   assert(!isNullOrUndefined(systemConfigurationItem), "systemConfigurationItem must be defined");
   var result = false;
   $.ajax({
      type: "PUT",
      url: "/rest/configuration/system/" + systemConfigurationItem.name,
      data: JSON.stringify(systemConfigurationItem),
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      async: false
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.SystemConfigurationManager.errorUpdating"), JSON.stringify(data));
            return;
         }
         //it's okay
         result = true;
      })
      .fail(function() {
         notifyError($.t("objects.SystemConfigurationManager.errorUpdating"));
      });

   return result;
};
