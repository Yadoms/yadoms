/**
 * Created by jmdecoret on 01/04/2015.
 */


/**
 * Constructor which does nothing because it is used as a static class
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

    return new Task(json.uuid, json.name, json.status, json.creationDate, json.progression, json.message);
};

/**
 * Get all tasks
 */
TaskManager.getAllTasks = function() {
   var d = $.Deferred();

   RestEngine.getJson("/rest/task/")
    .done(function (data) {
       var taskArray=[]
       for(var t of data.task) {
          taskArray.push(TaskManager.factory(t));
       }
       d.resolve(taskArray);
    })
    .fail(d.reject);

    return d.promise();
}

/**
 * Get all tasks matching a name
 * @param {string} name The task type to filter. If param is undefined, all tasks are returned
 */
TaskManager.getTasksByName = function(name) {
   var d = $.Deferred();

   TaskManager.getAllTasks()
   .done(function (taskArray) {
       var filter = _.filter(taskArray, function(t) {
           if(name)
              return t.name === name;
           return true;
       });
       d.resolve(filter);
    })
    .fail(d.reject);

    return d.promise();
}

/**
 * Get all tasks running matching a name
 * @param {string} name The task type to filter. If param is undefined, all tasks are returned
 */
TaskManager.getTasksRunning = function(name) {
   var d = $.Deferred();

   TaskManager.getTasksByName(name)
   .done(function (taskArray) {
       var filter = _.filter(taskArray, function(t) {
          return t.status === "Started";
       });
       d.resolve(filter);
    })
    .fail(d.reject);

    return d.promise();
}