/**
 * Created by sebastienGALLOU on 09/05/2017.
 */

/**
 * Obtain from the server time
 * @return server current local time
 */
YadomsTimeManager.getCurrentLocalTime = function () {
   return RestEngine.getJson("rest/system/currentTime");
};

/**
 *
 * @constructor
 */
function YadomsTimeManager()
{
}

