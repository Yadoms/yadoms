/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function ConfigurationManager(){}

ConfigurationManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.section), "json.section must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");
   assert(!isNullOrUndefined(json.defaultValue), "json.defaultValue must be defined");
   assert(!isNullOrUndefined(json.description), "json.description must be defined");
   assert(!isNullOrUndefined(json.securityAccess), "json.securityAccess must be defined");
   assert(!isNullOrUndefined(json.lastModificationDate), "json.lastModificationDate must be defined");

   return new ConfigurationItem(json.section, json.name, ConfigurationItem.decodeValue(json.value),
       ConfigurationItem.decodeValue(json.defaultValue), decodeURIComponent(json.description), json.securityAccess, json.lastModificationDate);
};

//Here is the list of items of system configuration
ConfigurationManager.items = {};
ConfigurationManager.items.system = {};
ConfigurationManager.items.systemSection = "system";
ConfigurationManager.items.system.developerMode = "developerMode";
ConfigurationManager.items.system.language = "language";
ConfigurationManager.items.system.timezone = "timezone";

ConfigurationManager.items.system.basicAuthentication = "basicAuthentication";
ConfigurationManager.items.system.basicAuthenticationUser = "basicAuthenticationUser";
ConfigurationManager.items.system.basicAuthenticationPassword = "basicAuthenticationPassword";
ConfigurationManager.items.system.basicAuthenticationPassword2 = "basicAuthenticationPassword2";

ConfigurationManager.items.system.advancedParameters = "advancedParameters";
ConfigurationManager.items.system.dateFormatString = "dateFormatString";

ConfigurationManager.items.install = {};
ConfigurationManager.items.installSection = "install";
ConfigurationManager.items.install.firstStart = "firstStart";

ConfigurationManager.getSection = function(section, callback) {
   assert(!isNullOrUndefined(section), "section must be defined");
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/configuration/" + section)
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.ConfigurationManager.errorDuringGettingSystemConfiguration"), JSON.stringify(data));
            callback(null);
            return;
         }

         var result = [];

         $.each(data.data.configuration, function(index, value) {
            var ci = ConfigurationManager.factory(value);
            result[ci.name] = ci;
         });

         callback(result);
      })
      .fail(function() {
         notifyError($.t("objects.ConfigurationManager.errorDuringGettingSystemConfiguration"));
         callback(null);
      });
}

ConfigurationManager.get = function(configurationSection, configurationName, callback) {
   assert(!isNullOrUndefined(configurationSection), "configurationSection must be defined");
   assert(!isNullOrUndefined(configurationName), "configurationName must be defined");
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/configuration/" + configurationSection + "/" + configurationName)
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            //we don't signal error. If necessary it will be done in the callback
            callback(null);
            return;
         }

         var result = ConfigurationManager.factory(data.data);
         callback(result);
      })
      .fail(function() {
         notifyError($.t("objects.ConfigurationManager.errorDuringGettingSystemConfiguration"));
         callback(null);
      });
};

ConfigurationManager.getSync = function(configurationSection, configurationName) {
   assert(!isNullOrUndefined(configurationSection), "configurationSection must be defined");
   assert(!isNullOrUndefined(configurationName), "configurationName must be defined");

   var result = null;

   $.ajax({
      type: "GET",
      url: "rest/configuration/" + configurationSection + "/" + configurationName,
      dataType: "json",
      async: false
   })
   .done(function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         //we don't signal error. If necessary it will be done in the callback
      }
      else {
         result = ConfigurationManager.factory(data.data);
      }
   })
   .fail(function() {
      notifyError($.t("objects.ConfigurationManager.errorDuringGettingSystemConfiguration"));
   });

   return result;
};

ConfigurationManager.updateToServerSync = function(configurationItem) {
   assert(!isNullOrUndefined(configurationItem), "configurationItem must be defined");
   var result = false;
   $.ajax({
      type: "PUT",
      url: "/rest/configuration/" + configurationItem.section + "/" + configurationItem.name,
      data: JSON.stringify(configurationItem),
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      async: false
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorUpdating", {objectName: "configuration"}), JSON.stringify(data));
            return;
         }
         //it's okay
         result = true;
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorUpdating", {objectName: "configuration"}));
      });

   return result;
};

//TODO : make it use a ConfigurationItem object
ConfigurationManager.createToServerSync = function(section, name, value, defaultValue, description, securityAccess) {
   assert(!isNullOrUndefined(section), "section must be defined");
   assert(!isNullOrUndefined(name), "name must be defined");
   assert(!isNullOrUndefined(value), "value must be defined");

   if (isNullOrUndefined(securityAccess))
      securityAccess = "none";

   if (isNullOrUndefined(defaultValue))
      defaultValue = value;

   var result = null;
   $.ajax({
      type: "PUT",
      url: "/rest/configuration/" + section + "/" + name,
      data: JSON.stringify({ "section": section,
              "name": name,
              "value": value,
              "defaultValue": defaultValue,
              "description": description,
              "securityAccess": securityAccess
      }),
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      async: false
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorCreating", {objectName : "configuration named " + name + " in section " + section}), JSON.stringify(data));
         }
         else {
            //it's okay
            result = data.data;
         }
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorCreating", {objectName : "configuration named " + name + " in section " + section}));
      });

   return result;
};
