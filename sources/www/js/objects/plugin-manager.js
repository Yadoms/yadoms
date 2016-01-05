/**
 * Created by jeanmichelDECORET on 26/05/14.
 */

/**
 * This associative array based on plugin Type contains all plugin type information with their package
 * @type {Array}
 */
PluginManager.pluginTypes = [];

/**
 * Factory which create objects from json data
 * @param json The json data
 * @returns {Plugin} List of plugins
 */
PluginManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   return new Plugin(json);
};

/**
 * Function which list all available plugins with their packages
 * @param callback The callback for result
 * @param sync true to be wait for results, false to call asynchronously
 */
PluginManager.get = function (callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/plugin",
      async: async
   })
    .done(function( data ) {
       //we parse the json answer
       if (data.result != "true") {
          notifyError($.t("objects.plugin.errorListing"), JSON.stringify(data));
          return;
       }

       //we've got a list of plugin type. For each of one we download the package.json
       PluginManager.pluginTypes = [];
       var pluginCountRemaining =  data.data.plugins.length;
       i18n.options.resGetPath = '__ns__/locales/__lng__.json';

       $.each(data.data.plugins, function (index, pluginType) {
           i18n.loadNamespace("plugins/" + pluginType);
           PluginManager.downloadPackage(pluginType, function(package) {
              PluginManager.pluginTypes[pluginType] = PluginManager.factory(package);
               pluginCountRemaining--;

               if(pluginCountRemaining <=0 ) {
                   //we restore the resGetPath
                   i18n.options.resGetPath = "locales/__lng__.json";
                   callback(PluginManager.pluginTypes);
               }
           });
       });
    })
    .fail(function() {
       notifyError($.t("objects.plugin.errorListing"));
    });
};

/**
 * Download a plugin package for an instance (asynchronously)
 * @param pluginInstance The plugin instance
 * @param callback The callback for receiving the plugin package
 */
PluginManager.downloadPackage = function(pluginType, callback, sync) {
    assert(!isNullOrUndefined(pluginType), "plugin Type must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    //we can't download package from system plugins
    $.ajax({
        dataType: "json",
        url: "plugins/" + pluginType + "/package.json",
        async: async
    })
    .done(function( data ) {

        i18n.loadNamespace("plugins/" + pluginType, function () {
            if ($.isFunction(callback))
                callback(data);
        });
    })
    .fail(function() {
        notifyError($.t("objects.pluginInstance.errorGettingPackage", {pluginName : pluginType}));
        if ($.isFunction(callback))
            callback();
    });
};

/**
 *
 * @constructor
 */
function PluginManager()
{
}

