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
 * Get the last backup information
 */
MaintenanceManager.lastBackupInformation = function() {
	return RestEngine.getJson("rest/maintenance/backup");
};

/**
 * Delete the given backup
 */
MaintenanceManager.deleteBackup = function(name) {
	return RestEngine.deleteJson("rest/maintenance/backup/" + name);
};

/**
 * Restore the given backup
 */
MaintenanceManager.restoreBackup = function(name) {
	return RestEngine.deleteJson("rest/maintenance/restore/" + name);
};

/**
 * Delete all backups
 */
MaintenanceManager.deleteAllBackups = function() {
	return RestEngine.deleteJson("rest/maintenance/backup");
};

/**
 * Start a new log packing task
 */
MaintenanceManager.startLogsDownload = function() {
	return RestEngine.postJson("rest/maintenance/packlogs");
};

/**
 * Get the last backup information
 */
MaintenanceManager.lastLogsDownloadInformation = function() {
	return RestEngine.getJson("rest/maintenance/logs");
};

/**
 * Delete all backups
 */
MaintenanceManager.deleteAllLogs = function() {
	return RestEngine.deleteJson("rest/maintenance/logs");
};


/**
 * Start a new export data task
 */
MaintenanceManager.startExportData = function(deviceId) {
	return RestEngine.postJson("rest/maintenance/startExportData/" + deviceId);
};

/**
 * Get the last export data information
 */
MaintenanceManager.lastExportDataInformation = function() {
	return RestEngine.getJson("rest/maintenance/exportData");
};



