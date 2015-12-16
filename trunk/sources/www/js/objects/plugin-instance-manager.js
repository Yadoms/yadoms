/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function PluginInstanceManager(){}

/**
 * The system category string
 * @type {string}
 * @private
 */
PluginInstanceManager._systemCategory = "system";

/**
 * Create a plugin instance object from server JSON
 * @param json The json data provided by server
 * @returns {PluginInstance} The created instance
 */
PluginInstanceManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");
   assert(!isNullOrUndefined(json.autoStart), "json.autoStart must be defined");
   assert(!isNullOrUndefined(json.category), "json.category of a pluginInstance must be defined");

   return new PluginInstance(json.id, json.displayName, json.type, json.configuration, json.autoStart, json.category);
};

/**
 * Tells if a plugin instance is system category
 * @returns {boolean}
 */
PluginInstanceManager.isSystemCategory = function(pluginInstance) {
    return pluginInstance.category.toLowerCase()== PluginInstanceManager._systemCategory.toLowerCase();
};

/**
 * Get a plugin instance using its ID
 * @param pluginInstanceId The plugin instance id to get
 * @param callback The callback used to return the plugin instance found
 * @param sync true to get result synchronously
 */
PluginInstanceManager.get = function (pluginInstanceId, callback, sync) {
   assert(!isNullOrUndefined(pluginInstanceId), "pluginInstanceId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

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
};

/**
 * Function which list all available plugins instance
 * @param callback The callback for result
 * @param sync true to be wait for results, false to call asynchronously
 */
PluginInstanceManager.getAll = function (callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;

   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/plugin/instance",
      async: async
   })
       .done(
       /**
        * Receive result from server
        * @param {{result:string}, {data: {plugin : object}}} data
        */
       function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorLoading", {objectName:"plugin instances"}), JSON.stringify(data));
             return;
          }

          var result = [];
          data.data.plugin.forEach(function( value) {
             var pi = PluginInstanceManager.factory(value);
             PluginInstanceManager.downloadPackage(pi, undefined, true);
             result.push(pi);
          });

          callback(result);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorLoading", {objectName:"plugin instances"}));
       });
};

/**
 * Get the pluginInstance state
 * @param pluginInstance
 * @param callback
 */
PluginInstanceManager.getState = function (pluginInstance, callback) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");
   assert($.isFunction(callback), "callback must be a function");

   //we ask for the started status of current pluginInstance only on non system plugins
    if (!pluginInstance.isSystemCategory()) {
        $.getJSON("/rest/plugin/" + pluginInstance.id + "/state/")
            .done(function(data) {
                callback(data.result == "true" ? data.data : "kUnknwonValue");
            })
            .fail(function () { notifyError($.t("objects.pluginInstance.errorGettingState", { pluginName: pluginInstance.displayName })); });
    }
}; 

