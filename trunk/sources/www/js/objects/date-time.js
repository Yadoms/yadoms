/**
 * Created by nicolasHILAIRE on 30/04/14.
 */

/**
 * Create the DateTimeFormatter used to parse ISO Date and to display it
 * @param format at moment.js syntax ie "DD/MM/YYYY HH:mm:ss"
 * @constructor
 */
function DateTimeFormatter(format, timeZone) {

   if (!isNullOrUndefined(DateTimeFormatter))
      this.format = format;
   else
      this.format = "DD/MM/YYYY HH:mm:ss";

   if (!isNullOrUndefined(timeZone))
      this.timeZone = timeZone;
   else
      this.timeZone = "Europe/Paris";
}

/**
 * Display date given in parameter
 * @param isoDate date to display
 * @returns {string}
 */
DateTimeFormatter.prototype.toString = function (isoDate) {
   var d = moment.utc(isoDate, "YYYYMMDD[T]HHmmss");
   assert(d.isValid(), "Date given " + isoDate + " is not valid");
   return d.tz(this.timeZone).format(this.format);
};