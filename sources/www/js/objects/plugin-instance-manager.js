/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function PluginInstanceManager(){}

PluginInstanceManager.systemCategory = "system";

PluginInstanceManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");
   assert(!isNullOrUndefined(json.autoStart), "json.autoStart must be defined");
   assert(!isNullOrUndefined(json.category), "json.category of a pluginInstance must be defined");

   return new PluginInstance(json.id, decodeURIComponent(json.name), json.type, json.configuration, json.autoStart, json.category);
};

PluginInstanceManager.get = function (pluginInstanceId, callback, sync) {
   assert(!isNullOrUndefined(pluginInstanceId), "pluginInstanceId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;

   if (!isNullOrUndefined(sync))
      async = false;

   $.ajax({
      dataType: "json",
      url: "rest/plugin/" + pluginInstanceId,
      async: async
   })
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "Plugin with Id = " + pluginInstanceId}), JSON.stringify(data));
            return;
         }
         callback(PluginInstanceManager.factory(data.data));
      })
      .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "Plugin with Id = " + pluginInstanceId}));});
}

PluginInstanceManager.getStatus = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
   //we ask for the status of current pluginInstance only on non system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.getJSON("/rest/plugin/" + pluginInstance.id + "/status/")
         .done(function(data) {
            //we parse the json answer
            //the default answer is false
            var result = false;
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorGettingStatus", {pluginName : pluginInstance.name}), JSON.stringify(data));
               return;
            }
            if (parseBool(data.data.running))
            {
               result = true;
            }
            if ($.isFunction(callback))
               callback(result);

            //we update the lastRunningStatus
            pluginInstance.lastRunningStatus = result;

            return result;
         })
         .fail(function() { notifyError($.t("objects.pluginInstance.errorGettingStatus", {pluginName : pluginInstance.name})); });
   }
};

PluginInstanceManager.start = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
   //we can't start system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id + "/start",
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : pluginInstance.name}), JSON.stringify(data));
               return;
            }
            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : pluginInstance.name}));});
   }
};

PluginInstanceManager.stop = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
   //we can't stop system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id + "/stop",
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : pluginInstance.name}), JSON.stringify(data));
               return;
            }

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : pluginInstance.name}));});
   }
};

PluginInstanceManager.deleteFromServer = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   //we can't delete system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "DELETE",
         url: "/rest/plugin/" + pluginInstance.id,
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : pluginInstance.name}), JSON.stringify(data));
               return;
            }

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : pluginInstance.name}));});
   }
};

PluginInstanceManager.createToServer = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   //we can't create system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "POST",
         url: "/rest/plugin",
         data: JSON.stringify({ name: pluginInstance.name, type: pluginInstance.type, configuration: pluginInstance.configuration, autoStart: pluginInstance.autoStart}),
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : pluginInstance.name}), JSON.stringify(data));
               //launch callback with false as ko result
               if ($.isFunction(callback))
                  callback(false);
               return;
            }

            //we update our information from the server
            pluginInstance.id = data.data.id;
            pluginInstance.name = data.data.name;
            pluginInstance.type = data.data.type;
            pluginInstance.configuration = data.data.configuration;
            pluginInstance.autoStart = parseBool(data.data.autoStart);

            if ($.isFunction(callback))
               callback(true);
         })
         .fail(function() {
            notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : pluginInstance.name}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
         });
   }
};

PluginInstanceManager.updateToServer = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   //we can't update system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id,
         data: JSON.stringify(pluginInstance),
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : pluginInstance.name}), JSON.stringify(data));
               //launch callback with false as ko result
               if ($.isFunction(callback))
                  callback(false);
               return;
            }
            //it's okay
            //we update our information from the server
            pluginInstance = PluginInstanceManager.factory(data.data);

            //we call the callback with true as a ok result
            if ($.isFunction(callback))
               callback(true);
         })
         .fail(function() {
            notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : pluginInstance.name}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
         });
   }
};

PluginInstanceManager.downloadPackage = function(pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   //we can't download package from system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.getJSON( "plugins/" + pluginInstance.type + "/package.json")
         .done(function (data) {

            pluginInstance.package = data;

            //we manage i18n
            i18n.options.resGetPath = 'plugins/__ns__/locales/__lng__.json';
            i18n.loadNamespace(pluginInstance.type);

            //we restore the resGetPath
            i18n.options.resGetPath = "locales/__lng__.json";

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorGettingPackage", {pluginName : pluginInstance.name}));});
   }
};


PluginInstanceManager.getPluginInstanceHandleManuallyDeviceCreation = function(callback) {
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/plugin/instance/handleManuallyDeviceCreation")
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.pluginInstance.errorGettingManuallyDeviceCreation"), JSON.stringify(data));
            return;
         }
         var pluginInstanceList = [];
         
         $.each(data.data.plugin, function(index, value) {
            var pi = PluginInstanceManager.factory(value);
            //we don't show system plugins to user
            if (pi.category != PluginInstanceManager.systemCategory) {
               pluginInstanceList.push(pi);
            }
         });
         callback(pluginInstanceList);
      })
      .fail(function() {
         notifyError($.t("modals.pluginInstance.errorGettingManuallyDeviceCreation"));
         callback(null);
      });
}

PluginInstanceManager.createManuallyDevice = function(pluginInstance, deviceName, deviceConfiguration, callback) {
   assert($.isFunction(callback), "callback must be defined");

   //we can't ask for manually creation of a device from system plugins
   if (pluginInstance.category != PluginInstanceManager.systemCategory) {
      $.ajax({
         type: "POST",
         url: "/rest/plugin/" + pluginInstance.id + "/createDevice",
         data: JSON.stringify({ name: deviceName, configuration: deviceConfiguration}),
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorCreatingManuallyDevice", {deviceName : deviceName}), JSON.stringify(data));
               return;
            }
            callback();
         })
         .fail(function() {
            notifyError($.t("objects.pluginInstance.errorCreatingManuallyDevice", {deviceName : deviceName}));
         });
   }
}