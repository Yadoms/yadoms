/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */


YadomsUpdateInformationManager.update = function(downloadUrl) {
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   return RestEngine.postJson("rest/update/yadoms/update", { data: JSON.stringify({ "downloadUrl": downloadUrl }) });
};

/**
 *
 * @constructor
 */
function YadomsUpdateInformationManager()
{
}

