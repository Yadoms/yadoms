/**
 * Assertion method
 * @param condition condition which must be ok
 * @param message message which will be thrown in exception if condition is not ok
 */
var assert = function (condition, message) {
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
function notify(message, gravity, timeout) {
   if (timeout === undefined)
      timeout = defaultNotyTimeout;
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   return noty({ text: message, timeout: timeout, layout: defaultNotyLayout, type: gravity });
}

/**
 * Notify an Information message to the window using noty library
 * @param message message to display
  * @returns {noty}
 */
function notifyInformation(message) {
   console.info(message);
   return notify(message, 'information', undefined);
}

/**
 * Notify a Warning message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifyWarning(message) {
   console.warn(message);
   return notify(message, 'warning', undefined);
}

/**
 * Notify a Success message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifySuccess(message) {
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
function notifyError(message, errorInfo, timeout) {
   if (errorInfo == undefined)
      console.error(message);
   else
      console.error(message, errorInfo);
   return notify(message, 'error', timeout);
}

/**
 * Parse a string as a boolean
 * @param string to parse
 * @param defaultValue 
 * @returns boolean value 
 */
function parseBool(string, defaultValue) {
   if (isNullOrUndefined(defaultValue))
      defaultValue = false;
   if (string === undefined)
      return defaultValue;
   if (string == null)
      return defaultValue;
   if (typeof (string) == 'boolean')
      return string;
   switch (string.toLowerCase()) {
      case "true": case "yes": case "1": case "on" : return true;
      case "false": case "no": case "0": case "off" : case null: return false;
      default: return defaultValue;
   }
}
/**
 * Display a confirm dialog
 * @param {string} message to display
 * @param {string} gravity of the dialog
 * @param {function pointer} confirmCallback 
 * @param {functionpointer} cancelCallback 
 * @returns {noty} 
 */
function notifyConfirm(message, gravity, confirmCallback, cancelCallback) {
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   assert($.isFunction(confirmCallback), "confirmCallback must be defined");
   assert($.isFunction(cancelCallback), "cancelCallback must be defined");

   return noty({
      text: message,
      layout: defaultNotyLayout,
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
   String.prototype.startsWith = function (str) {
      return this.slice(0, str.length) == str;
   };
}

if (typeof String.prototype.endsWith != 'function') {
   String.prototype.endsWith = function (str) {
      return this.slice(-str.length) == str;
   };
}

/**
 * Take an array as input and provide another array without duplicated elements
 * @param {object} arr  The array to treat
 * @returns {object} The resulting array without duplicates
 */
function removeDuplicates(arr) {
   var newArray = [];
   for (var i = 0, j = arr.length; i < j; i++) {
      if (newArray.indexOf(arr[i]) == -1)
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
   if (url1 != undefined & url1.length > 0 && url2 != undefined && url2.length > 0) {

      //case : no "/" trailing url1 or starting url2
      //then append "/" between url1 and url2
      if (url1[url1.length - 1] != "/" && url2[0] != "/") {
         return url1 + "/" + url2;
      }

      //cas : "/" trailing url1 AND "/" starting url2
      //remove the url1 trailing slash and append url2
      if (url1[url1.length - 1] == "/" && url2[0] == "/") {
         return url1.slice(0, -1) + url2;
      }
   }

   //default case : append the url
   return url1 + url2;
}

var loadedJSLibs = [];

/**
 * Load js library in gz format and return a promise
 * @param {string} url of the library
 */
function asyncLoadJSGzLib(libraryName) {
   assert(libraryName != undefined, "libraryName must be defined");
   var d = new $.Deferred();
   if (!loadedJSLibs[libraryName]){
      //we save the promise for other load requests
      loadedJSLibs[libraryName] = d.promise();
      //we create a new deffered
      RestEngine.getBinaryFiles(libraryName)
      .done(function(data) {
         var script = document.createElement("script");
         script.type = "text/javascript";
         script.innerHTML = data;
         // ASYNC: load in parallel and execute as soon as possible
         script.async = false;
         // DEFER: load in parallel but maintain execution order
         script.defer = false;

         //we insert into head (from HeadJS)
         var head = document.head || document.getElementsByTagName("head")[0];
         head.appendChild(script, head.lastChild);
         d.resolve();
      })
      .fail(function(error) {
         console.error(error);
         d.reject();
      });
      return d.promise();
   } else {
      return loadedJSLibs[libraryName];
   }
}

/**
 * Load js libraries in gz format and return a promise
 * @param {array of string} urls of the libraries
 */
function asyncLoadJSGzLibs(libraryNames) {
   assert(Array.isArray(libraryNames), "librayNames must be an array of string");

   var d = new $.Deferred();
   var arrayOfDeffered = [];

   $.each(libraryNames, function (index, lib) {
      arrayOfDeffered.push(asyncLoadJSGzLib(lib)
      .fail(function(error){
         console.log(error);
      }));
   });

   $.when.apply($,arrayOfDeffered)
   .done(d.resolve)
   .fail(d.reject);
   
   return d.promise();
}

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

      script.onerror = function (event) {
         console.error(event);
         script.onload = script.onreadystatechange = script.onerror = null;
         d.resolve();
      }

      console.debug("loading " + librayName + " ...");

      //we insert into head (from HeadJS)
      var head = document.head || document.getElementsByTagName("head")[0];
      head.insertBefore(script, head.lastChild);

      //we save the promise for other load requests
      loadedJSLibs[librayName] = d.promise();
   } else {
      d.resolve();
   }

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

   $.each(librayNames, function (index, lib) {
      arrayOfDeffered.push(asyncLoadJSLib(lib));
   });

   $.when.apply($,arrayOfDeffered)
   .done(d.resolve)
   .fail(d.reject);

   return d.promise();
}

var loadedCss = [];
/**
 * Load css library in a compressed format and return a promise
 * @param {string} url of the library
 */
function asyncLoadGzCss(cssFile) {
   assert(cssFile != undefined, "cssFile must be defined");

   if (!loadedCss[cssFile]) {
      //we create a new deffered
      var d = new $.Deferred();
      RestEngine.getBinaryFiles(cssFile)
      .done(function(data) {
         var script = document.createElement("style");
         //script.type = "text/css";
         //script.rel = "stylesheet"; //
         script.innerHTML = data;
         // ASYNC: load in parallel and execute as soon as possible
         script.async = false;
         // DEFER: load in parallel but maintain execution order
         script.defer = false;

         //we insert into head (from HeadJS)
         var head = document.head || document.getElementsByTagName("head")[0];
         head.appendChild(script, head.lastChild);

         //we save the promise for other load requests
         loadedCss[cssFile] = d.promise();
         d.resolve();
      })
      .fail(function(error) {
         console.error(error);
         d.reject();
      });
      return d.promise();
   } else {
      return loadedCss[cssFile];
   }
}

/**
 * Load css libraries in gz format and return a promise
 * @param {array of string} urls of the libraries
 */
function asyncLoadManyGzCss(cssNames) {
   assert(Array.isArray(cssNames), "librayNames must be an array of string");

   var d = new $.Deferred();
   var arrayOfDeffered = [];

   $.each(cssNames, function (index, lib) {
      arrayOfDeffered.push(asyncLoadGzCss(lib));
   });

   $.when.apply($,arrayOfDeffered)
   .done(d.resolve)
   .fail(d.reject);
   
   return d.promise();
}

/**
 * Load css library and return a promise
 * @param {string} url of the library
 */
function asyncLoadCss(cssFile) {
   assert(cssFile != undefined, "cssFile must be defined");

   if (!loadedCss[cssFile]) {

      //we create a new deffered
      var d = new $.Deferred();

      var script = document.createElement("link");
      script.rel = "stylesheet";
      script.href = cssFile;
      // ASYNC: load in parallel and execute as soon as possible
      script.async = false;
      // DEFER: load in parallel but maintain execution order
      script.defer = false;

      script.onload = script.onreadystatechange = function (event) {
         //from headJS
         if (event.type === "load" || (/loaded|complete/.test(script.readyState) && (!document.documentMode || document.documentMode < 9))) {
            // release event listeners
            script.onload = script.onreadystatechange = script.onerror = null;
            console.debug(cssFile + " loaded");
            d.resolve();
         }
      };

      script.onerror = function (event) {
         console.error(event);
         script.onload = script.onreadystatechange = script.onerror = null;
         d.resolve();
      }

      //we insert into head (from HeadJS)
      var head = document.head || document.getElementsByTagName("head")[0];
      head.insertBefore(script, head.lastChild);

      //the js has been ran, we save the information to prevent from other reloads
      var promise = d.promise();
      //we save the promise for other load requests
      loadedCss[cssFile] = promise;
      return promise;
   } else {
      return loadedCss[cssFile];
   }
}

/**
 * Load css libraries and return a promise
 * @param {array of string} urls of the libraries
 */
function asyncLoadManyCss(cssNames) {
   assert(Array.isArray(cssNames), "librayNames must be an array of string");

   var d = new $.Deferred();
   var arrayOfDeffered = [];

   $.each(cssNames, function (index, lib) {
      arrayOfDeffered.push(asyncLoadCss(lib));
   });

   $.when.apply($,arrayOfDeffered).done(function () {
      d.resolve();
   });

   return d.promise();
}

/**
 * Returns a reference to the specified CSS rule(s).
 * @param {RuleName} The name of the CSS Rule
 */
function getRule( RuleName ) {

  var rule;
  var findedRule=[];

  var ss = document.styleSheets;

  for (var i = 0; i < ss.length; ++i) {

	  // loop through all the rules!

	  for (var x = 0; x < ss[i].cssRules.length; ++x) {
		  rule = ss[i].cssRules[x];
		  if (rule.name == RuleName && rule.type == CSSRule.KEYFRAMES_RULE) {
			  findedRule.push( rule );
		  }
	  }
  }
  return findedRule;
} 

/**
 * Returns custom sort function to be passed as param/callback to the Array's sort method.
 * @param a The the first parameter
 * @param a The the second parameter
 */
function CustomSortFriendlyName(a, b) {
	
	if (typeof(a.friendlyName) == "undefined")
	{
		console.error("objet have no parameter friendlyName");
		console.error(a);
		return 1;
	}
	
	if (typeof(b.friendlyName) == "undefined")
	{
		console.error("objet have no parameter friendlyName");
		console.error(b);
		return 1;
	}
	
	return (a.friendlyName.toLowerCase() > b.friendlyName.toLowerCase()) ? 1 : -1;
}

/**
 * Returns A sorted array containing all keywords/devices. Each cell would have the friendlyName parameter.
 * @param arguments The existing array of keywords/devices.
 */
function sortListItemsWithFriendlyName( arguments ) {
	var op = Array.prototype.sort.call(arguments, CustomSortFriendlyName);
	return op;
}

/**
 * Returns custom sort function to be passed as param/callback to the Array's sort method.
 * @param a The the first parameter
 * @param a The the second parameter
 */
function CustomSortDisplayName(a, b) {
	
	if (typeof(a.displayName) == "undefined")
	{
		console.error("object have no parameter displayName");
		console.error(a);
		return 1;
	}
	
	if (typeof(b.displayName) == "undefined")
	{
		console.error("object have no parameter displayName");
		console.error(b);
		return 1;
	}
	
	return (a.displayName.toLowerCase() > b.displayName.toLowerCase()) ? 1 : -1;
}

/**
 * Returns A sorted array containing all keywords/devices. Each cell would have the friendlyName parameter.
 * @param arguments The existing array of keywords/devices.
 */
function sortListItemsWithdisplayName( arguments ) {
	var op = Array.prototype.sort.call(arguments, CustomSortDisplayName);
	return op;
}

/**
 * Format a file size to a human readable string
 * @param size The file size in bbytes
 * @return string The file size formatted : 88 kB
 */
function humanFileSize(size) {
  var i = Math.floor( Math.log(size) / Math.log(1024) );
  return ( size / Math.pow(1024, i) ).toFixed(2) * 1 + ' ' + [$.t("filesize-units.b", {defaultValue: 'B'}), 
															  $.t("filesize-units.kb", {defaultValue: 'kB'}), 
															  $.t("filesize-units.mb", {defaultValue: 'MB'}), 
															  $.t("filesize-units.gb", {defaultValue: 'GB'}), 
															  $.t("filesize-units.tb", {defaultValue: 'TB'})][i];
}

/**
 * Obtain the key of the first child which have matching value
 * @param object The container
 * @param value The value to test
 * @return the key of matching value
 */
function getKeyByValue(object, value) {
  return Object.keys(object).find(key => object[key] === value);
}