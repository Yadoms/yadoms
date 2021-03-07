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
AutomationInterpreterManager.getAll = function () {
   var d = new $.Deferred();

   RestEngine.getJson("rest/automation/interpreter")
   .done(function(data) {
      var interpreters = [];
      
      for (var position = 0, len = data.interpreters.length; position < len; position++) {
         var interpreter = AutomationInterpreterManager.factory(data.interpreters[position]);
         interpreters[interpreter.type] = interpreter;         
      }
      d.resolve(interpreters);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Get all available interpreters
 * @param callback
 * @param sync : true if function must be blocking (synchronous)
 */
AutomationInterpreterManager.getAvailable = function () {
   var d = new $.Deferred();

   RestEngine.getJson("rest/automation/interpreter/available")
   .done(function(data) {
      var interpreters = [];
      
      for (var position = 0, len = data.interpreters.length; position < len; position++) {
         var interpreter = AutomationInterpreterManager.factory(data.interpreters[position]);
         interpreters[interpreter.type] = interpreter;         
      }
      d.resolve(interpreters);
   })
   .fail(d.reject);

   return d.promise();
};


/**
 * Get all interpreters from database and package.json
 */
AutomationInterpreterManager.getAllDetailed = function () {
   var d = new $.Deferred();

   AutomationInterpreterManager.getAll()
   .done(function (interpreters) {

      var deferredArray =[];

      //for each name we get the package.json file and append it to the associative array
      Object.keys(interpreters).forEach(function (key) {
         //this thread will ask for synchronous package.json requests
         var value = interpreters[key];

         var deferred = RestEngine.get("scriptInterpreters/" + value.type + "/package.json", { dataType: "json" });
         deferredArray.push(deferred);

         deferredArray.push(i18nManager.loadNamespace("scriptInterpreters", value.type));
         
         deferred.done(function (data) {
             value.fillDetails(data);
          })
          .fail(function() {
             notifyError($.t("objects.generic.errorGetting", {objectName : "scriptInterpreters/" + value + "/package.json"}));
          });
      });

      $.when.apply($, deferredArray)
      .done(function() {
         d.resolve(interpreters);
      });
   })
   .fail(d.reject);

   return d.promise();
};

AutomationInterpreterManager.getInterpreterBaseUrl = function (interpreter) {
    assert(!isNullOrUndefined(interpreter), "interpreter must be defined");

    var interpreterPath = AutomationInterpreterManager.factory(interpreter).type;
    return "scriptInterpreters/" + interpreterPath ;
};


