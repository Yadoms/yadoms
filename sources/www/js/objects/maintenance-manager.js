 /**
 *
 * @constructor
 */
function MaintenanceManager()
{
}


/**
 * Obtain from the server information
 * @param callback
 * @param objectType
 * @param sync
 */
MaintenanceManager.getDatabaseDetails = function () {
   return RestEngine.getJson("rest/maintenance/information");
};

