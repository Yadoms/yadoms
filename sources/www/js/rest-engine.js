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
 * @param {JSON} data The data to add
 * @param {String} dataType The expected return data type
 * @return a promise
*/
RestEngine.get = function (url, data, dataType) {
   return RestEngine.restCall("GET", url, data, undefined, dataType);
};

/**
 * Send a REST PUT asynchronous request
 * @param {string} url The URL to request : "/devices/32"
 * @param {JSON} data The request data
 * @return a promise
 */
RestEngine.put = function (url, data) {
   return RestEngine.restCall("PUT", url, data);
};

/**
 * Send a REST POST asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @return a promise
 */
RestEngine.post = function (url, data) {
   return RestEngine.restCall("POST", url, data);
};

/**
 * Send a REST DELETE asynchronous request
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @return a promise
 */
RestEngine.delete = function (url, data) {
   return RestEngine.restCall("DELETE", url, data);
};

/**
 * Send a REST call asynchronous request
 * @param {string} type of the request  : "GET", "POST", "DELETE", "POST"
 * @param {string} url The URL request : "/devices/32"
 * @param {JSON} data The request data
 * @param {bool} sync Indicate if the query is synchronous
 * @param {String} dataType The expected return data type (is specified use value, if undefined use json, if null don't speciy anythin and let $.ajax guess
 * @return a promise
 */
RestEngine.restCall = function(type, url, data, sync, dataType){
   assert(!isNullOrUndefined(type), "type must be defined");
   assert(!isNullOrUndefined(url), "url must be defined");
   
	var d = $.Deferred();
   
   var ajaxOptions ={
      type : type,
      url: url
   };

   //manage data
   if (!isNullOrUndefined(data)) {
      ajaxOptions.data = data;
   }
   
   //manage synchronous option
	if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
		ajaxOptions.async = !sync;


   //manage dataType option
   //if null => don't define option, let $ajax guess type
   //if undefined => use "json"
   //other => use specified value
   if(dataType != null) {
      ajaxOptions.dataType = "json";
      if (!isNullOrUndefined(dataType))
         ajaxOptions.dataType = dataType;
   }
      
      
   //We send request to server
   $.ajax(ajaxOptions)
      .done(function( data ) {
         if(ajaxOptions.dataType == "json") {
            //if JSON dataType expected, we parse the json answer
            if (data.result) {
               //if answer is ok we return data field
               d.resolve(data.data);
            }
            else {
               d.reject(data.errorMessage);
            }
         } else {
            //it is not json result, just resolve using data (case of simple ajax file download)
            d.resolve(data);
         }
      })
      .fail(function(jqXHR, textStatus, errorThrown) {
         //console.error("Options : " + ajaxOptions);
         //we build an error using HTTP error code
         d.reject(jqXHR.status + errorThrown);
      });
   
   return d.promise();
}


