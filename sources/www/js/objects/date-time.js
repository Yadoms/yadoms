/**
 * Created by nicolasHILAIRE on 30/04/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function DateTimeFormatter() {}

DateTimeFormatter.isoDateToDate = function (isoDate) {

   var d = moment.utc(isoDate, "YYYYMMDD[T]HHmmss");
   assert(d.isValid(), "Date given " + isoDate + " is not valid");
   return d;
};

/**
 * Display date given in parameter
 * @param isoDate date to display
 * @returns {string}
 */
DateTimeFormatter.isoDateToString = function (isoDate) {

   var d = DateTimeFormatter.isoDateToDate(isoDate);
   var format = systemConfiguration[ConfigurationManager.items.system.dateFormatString].value;
   var timeZone = systemConfiguration[ConfigurationManager.items.system.timezone].value;
   //var language = systemConfiguration[ConfigurationManager.items.system.language].value;

   return d.tz(timeZone).format(format);
};

DateTimeFormatter.dateToIsoDate = function (date) {
   return moment(date).utc().format("YYYYMMDD[T]HHmmss");
};