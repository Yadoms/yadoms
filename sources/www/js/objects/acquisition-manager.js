/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AcquisitionManager(){}

AcquisitionManager.factory = function(json) {
   if (isNullOrUndefinedOrEmpty(json) || !json.date || !json.value) {
      if(json && json.keywordId && !json.error)
         return new EmptyAcquisition(json.keywordId);
      if(json && json.keywordId && json.error)
         return new noKeyword(json.keywordId, json.error);
      return new EmptyAcquisition();
   }

   assert(!isNullOrUndefined(json.date), "json.date must be defined");
   assert(!isNullOrUndefined(json.keywordId), "json.keywordId must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");

   return new Acquisition(json.date, json.keywordId, json.value);
};

/**
 * Get the last value of the keywordId
 * @param {Integer|String} keywordId The keyword id to request last data
 * @return {Promise(lastData)}
 */
AcquisitionManager.getLastValue = function (keywordId) {
   assert(!isNullOrUndefinedOrEmpty(keywordId), "keywordId must be defined");

   var d = new $.Deferred();

   RestEngine.getJson("/rest/acquisition/keyword/" + keywordId + "/lastdata")
      .done(function(data) {
         d.resolve(AcquisitionManager.factory(data));
      })
      .fail(d.reject);

   return d.promise();
};

/**
 * Get the last values of the keyword list
 * @param {Array} keywords The keyword array
 * @return {Promise(lastData)}
 */
AcquisitionManager.getLastValues = function(keywords) {
   var d = new $.Deferred();

   if (keywords && keywords.length > 0) {

      duplicateRemoval(keywords);
   
      //extract only keyword id
      var allKeywordId = [];
      $.each(keywords, function(index, keyword) {
         if (keyword) {
            if (keyword.id)
               allKeywordId.push(keyword.id);
            else
               allKeywordId.push(keyword);
         }
      });

      RestEngine.putJson("/rest/acquisition/keyword/lastdata", { data: JSON.stringify({ keywords: allKeywordId }) })
         .done(function(data) {
            var result = [];
            $.each(data, function(index, keydata) {
               result.push(AcquisitionManager.factory(keydata));
            });
            d.resolve(result);
         })
         .fail(d.reject);
   } else {
      d.resolve();
   }
   return d.promise();
}