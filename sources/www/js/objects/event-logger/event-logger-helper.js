/**
 * Created by nicolasHILAIRE on 24/04/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function EventLoggerHelper(){}

/**
 * return the event logger represented in a translated string
 * @param event
 * @returns {string}
 */
EventLoggerHelper.toString = function(event) {
   assert(!isNullOrUndefined(event), "Param must be an EventLogger");

   var translation = "eventLogger." + event.code;

   return DateTimeFormatter.isoDateToString(event.date) + " : " + $.t(translation, {"who" : event.who, "what" : event.what});
};