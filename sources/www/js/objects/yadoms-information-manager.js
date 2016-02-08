/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 * Obtain from the server information
 * @param callback
 * @param objectType
 * @param sync
 */
YadomsInformationManager.getList = function () {
   return RestEngine.getJson("rest/system/information");
};

/**
 *
 * @constructor
 */
function YadomsInformationManager()
{
}

