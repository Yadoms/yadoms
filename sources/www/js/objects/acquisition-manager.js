/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AcquisitionManager(){}

AcquisitionManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.date), "json.date must be defined");
   assert(!isNullOrUndefined(json.keywordId), "json.keywordId must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");

   return new Acquisition(json.date, json.keywordId, json.value);
};

/**
* Return the last value of the keywordId
*/
AcquisitionManager.getLastValue = function (keywordId, callback, sync) {
   assert(!isNullOrUndefined(keywordId), "keywordId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "/rest/acquisition/keyword/" + keywordId  + "/lastdata",
      async: async
   })
   .done(function( data ) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.generic.errorGetting", {objectName : "Acquisition KeywordId = " + keywordId}), JSON.stringify(data));
         return;
      }
	  
      callback(AcquisitionManager.factory(data.data));
   })
   .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "Acquisition KeywordId = " + keywordId}));});   
};