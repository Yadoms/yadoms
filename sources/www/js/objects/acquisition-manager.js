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
                  return new EmptyAcquisition(json.keywordId);
            if (json && json.keywordId && json.error)
                  return new noKeyword(json.keywordId, json.error);
            return new EmptyAcquisition();
      }

      assert(!isNullOrUndefined(json.date), "json.date must be defined");
      assert(!isNullOrUndefined(json.keywordId), "json.keywordId must be defined");
      assert(!isNullOrUndefined(json.value), "json.value must be defined");

      return new Acquisition(json.date, json.keywordId, json.value);
};

/**
 * Get the last acquisition for a list of keywords
 * @param {Array} keywords The keywords list
 * @return {Promise(lastData)}
 */
AcquisitionManager.getLastAcquisition = function (keywords) {
      var d = new $.Deferred();

      if (keywords && keywords.length > 0) {

            removeDuplicates(keywords);

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

            RestEngine.putJson("/rest/acquisition/keyword/info", {
                        data: JSON.stringify({
                              keywords: allKeywordId,
                              info: ["lastValue", "lastValueDate"]
                        })
                  })
                  .done(function (data) {
                        var result = [];
                        $.each(data, function (index, keydata) {
                              result.push(new Acquisition(index, keydata.lastValue, keydata.lastValueDate));
                        });
                        d.resolve(result);
                  })
                  .fail(d.reject);
      } else {
            d.resolve();
      }
      return d.promise();
}