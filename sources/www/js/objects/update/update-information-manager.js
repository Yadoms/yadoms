/**
 * Created by jeanmichelDECORET on 26/05/14.
 */

/**
 * Obtain from the server the available plugin/widget/scriptInterpreter list from the yadoms.com web site
 * @param callback
 * @param objectType : "plugin", "widget", "scriptInterpreter"
 * @param sync
 */
UpdateInformationManager.getList = function(includePreReleases) {
   return RestEngine.getJson("rest/update/list/" + (includePreReleases ? "includePreReleases/" : "releasesOnly/"));
};

UpdateInformationManager.scanForUpdates = function() {
   return RestEngine.postJson("rest/update/scan");
};

UpdateInformationManager.install = function(objectType, downloadUrl) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   return RestEngine.postJson("rest/update/" + objectType + "/install", { data: JSON.stringify({ "downloadUrl": downloadUrl }) });
};

UpdateInformationManager.update = function(objectType, itemType, downloadUrl) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");
   assert(!isNullOrUndefined(itemType), "itemType must be defined");
   return RestEngine.postJson("rest/update/" + objectType + "/update/" + itemType, { data: JSON.stringify({ "downloadUrl": downloadUrl }) });
};

/**
 * Permit to remove the plugin from the system
 * @param pluginName
 * @param callback
 * @param sync
 */
UpdateInformationManager.remove = function(objectType, type) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");
   assert(!isNullOrUndefined(type), "type must be defined");
   return RestEngine.postJson("rest/update/" + objectType + "/remove/" + type);
};

/**
 *
 * @constructor
 */
function UpdateInformationManager()
{
}

