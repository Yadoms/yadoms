/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */


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

