/**
 * Created by nicolasHILAIRE on 24/04/14.
 */

function EventLoggerHelper(){}

EventLoggerHelper.toString = function(event) {
   assert(!isNullOrUndefined(event), "Param must be an EventLogger");

   var translation;
   switch (event.code) {

      //INFO EVENTS
      default:
      case "0" :
         translation = "eventLogger.default";
         break;
      case "1" :
         translation = "eventLogger.started";
         break;
      case "2" :
         translation = "eventLogger.stopped";
         break;
      case "3" :
         translation = "eventLogger.updated";
         break;

      //ERRORS EVENTS
      case "-1" :
         translation = "eventLogger.yadomsCrashed";
         break;
      case "-2" :
         translation = "eventLogger.pluginCrashed";
         break;
      case "-3" :
         translation = "eventLogger.taskFailed";
         break;
   }
   return event.date + " : " + $.t(translation, {"who" : event.who, "what" : event.what});
}