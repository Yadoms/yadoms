 /**
 *
 * @constructor
 */
function MaintenanceManager()
{
}


/**
 * Obtain from the server databaseinformation
 */
MaintenanceManager.getDatabaseDetails = function () {
   return RestEngine.getJson("rest/maintenance/information");
};

/**
 * Start a new backup task
 */
MaintenanceManager.startBackup = function() {
	return RestEngine.postJson("rest/maintenance/backup");
};

/**
 * Get the last database backup information
 */
MaintenanceManager.lastBackupInformation = function() {
	return RestEngine.getJson("rest/maintenance/backup");
};

/**
 * Delete the last database backup
 */
MaintenanceManager.deleteBackup = function(name) {
	return RestEngine.deleteJson("rest/maintenance/backup/" + name);
};


