/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AcquisitionManager() {}

AcquisitionManager.factory = function (json) {
   if (isNullOrUndefinedOrEmpty(json) || !json.date || !json.value) {
         if (json && json.keywordId && !json.error)
            return {date: "", keywordId: json.keywordId, value: ""};
         if (json && json.keywordId && json.error)
            return {keywordId: json.keywordId, error: json.error};
         return {date: "", keywordId: "", value: ""};
   }

   assert(!isNullOrUndefined(json.date), "json.date must be defined");
   assert(!isNullOrUndefined(json.keywordId), "json.keywordId must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");

   return {date: json.date, keywordId: json.keywordId, value: json.value};
};

/**
 * Get the last acquisition for a list of keywords
 * @param {Array} keywords The keywords list
 * @return {Promise(lastData)}
 */
AcquisitionManager.getLastAcquisition = function (keywords, additionalInfos) {
   var d = new $.Deferred();
   var getAdditionInfo = ["lastValue", "lastValueDate", "exist"];

   //append additional informations
   if(additionalInfos) {
	  getAdditionInfo = getAdditionInfo.concat(additionalInfos);
   }
   return KeywordManager.getInformation(keywords, getAdditionInfo);
}