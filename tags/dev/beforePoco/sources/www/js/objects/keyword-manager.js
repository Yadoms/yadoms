/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function KeywordManager(){}

KeywordManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.deviceId), "json.deviceId must be defined");
   assert(!isNullOrUndefined(json.capacityName), "json.capacityName must be defined");
   assert(!isNullOrUndefined(json.accessMode), "json.capacityName must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.friendlyName), "json.friendlyName must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");
   assert(!isNullOrUndefined(json.units), "json.units must be defined");

   return new Keyword(json.id, json.deviceId, json.capacityName, json.accessMode, decodeURIComponent(json.name), decodeURIComponent(json.friendlyName), json.type, json.units, json.details);
};

KeywordManager.updateToServer = function(keyword, callback) {
   assert(!isNullOrUndefined(keyword), "keyword must be defined");
   $.ajax({
      type: "PUT",
      url: "/rest/device/keyword/" + keyword.id,
      data: JSON.stringify(keyword),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorUpdating", {objectName : keyword.friendlyName}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }
         //it's okay

         keyword.friendlyName = decodeURIComponent(data.data.friendlyName);

         //we call the callback with true as a ok result
         if ($.isFunction(callback))
            callback(true);
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorUpdating", {objectName : keyword.friendlyName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};