/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

var assert = function(condition, message) {
   if (!condition)
      throw Error("Assert failed" + (typeof message !== "undefined" ? ": " + message : ""));
};

function notify(message, gravity)
{
   assert(message != undefined, "Message must be defined");
   assert(gravity != undefined, "Gravity must be defined");
   return noty({text: message, timeout:2000,  layout:'topLeft', type: gravity});
}


function notifyInformation(message)
{
   return notify(message, 'information');
}

function notifyWarning(message)
{
   return notify(message, 'alert');
}

function notifySuccess(message)
{
   return notify(message, 'success');
}

function notifyError(message)
{
   return notify(message, 'error');
}