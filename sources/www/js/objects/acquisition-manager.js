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
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.date), "json.date must be defined");
   assert(!isNullOrUndefined(json.keywordId), "json.keywordId must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");

   return new Acquisition(json.id, json.date, json.keywordId, json.value);
};
