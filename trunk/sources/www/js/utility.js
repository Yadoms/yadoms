/**
 * Assertion method
 * @param condition condition which must be ok
 * @param message message which will be thrown in exception if condition is not ok
 */
var assert = function(condition, message) {
   if (!condition)
      throw Error("Assert failed" + (typeof message !== "undefined" ? ": " + message : ""));
};

/**
 * Notify a message to the window using noty library
 * @param message message to display
 * @param gravity gravity of the message
 * @returns {noty}
 */
function notify(message, gravity)
{
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   return noty({text: message, timeout:2000,  layout:'bottomRight', type: gravity});
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
   return notify(message, 'alert');
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
function notifyError(message)
{
   console.error(message);
   return notify(message, 'error');
}