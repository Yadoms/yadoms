/** @module EventLogger class */

/**
 * Creates an instance of EventLogger
 * @param id database id of the page
 * @param date date when the event occurred
 * @param code code of the event
 * @param who who fire this event
 * @param what parameter of the event
 * @constructor
 */
function EventLogger(id, date, code, who, what) {
   assert(id !== undefined, "id of an event-logger must be defined");
   assert(date !== undefined, "date of an event-logger must be defined");
   assert(code !== undefined, "code of an event-logger must be defined");

   this.id = id;
   this.date = date;
   this.code = code;
   this.who = who;
   this.what = what;
}
