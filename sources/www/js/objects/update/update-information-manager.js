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
   assert(!isNullOrUndefined(json.version), "json.version must be defined");
   assert(!isNullOrUndefined(json.downloadUrl), "json.downloadUrl must be defined");

   return new UpdateInformation(json.type, json.name,
                     json.author,
                     json.description,
                     (!isNullOrUndefined(json.url)?json.url:""),
                     (!isNullOrUndefined(json.credits)?json.credits:""),
                     json.version,
                     json.downloadUrl,
                     json.iconUrl);
};

/**
 * Obtain from the server the available plugin/widget/scriptInterpreter list from the yadoms.com web site
 * @param callback
 * @param objectType : "plugin", "widget", "scriptInterpreter"
 * @param sync
 */
UpdateInformationManager.getList = function(objectType, includePreReleases) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");

   var d = new $.Deferred();
   debugger;

   RestEngine.getJson("rest/update/" + objectType + "/list/" + (includePreReleases ? "includePreReleases/" : "releasesOnly/") + i18n.lng())
   .done(function(data) {
      var result = {};
      $.each(data, function(index, value) {
         //value is a list of all version available
          if (value && value.length > 0) {
              $.each(value,
                  function(versionIndex, versionValue) {
                      try {
                          var infos = UpdateInformationManager.factory(versionValue);
                          //we push into list only if the factory succeed
                          if (!result[index])
                              result[index] = [];
                          result[index].push(infos);
                      } catch (e) {
                          console.warn("Fail to parse " + objectType + " (" + index + ") package.");
                          console.warn("    name:" + versionValue.name);
                          console.warn("    version:" + versionValue.version);
                          console.warn("    type:" + versionValue.type);
                          console.warn("    description:" + versionValue.description);
                          console.warn("    url:" + versionValue.url);
                          console.warn("    author:" + versionValue.author);
                          console.warn("    credits:" + versionValue.credits);
                          console.warn("    downloadUrl:" + versionValue.downloadUrl);
                          console.warn("    iconUrl:" + versionValue.iconUrl);
                          console.warn("    md5Hash:" + versionValue.md5Hash);
                          console.warn(e);
                      }
                  });
          }
      });

      d.resolve(result);
   })
   .fail(d.reject);

   return d.promise();
};

UpdateInformationManager.install = function(objectType, downloadUrl) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   return RestEngine.postJson("rest/update/" + objectType + "/install", { data: JSON.stringify({ "downloadUrl": downloadUrl }) });
};

UpdateInformationManager.update = function(objectType, type, downloadUrl) {
   assert(!isNullOrUndefined(objectType), "objectType must be defined");
   assert(!isNullOrUndefined(type), "type must be defined");
   return RestEngine.postJson("rest/update/" + objectType + "/update/" + type, { data: JSON.stringify({ "downloadUrl": downloadUrl }) });
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

