/**
 * Created by jeanmichelDECORET on 26/05/14.
 */

/**
 * Factory which create objects from json data
 * @param json The json data
 * @returns {Plugin} List of plugins
 */
UpdateInformationManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.author), "json.author must be defined");
   assert(!isNullOrUndefined(json.description), "json.description must be defined");
   assert(!isNullOrUndefined(json.releaseType), "json.releaseType must be defined");
   assert(!isNullOrUndefined(json.version), "json.version of a pluginInstance must be defined");
   assert(!isNullOrUndefined(json.downloadUrl), "json.downloadUrl of a pluginInstance must be defined");
   assert(!isNullOrUndefined(json.iconUrl), "json.iconUrl of a pluginInstance must be defined");

   return new UpdateInformation(json.type, decodeURIComponent(json.name),
                     decodeURIComponent(json.author),
                     decodeURIComponent(json.description),
                     decodeURIComponent(json.releaseType),
                     (!isNullOrUndefined(json.url)?json.url:""),
                     (!isNullOrUndefined(json.credits)?json.credits:""),
                     json.version,
                     json.downloadUrl,
                     json.iconUrl);
};

/**
 * Obtain from the server the available plugin list from the yadoms.com web site
 * @param callback
 * @param sync
 */
UpdateInformationManager.getPluginList = function(callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/update/plugin/list/" + i18n.lng(),
      async: async
   })
   .done(function( data ) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.update-information.errorListingPlugin"), JSON.stringify(data));
         callback(false);
         return;
      }

      var result = {};
      $.each(data.data, function(index, value) {
         //value is a list of all version available
         result[index] = [];
         $.each(value, function(versionIndex, versionValue) {
            try {
               result[index].push(UpdateInformationManager.factory(versionValue));
            }
            catch (e) {
               console.error(e);
            }
         });
      });

      callback(result);
   })
   .fail(function() {
      notifyError($.t("objects.update-information.errorListingPlugin"));
      callback(false);
   });
};

UpdateInformationManager.installPlugin = function(downloadUrl, callback, sync) {
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/update/plugin/install",
      async: async,
      data: JSON.stringify({"downloadUrl" : downloadUrl}),
      type: "POST",
      contentType: "application/json; charset=utf-8"
   })
       .done(function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorInstalling", {objectName : "plugin"}), JSON.stringify(data));
             return;
          }

          //we launch the callback with the task id
          callback(data.data.taskId);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorInstalling", {objectName : "plugin"}));
       });
};

UpdateInformationManager.updatePlugin = function(pluginType, downloadUrl, callback, sync) {
   assert(!isNullOrUndefined(pluginType), "pluginName must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/update/plugin/update/" + pluginType,
      async: async,
      data: JSON.stringify({"downloadUrl" : downloadUrl}),
      type: "POST",
      contentType: "application/json; charset=utf-8"
   })
       .done(function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorUpdating", {objectName : "plugin"}), JSON.stringify(data));
             return;
          }

          callback(data.data.taskId);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorUpdating", {objectName : "plugin"}));
       });
};

/**
 * Permit to remove the plugin from the system
 * @param pluginName
 * @param callback
 * @param sync
 */
UpdateInformationManager.removePlugin = function(pluginType, callback, sync) {
   assert(!isNullOrUndefined(pluginType), "pluginName must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/update/plugin/remove/" + pluginType,
      async: async,
      data: JSON.stringify({"downloadUrl" : downloadUrl}),
      type: "POST",
      contentType: "application/json; charset=utf-8"
   })
       .done(function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorUpdating", {objectName : "plugin"}), JSON.stringify(data));
             return;
          }

          callback(data.data.taskId);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorUpdating", {objectName : "plugin"}));
       });
};

/**
 * Compare the two UpdateInformationObjects. Return > 0 if item1 is lower, =0 if the same and <0 if item1 is higher
  */
UpdateInformationManager.compareVersion = function(item1, item2) {
   return item2.version - item1.version;
};

/**
 *
 * @constructor
 */
function UpdateInformationManager()
{
}

