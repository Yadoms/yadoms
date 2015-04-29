/**
 * Created by jmdecoret on 29/04/2015.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function UpdateManager(){}


/**
 * Start a server check for update of yadoms
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.checkForUpdateYadoms = function(callback, sync) {
    UpdateManager._sendQuery("yadoms", "check", callback, sync);
};

/**
 * Start a server update of yadoms
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.updateYadoms = function(callback, sync) {
    UpdateManager._sendQuery("yadoms", "update", callback, sync);
};

/**
 * Start a server check for update of a plugin
 * @param {string} pluginName The plugin name
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.checkForUpdatePlugin = function(pluginName, callback, sync) {
    UpdateManager._sendQuery("plugin/" + pluginName, "check", callback, sync);
};

/**
 * Start a server update of a plugin
 * @param {string} pluginName The plugin name
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.updatePlugin = function(pluginName, callback, sync) {
    UpdateManager._sendQuery("plugin/" + pluginName, "update", callback, sync);
};


/**
 * Start a server check for update of a widget
 * @param {string} widgetName The widget name
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.checkForUpdateWidget = function(widgetName, callback, sync) {
    UpdateManager._sendQuery("widget/" + widgetName, "check", callback, sync);
};

/**
 * Start a server update of a widget
 * @param {string} widgetName The widget name
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
UpdateManager.updatePlugin = function(widgetName, callback, sync) {
    UpdateManager._sendQuery("widget/" + widgetName, "update", callback, sync);
};



/**
 * Send an update or a check for update to server
 * @param {string} component The component to check (yadoms, plugin/pluginName,...)
 * @param {string} method Can be "check" ou "update"
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 * @private
 */
UpdateManager._sendQuery = function(component, method, callback, sync) {
    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        type: "POST",
        url: "/rest/update/"+ component + "/" + method,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
                if(method == "check")
                    notifyError($.t("objects.update.errorChecking", {objectName : component }), JSON.stringify(data));
                else
                    notifyError($.t("objects.update.errorUpdating", {objectName : component }), JSON.stringify(data));
            }
            if ($.isFunction(callback))
                callback();
        })
        .fail(function() {
            if(method == "check")
                notifyError($.t("objects.update.errorChecking", {objectName : component }));
            else
                notifyError($.t("objects.update.errorUpdating", {objectName : component }));
        });
};
