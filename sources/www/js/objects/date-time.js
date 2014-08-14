/**
 * Created by nicolasHILAIRE on 30/04/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function DateTimeFormatter() {}

/**
 * Display date given in parameter
 * @param isoDate date to display
 * @returns {string}
 */
DateTimeFormatter.toString = function (isoDate) {

   var format = systemConfiguration[ConfigurationManager.items.system.dateFormatString].value;
   var timeZone = systemConfiguration[ConfigurationManager.items.system.timezone].value;
   var language = systemConfiguration[ConfigurationManager.items.system.language].value;

   moment.lang(language);

   var d = moment.utc(isoDate, "YYYYMMDD[T]HHmmss");
   assert(d.isValid(), "Date given " + isoDate + " is not valid");
   return d.tz(timeZone).format(format);
};