/**
 * Start a plugin instance
 * @param pluginInstance The plugin instance to start
 * @param callback The callback for the result
 * @param sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.start = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't start system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id + "/start",
         contentType: "application/json; charset=utf-8",
         dataType: "json",
         async: async
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : pluginInstance.displayName}), JSON.stringify(data));
               return;
            }
            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : pluginInstance.displayName}));});
   }
};

/**
 * Stop a plugin instance
 * @param pluginInstance The plugin instance to stop
 * @param callback The callback for the result
 * @param sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.stop = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't stop system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id + "/stop",
         contentType: "application/json; charset=utf-8",
         dataType: "json",
         async: async
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : pluginInstance.displayName}), JSON.stringify(data));
               return;
            }

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : pluginInstance.displayName}));});
   }
};

/**
 * Delete a plugin instance
 * @param pluginInstance The plugin instance to delete
 * @param callback The callback for the result
 * @param sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.deleteFromServer = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't delete system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "DELETE",
         url: "/rest/plugin/" + pluginInstance.id,
         contentType: "application/json; charset=utf-8",
         dataType: "json",
          async: async
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : pluginInstance.displayName}), JSON.stringify(data));
               return;
            }

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : pluginInstance.displayName}));});
   }
};

/**
 * Create a plugin instance to server
 * @param {object} pluginInstance The plugin instance to create to server
 * @param {function(boolean)} callback The callback for the result
 * @param {boolean} sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.createToServer = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't create system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "POST",
         url: "/rest/plugin",
         data: JSON.stringify({ displayName: pluginInstance.displayName, type: pluginInstance.type, configuration: pluginInstance.configuration, autoStart: pluginInstance.autoStart}),
         contentType: "application/json; charset=utf-8",
         dataType: "json",
          async: async
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : pluginInstance.displayName}), JSON.stringify(data));
               //launch callback with false as ko result
               if ($.isFunction(callback))
                  callback(false);
               return;
            }

            //we update our information from the server
            pluginInstance.id = data.data.id;
            pluginInstance.displayName = data.data.displayName;
            pluginInstance.type = data.data.type;
            pluginInstance.configuration = data.data.configuration;
            pluginInstance.autoStart = parseBool(data.data.autoStart);

            if ($.isFunction(callback))
               callback(true);
         })
         .fail(function() {
            notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : pluginInstance.displayName}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
         });
   }
};

/**
 * Update a plugin instance to server
 * @param pluginInstance The plugin instance to update
 * @param {function(boolean)} callback The callback for the result
 * @param {boolean} sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.updateToServer = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't update system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "PUT",
         url: "/rest/plugin/" + pluginInstance.id,
         data: JSON.stringify(pluginInstance),
         contentType: "application/json; charset=utf-8",
         dataType: "json",
          async: async
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : pluginInstance.displayName}), JSON.stringify(data));
               //launch callback with false as ko result
               if ($.isFunction(callback))
                  callback(false);
            }
            else {
                //it's okay
                //we update our information from the server
                pluginInstance = PluginInstanceManager.factory(data.data);

                //we call the callback with true as a ok result
                if ($.isFunction(callback))
                   callback(true);
            }
          })
         .fail(function() {
            notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : pluginInstance.displayName}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
         });
   }
};

/**
 * Download a plugin package for an instance (asynchronously)
 * @param pluginInstance The plugin instance
 * @param callback The callback for receiving the plugin package
 */
PluginInstanceManager.downloadPackage = function(pluginInstance, callback, sync) {
   assert(!isNullOrUndefined(pluginInstance), "pluginInstance must be defined");

   //we can't download package from system plugins
   if (!pluginInstance.isSystemCategory()) {

       var async = true;
       if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
           async = !sync;

       $.ajax({
           type: "GET",
           url: "plugins/" + pluginInstance.type + "/package.json",
           contentType: "application/json; charset=utf-8",
           dataType: "json",
           async: async
       }).done(function (data) {

            pluginInstance.package = data;

            //we manage i18n
            i18n.options.resGetPath = 'plugins/__ns__/locales/__lng__.json';
            i18n.loadNamespace(pluginInstance.type);

            //we restore the resGetPath
            i18n.options.resGetPath = "locales/__lng__.json";

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() {
             notifyError($.t("objects.pluginInstance.errorGettingPackage", {pluginName : pluginInstance.displayName}));
             if ($.isFunction(callback))
                callback();
          });
   } else {
      if ($.isFunction(callback))
         callback();
   }
};

/**
 * Get all plugin instances which supports manually created device
 * @param callback The callback for results
 */
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

           if(!isNullOrUndefined(data.data.plugin)) {
               $.each(data.data.plugin, function (index, value) {
                   var pi = PluginInstanceManager.factory(value);
                   //we don't show system plugins to user
                   if (!pi.isSystemCategory()) {
                       pluginInstanceList.push(pi);
                   }
               });
           }
         callback(pluginInstanceList);
      })
      .fail(function() {
         notifyError($.t("modals.pluginInstance.errorGettingManuallyDeviceCreation"));
         callback(null);
      });
};

/**
 * Create a device manually
 * @param pluginInstance The plugin instance
 * @param deviceName The device name
 * @param deviceConfiguration The device configuration
 * @param {function()} callback The callback when operation ends
 * @param {boolean} sync True to wait for result, false (or undefined) to work asynchronously
 */
PluginInstanceManager.createManuallyDevice = function(pluginInstance, deviceName, deviceConfiguration, callback, sync) {
   assert($.isFunction(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

   //we can't ask for manually creation of a device from system plugins
   if (!pluginInstance.isSystemCategory()) {
      $.ajax({
         type: "POST",
         url: "/rest/plugin/" + pluginInstance.id + "/createDevice",
         data: JSON.stringify({ name: deviceName, configuration: deviceConfiguration}),
         contentType: "application/json; charset=utf-8",
         dataType: "json",
          async: async
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
};