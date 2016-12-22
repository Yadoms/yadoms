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
   return RestEngine.getJson("rest/update/yadoms/list/" + i18n.lng());
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

