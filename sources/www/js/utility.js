/**
 * Assertion method
 * @param condition condition which must be ok
 * @param message message which will be thrown in exception if condition is not ok
 */
var assert = function(condition, message) {
   if (!condition)
      throw Error("Assert failed" + (typeof message !== "undefined" ? ": " + message : ""));
   return true;
};

var defaultNotyTimeout = 4000;
var defaultNotyLayout = "bottomRight";

/**
 * Notify a message to the window using noty library
 * @param message message to display
 * @param gravity gravity of the message
 * @returns {noty}
 * @param timeout timeout of the noty
 */
function notify(message, gravity, timeout)
{
   if (timeout === undefined)
      timeout = defaultNotyTimeout;
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   return noty({text: message, timeout:timeout,  layout:defaultNotyLayout, type: gravity});
}

/**
 * Notify an Information message to the window using noty library
 * @param message message to display
  * @returns {noty}
 */
function notifyInformation(message)
{
   console.info(message);
   return notify(message, 'information', undefined);
}

/**
 * Notify a Warning message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifyWarning(message)
{
   console.warn(message);
   return notify(message, 'warning', undefined);
}

/**
 * Notify a Success message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifySuccess(message)
{
   console.info(message);
   return notify(message, 'success', undefined);
}

/**
 * Notify an Error message to the window using noty library
 * @param message
 * @returns {noty}
 */
function notifyError(message) {
   return notifyError(message, undefined);
}


/**
 * Notify an Error message to the window using noty library
 * @param message
 * @param errorInfo
 */
function notifyError(message, errorInfo) {
   return notifyError(message, errorInfo, false);
}

/**
 * Notify an Error message to the window using noty library
 * @param message message to display
 * @param errorInfo
 * @param timeout
 * @returns {noty}
 */
function notifyError(message, errorInfo, timeout)
{
   console.error(message, errorInfo);
   return notify(message, 'error', timeout);
}

function parseBool(string, defaultValue) {
   if (isNullOrUndefined(defaultValue))
      defaultValue = false;
   if (string === undefined)
      return defaultValue;
   if (string == null)
      return defaultValue;
   if (typeof(string)=='boolean')
      return string;
   switch(string.toLowerCase()){
      case "true": case "yes": case "1": return true;
      case "false": case "no": case "0": case null: return false;
      default: return defaultValue;
   }
}

function notifyConfirm(message, gravity, confirmCallback, cancelCallback) {
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   assert($.isFunction(confirmCallback), "confirmCallback must be defined");
   assert($.isFunction(cancelCallback), "cancelCallback must be defined");

   return noty({
      text: message,
      layout:defaultNotyLayout,
      gravity: gravity,
      buttons: [
         {
            addClass: 'btn btn-primary', text: $.t("common.yes"), onClick: confirmCallback
         },
         {
            addClass: 'btn btn-danger', text: $.t("common.no"), onClick: cancelCallback
         }
      ]
   });
}

/**
 * Create an unique identifier following RFC4122
 * https://stackoverflow.com/questions/105034/how-to-create-a-guid-uuid-in-javascript
 * @returns {string}
 */
function createUUID() {
   // http://www.ietf.org/rfc/rfc4122.txt
   var s = [];
   var hexDigits = "0123456789abcdef";
   for (var i = 0; i < 36; i++) {
      s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
   }
   s[14] = "4";  // bits 12-15 of the time_hi_and_version field to 0010
   s[19] = hexDigits.substr((s[19] & 0x3) | 0x8, 1);  // bits 6-7 of the clock_seq_hi_and_reserved to 01
   s[8] = s[13] = s[18] = s[23] = "-";

   return s.join("");
}

/**
 * Check if the object is null or undefined
 * @param object
 * @returns {boolean}
 */
function isNullOrUndefined(object) {
   return ((object === undefined) || (object == null) || (object == "null"));
}

