/**
 * Created by jmdecoret on 01/04/2015.
 */

/**
 * Create an instance of Widget
 * @param uuid Task unique identifier
 * @param name Name of task
 * @param status Status of task (running,...)
 * @param creationDate creationDate
 * @param progression Current progression of a task
 * @param message Current error message of task
 * @constructor
 */
function Task(uuid, name, status, creationDate, progression, message) {
    assert(uuid !== undefined, "uuid of a task must be defined");
    assert(name !== undefined, "name of a task must be defined");
    assert(status !== undefined, "status of a task must be defined");
    //configuration can be undefined

    this.uuid = uuid;
    this.name = name;
    this.status = status;
    this.creationDate = creationDate;
    this.progression = progression;
    this.message = message;
}

