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
   var getAdditionInfo = ["lastValue", "lastValueDate"];

   if (keywords && keywords.length > 0) {
      keywords = removeDuplicates(keywords);
      
      //extract only keyword id
      var allKeywordId = [];
      $.each(keywords, function (index, keyword) {
         if (keyword) {
            if (keyword.id)
               allKeywordId.push(keyword.id);
            else
               allKeywordId.push(keyword);
         }
      });
      
      getAdditionInfo = getAdditionInfo.concat(additionalInfos);
      getAdditionInfo = removeDuplicates(getAdditionInfo);
      
      RestEngine.putJson("/rest/acquisition/keyword/info", {
               data: JSON.stringify({
                     keywords: allKeywordId,
                     info: getAdditionInfo
               })
         })
         .done(function (data) {
               var result = [];
               $.each(data, function (index, keydata) {
                  console.log(keydata);
                  result.push({date: keydata.lastValueDate, 
                               keywordId: index, 
                               value: keydata.lastValue,
                               unit: keydata.unit,
                               capacity: keydata.capacity});
               });
               d.resolve(result);
         })
         .fail(d.reject);
   } else {
         d.resolve();
   }
   return d.promise();
}