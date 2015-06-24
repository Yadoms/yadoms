/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AutomationInterpreterManager(){}

/**
 * Create a AutomationInterpreter from json object
 * @param json
 * @returns {AutomationRule}
 */
AutomationInterpreterManager.factory = function(interpreterName) {
   assert(!isNullOrUndefined(interpreterName), "interpreterName must be defined");
   return new AutomationInterpreter(interpreterName);
};

/**
 * Get all interpreters from database
 * @param callback
 * @param sync : true if function must be blocking (synchronous)
 */
AutomationInterpreterManager.getAll = function (callback, sync) {
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/automation/interpreter",
      async: async
   })
   .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "interpreters"}), JSON.stringify(data));
            return;
         }

         var interpreters = [];
         data.data.interpreters.forEach(function(value, index) {
            interpreters.push(AutomationInterpreterManager.factory(value));
         });

         callback(interpreters);
      })
   .fail(function() { notifyError($.t("objects.generic.errorGetting", {objectName : "interpreters"})); });
};
