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
      timeout = 4000;
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   return noty({text: message, timeout:timeout,  layout:'bottomRight', type: gravity});
}

/**
 * Notify an Information message to the window using noty library
 * @param message message to display
  * @returns {noty}
 */
function notifyInformation(message)
{
   console.info(message);
   return notify(message, 'information');
}

/**
 * Notify a Warning message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
   function notifyWarning(message)
{
   console.warn(message);
   return notify(message, 'warning');
}

/**
 * Notify a Success message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifySuccess(message)
{
   console.info(message);
   return notify(message, 'success');
}

/**
 * Notify an Error message to the window using noty library
 * @param message message to display
 * @returns {noty}
 */
function notifyError(message, errorInfo, timeout)
{
   console.error(message, errorInfo);
   return notify(message, 'error', timeout);
}

function parseBool(string, defaultValue) {
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

/**
 * Create an unique identifier following RFC4122
 * https://stackoverflow.com/questions/105034/how-to-create-a-guid-uuid-in-javascript
 * @returns {unique UUID}
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

   var uuid = s.join("");
   return uuid;
}

function isNullOrUndefined(object) {
   return ((object === undefined) || (object == null) || (object == "null"));
}