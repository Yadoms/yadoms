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
   assert(!isNullOrUndefined(json.blacklist), "json.blacklist must be defined");
   assert(!isNullOrUndefined(json.measure), "json.measure must be defined");

   return new Keyword(json.id, json.deviceId, json.capacityName, json.accessMode, json.name, json.friendlyName, json.type, json.units, json.details, json.typeInfo, json.blacklist, json.measure);
};

KeywordManager.updateToServer = function(keyword) {
   assert(!isNullOrUndefined(keyword), "keyword must be defined");

   var d = new $.Deferred();

   RestEngine.putJson("/rest/device/keyword/" + keyword.id, { data: JSON.stringify(keyword) })
   .done(function(data) {
      keyword.friendlyName = data.friendlyName;
      d.resolve();
   })
   .fail(d.reject);

   return d.promise();
};


KeywordManager.updateBlacklistStateToServer = function(keyword) {
   assert(!isNullOrUndefined(keyword), "keyword must be defined");

   var d = new $.Deferred();

   RestEngine.putJson("/rest/device/keyword/" + keyword.id + "/blacklist", { data: JSON.stringify(keyword) })
   .done(function(data) {
      keyword.blacklist = data.blacklist;
      d.resolve();
   })
   .fail(d.reject);

   return d.promise();
};


KeywordManager.get = function (keywordId) {
   assert(!isNullOrUndefined(keywordId), "keywordId must be defined");

   var d = new $.Deferred();

   RestEngine.getJson("rest/device/keyword/" + keywordId)
   .done(function (data) {
      d.resolve(KeywordManager.factory(data));
   })
   .fail(d.reject);

   return d.promise();
};

KeywordManager.getAll = function () {
   var d = new $.Deferred();

   RestEngine.getJson("/rest/device/keyword")
   .done(function (data) {
      var devices = [];
      $.each(data.keywords, function (index, value) {
         devices.push(KeywordManager.factory(value));
      });
      d.resolve(devices);
   })
   .fail(d.reject);

   return d.promise();
};



/**
 * Get the last value of a keywordId
 * @param {Integer|String} keywordId The keyword id to request last value
 * @return {Promise(lastData)}
 */
KeywordManager.getLastValue = function (keywordId) {
   assert(!isNullOrUndefinedOrEmpty(keywordId), "keywordId must be defined");

   var d = new $.Deferred();

   RestEngine.getJson("/rest/acquisition/keyword/" + keywordId + "/lastdata")
         .done(function (data) {
               d.resolve(data.lastValue);
         })
         .fail(d.reject);

   return d.promise();
};

/**
 * Allow to send a command
 */
KeywordManager.sendCommand = function (keywordId, data) {
   assert(!isNullOrUndefined(keywordId), "keywordId must be defined");
   return RestEngine.postJson("/rest/device/keyword/" + keywordId + "/command", { data: data });
};