/**
 * Check if the object is null or undefined
 * @param object
 * @returns {boolean}
 */
function isNullOrUndefinedOrEmpty(object) {
   return (isNullOrUndefined(object) || (object == ""));
}

//We add extension method to String objects

if (typeof String.prototype.startsWith != 'function') {
   String.prototype.startsWith = function (str){
      return this.slice(0, str.length) == str;
   };
}

if (typeof String.prototype.endsWith != 'function') {
   String.prototype.endsWith = function (str){
      return this.slice(-str.length) == str;
   };
}

Array.prototype.insert = function(index) {
   this.splice.apply(this, [index, 0].concat(
      Array.prototype.slice.call(arguments, 1)));
   return this;
};

/**
 * Take an array as input and provide another array without duplicated elements
 * @param {object} arr  The array to treat
 * @returns {object} The resulting array without duplicates
 */
function duplicateRemoval(arr){
    var newArray = [];
    for(var i=0, j=arr.length; i<j; i++){
        if(newArray.indexOf(arr[i]) == -1)
              newArray.push(arr[i]);  
    }
    return newArray;
}


/**
 * Concatenate to url parts
 * @param {string} url1 The first part
 * @param {string} url2 The second part
 * @return {string} The concatenation of the two parts with managing "/"
 * @private
 */
function concatenateUrl(url1, url2) {
   if(url1 != undefined & url1.length>0 && url2 != undefined && url2.length>0) {

      //case : no "/" trailing url1 or starting url2
      //then append "/" between url1 and url2
      if(url1[url1.length - 1] != "/" && url2[0] != "/") {
         return url1 + "/" + url2;
      } 
      
      //cas : "/" trailing url1 AND "/" starting url2
      //remove the url1 trailing slash and append url2
      if(url1[url1.length - 1] == "/" && url2[0] == "/") {
         return url1.slice(0, -1) + url2;
      } 
   }
   
   //default case : append the url
   return url1 + url2;
}

var loadedJSLibs = [];

/**
 * Load js library and return a promise
 * @param {string} url of the library
 */
function asyncLoadJSLib(librayName) {
    assert(librayName != undefined, "librayName must be defined");

    var d = new $.Deferred();

    if (!loadedJSLibs[librayName]) {
        var script = document.createElement("script");
        script.type = "text/javascript";
        script.src = librayName;
        // ASYNC: load in parallel and execute as soon as possible
        script.async = false;
        // DEFER: load in parallel but maintain execution order
        script.defer = false;

        script.onload = script.onreadystatechange = function (event) {
            //from headJS
            if (event.type === "load" || (/loaded|complete/.test(script.readyState) && (!document.documentMode || document.documentMode < 9))) {
                // release event listeners
                script.onload = script.onreadystatechange = script.onerror = null;
                console.debug(librayName + " loaded");
                d.resolve();
            }
        };

        script.onerror = function(event) {
            console.error(event);
            script.onload = script.onreadystatechange = script.onerror = null;
            d.resolve();
        }

        console.debug("loading " + librayName + " ...");

        //we insert into head (from HeadJS)
        var head = document.head || document.getElementsByTagName("head")[0];
        head.insertBefore(script, head.lastChild);

        //the js has been ran, we save the information to prevent from other reloads
        loadedJSLibs[librayName] = true;
    }
    else
        d.resolve();

    return d.promise();
}

/**
 * Load js libraries and return a promise
 * @param {array of string} urls of the libraries
 */
function asyncLoadJSLibs(librayNames) {
    assert(Array.isArray(librayNames), "librayNames must be an array of string");

    var d = new $.Deferred();
    var arrayOfDeffered = [];

    $.each(librayNames, function(index, lib) {
        if (!loadedJSLibs[lib]) {
            arrayOfDeffered.push(asyncLoadJSLib(lib));
        }
    });
    $.whenAll(arrayOfDeffered).done(function () {
        d.resolve();
    });

    return d.promise();
}
