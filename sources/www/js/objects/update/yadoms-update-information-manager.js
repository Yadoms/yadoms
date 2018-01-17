/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 * Obtain from the server the available plugin list from the yadoms.com web site
 * @param callback
 * @param objectType
 * @param sync
 */
YadomsUpdateInformationManager.getList = function () {
   var d = new $.Deferred();
   
   RestEngine.getJson("rest/update/yadoms/list/" + i18nManager.getLanguage())
   .done(function(data) {
      $.each(data, function (versionIndex, versionValue) {
         data[versionIndex].version = new Version(data[versionIndex].version);
      });
      d.resolve(data);
   }).fail(d.reject);
   
   return d.promise();
};

YadomsUpdateInformationManager.update = function(yadomsUpdateInformation) {
   assert(!isNullOrUndefined(yadomsUpdateInformation), "yadomsUpdateInformation must be defined");
   return RestEngine.postJson("rest/update/yadoms/update", { data: JSON.stringify({ "versionData": yadomsUpdateInformation }) });
};

/**
 *
 * @constructor
 */
function YadomsUpdateInformationManager()
{
}

