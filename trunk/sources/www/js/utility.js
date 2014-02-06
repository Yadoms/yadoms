/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

var assert = function(condition, message) {
   if (!condition)
      throw Error("Assert failed" + (typeof message !== "undefined" ? ": " + message : ""));
};