/**
 * Created by nicolasHILAIRE on 30/04/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function DateTimeFormatter() {}

DateTimeFormatter.isoDateToDate = function (isoDate) {

   var d = moment(isoDate, "YYYYMMDD[T]HHmmss");
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
   var format = ConfigurationManager.SystemConfiguration()[ConfigurationManager.items.system.dateFormatString];

   return d.format(format);
};

/**
 * Display date given in parameter
 * @returns {string}
 * @param date
 * @param optionalFormat
 */
DateTimeFormatter.dateToString = function (date, optionalFormat) {

   var d = moment(date);
   assert(d.isValid(), "Date given " + date + " is not valid");
   var format = optionalFormat;
   if (!format)
      format = ConfigurationManager.SystemConfiguration()[ConfigurationManager.items.system.dateFormatString];

   return d.format(format);
};

DateTimeFormatter.dateToIsoDate = function (date) {
   return moment(date).format("YYYYMMDD[T]HHmmss");
};