/**
 * Created by jmdecoret on 01/04/2015.
 */


/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function TaskManager(){}

/**
 * Define the list of all known task type
 * @type {string[]}
 */
TaskManager.knownTaskType = {
    yadomsUpdate : "system.yadomsUpdate"
};

/**
 * Create a new task (from server data only)
 * @param json Json data from server
 * @returns {Task} a task object
 */
TaskManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.uuid), "json.uuid must be defined");
    assert(!isNullOrUndefined(json.name), "json.id must be defined");
    assert(!isNullOrUndefined(json.status), "json.id must be defined");

    return new Task(json.uuid, decodeURIComponent(json.name), json.status, json.creationDate, json.progression, decodeURIComponent(json.message));
};

/**
 * Create a task
 * @param {string} taskType The task type
 * @param {function} callback The callback
 * @param {boolean} sync specify true for synchronous call to server
 */
TaskManager.createTask = function(taskType, callback, sync) {
    assert(!isNullOrUndefined(taskType), "taskType must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        type: "POST",
        url: "/rest/task/",
        data: JSON.stringify({ type: taskType }),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
                notifyError($.t("objects.generic.errorCreating", {objectName : taskType}), JSON.stringify(data));
                return;
            }

            var createdTask = TaskManager.factory(data.data);
            if ($.isFunction(callback))
                callback(createdTask);
        })
        .fail(function() {notifyError($.t("objects.generic.errorCreating", {objectName : pageName}));});
};
