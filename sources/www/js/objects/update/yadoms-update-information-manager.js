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
 * Compare the two UpdateInformationObjects. Return > 0 if item1 is lower, =0 if the same and <0 if item1 is higher
  */
YadomsUpdateInformationManager.compareVersion = function(item1, item2) {
   return item2.version - item1.version;
};

/**
 *
 * @constructor
 */
function YadomsUpdateInformationManager()
{
}

