/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 *
 * @constructor
 */
function EventLoggerManager() {
}



/**
 * Obtain last event log item
 * @param {Object} additionnalJsonParameters An object containing $.ajax like options
 * @returns {Promise} 
 */
EventLoggerManager.getLast = function (additionnalJsonParameters) {
   return RestEngine.getJson("/rest/eventLogger/last", additionnalJsonParameters);
};

/**
 * Obtain all event log items occured after eventId
 * @param {String|Integer} eventId  The base event id 
 * @returns {Promise} 
 */
EventLoggerManager.getFrom = function (eventId) {
   return RestEngine.getJson("/rest/eventLogger/from/" + eventId);
};

