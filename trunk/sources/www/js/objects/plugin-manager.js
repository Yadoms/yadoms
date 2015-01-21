/**
 * Created by jeanmichelDECORET on 26/05/14.
 */

/**
 * Factory which create objects from json data
 * @param json The json data
 * @returns {Plugin} List of plugins
 */
PluginManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.author), "json.author must be defined");
   assert(!isNullOrUndefined(json.description), "json.description must be defined");
   assert(!isNullOrUndefined(json.nameInformation), "json.nameInformation must be defined");
   assert(!isNullOrUndefined(json.identity), "json.identity must be defined");
   assert(!isNullOrUndefined(json.releaseType), "json.releaseType of a pluginInstance must be defined");
   assert(!isNullOrUndefined(json.url), "json.url of a pluginInstance must be defined");
   assert(!isNullOrUndefined(json.version), "json.version of a pluginInstance must be defined");
   assert(!isNullOrUndefined(json.supportManuallyCreatedDevice), "json.supportManuallyCreatedDevice of a pluginInstance must be defined");

   return new Plugin(decodeURIComponent(json.name), decodeURIComponent(json.author), decodeURIComponent(json.description), decodeURIComponent(json.nameInformation), decodeURIComponent(json.identity), json.releaseType, json.url, json.version, json.supportManuallyCreatedDevice);
};


/**
 * Function which list all available plugins
 * @param callback The callback for result
 * @param sync true to be wait for results, false to call asynchronously
 */
PluginManager.get = function (callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;

   if (!isNullOrUndefined(sync))
      async = false;

   $.ajax({
      dataType: "json",
      url: "rest/plugin",
      async: async
   })
    .done(function( data ) {
       //we parse the json answer
       if (data.result != "true")
       {
          notifyError($.t("objects.plugin.errorListing"), JSON.stringify(data));
          return;
       }

       var result = [];
          $.each(data.data.plugins, function(index, value) {
             result.push(PluginManager.factory(value));
          })

       callback(result);
    })
    .fail(function() {
       notifyError($.t("objects.plugin.errorListing"), JSON.stringify(data));
    });
};



PluginManager.getWithPackage = function (callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync))
      async = false;

   PluginManager.get( function(allPlugins) {

      $.each(allPlugins, function(index, plugin) {
         plugin.downloadPackage(function() {}, true); //force sync mode to ensure all is ok before returning
      });

      callback(allPlugins);

   } , async);

};

/**
 *
 * @constructor
 */
function PluginManager()
{

}

