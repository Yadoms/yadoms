/**
 * Created by jeanmichelDECORET on 08/10/15.
 */

 
/** Classe qui gère les requetes REST auprès du server

Cette classe ne s'instancie pas et ouvre simplement des méthodes statiques accessible depuis n'importe quel code JS

 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function RestEngine(){}

/**
 * Send a REST GET asynchronous request
 * @param {string} url The URL requested : "/devices/32"
 * @param {JSON} data The $.ajax options
 * @return a promise
*/
RestEngine.get = function (url, data) {
    return RestEngine.restCall_("GET", url, data);
};

/**
 * Send a REST GET asynchronous request
 * @param {string} url The URL requested : "/devices/32"
 * @param {JSON} data The $.ajax options (dataType will be overwritten by 'json')
 * @return a promise
*/
RestEngine.getJson = function (url, data) {
    return RestEngine.get(url, RestEngine.makeJsonOptions_(data));
};

/**
 * Send a REST GET asynchronous request
 * @param {string} url The URL requested : "/devices/32"
 * @param {JSON} data The $.ajax options (dataType will be overwritten by 'script')
 * @return a promise
*/
RestEngine.getScript = function (url, data) {
    var completeData = data || {};
    completeData.dataType = "script";
    return RestEngine.get(url, completeData);
};

/**
 * Send a REST GET asynchronous request
 * @param {string} url The URL requested : "/devices/32"
 * @param {JSON} data The $.ajax options (dataType will be overwritten by 'html')
 * @return a promise
*/
RestEngine.getHtml = function (url, data) {
    var completeData = data || {};
    completeData.dataType = "html";
    return RestEngine.get(url, completeData);
};

/**
 * Send a REST PUT asynchronous request
 * @param {string} url The URL to request : "/devices/32"
 * @param {JSON} data The $.ajax options
 * @return a promise
 */
RestEngine.put = function (url, data) {
   return RestEngine.restCall_("PUT", url, data);
};

/**
 * Send a REST PUT asynchronous request
 * @param {string} url The URL to request : "/devices/32"
 * @param {JSON} data The $.ajax options (dataType will be overwritten by 'json')
 * @return a promise
 */
RestEngine.putJson = function (url, data) {
    return RestEngine.put(url, RestEngine.makeJsonOptions_(data));
};

/**
 * Send a REST POST asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The $.ajax options
 * @return a promise
 */
RestEngine.post = function (url, data) {
   return RestEngine.restCall_("POST", url, data);
};

/**
 * Send a REST POST asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The $.ajax options
 * @return a promise
 */
RestEngine.postJson = function (url, data) {
    console.log ( data );
    return RestEngine.post(url, RestEngine.makeJsonOptions_(data));

};

/**
 * Send a REST DELETE asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @return a promise
 */
RestEngine.delete = function (url, data) {
   return RestEngine.restCall_("DELETE", url, data);
};

/**
 * Send a REST DELETE asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @return a promise
 */
RestEngine.deleteJson = function (url, data) {
    return RestEngine.delete(url, RestEngine.makeJsonOptions_(data));
};


/**
 * Complete or create the $.ajax options for JSON request
 * @param {JSON} data The initial options (can be null/undefined)
 * @return {JSON} The $.ajax options for json data
 * @private
 */
RestEngine.makeJsonOptions_ = function(data) {
    var completeData = data || {};
    completeData.dataType = "json";
    if (!completeData.contentType)
        completeData.contentType = "application/x-www-form-urlencoded;charset=utf-8";
    return completeData;
}

/**
 * Send a REST call asynchronous request
 * @param {string} type of the request  : "GET", "POST", "DELETE", "POST"
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @param {bool} sync Indicate if the query is synchronous
 * @param {String} dataType The expected return data type (is specified use value, if undefined use json, if "auto" don't speciy anything and let $.ajax guess
 * @return a promise
 * @private
 */
RestEngine.restCall_ = function(type, url, data){
    assert(!isNullOrUndefined(type), "type must be defined");
   assert(!isNullOrUndefined(url), "url must be defined");
   
	var d = $.Deferred();

	var ajaxOptions = data || {};
    ajaxOptions.type = type;
	ajaxOptions.url = url;
      
   //We send request to server
   $.ajax(ajaxOptions)
      .done(function (data) {
          if (ajaxOptions.dataType === "json" && !isNullOrUndefined(data) && data.result != undefined) {
            //if JSON dataType expected, we parse the json answer
            if (data.result && data.result === "true") {
               //if answer is ok we return data field
               d.resolve(data.data);
            }
            else {
               d.reject(data.message);
            }
         } else {
            //it is not json result, just resolve using data (case of simple ajax file download)
            d.resolve(data);
         }
      })
      .fail(function(jqXhr, textStatus, errorThrown) {
         //console.error("Options : " + ajaxOptions);
         //we build an error using HTTP error code
         d.reject(jqXhr.status + errorThrown);
      });
   
   return d.promise();
}
