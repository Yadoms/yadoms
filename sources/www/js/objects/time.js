/** @module Device class */

/**
 * Creates an instance of Time
 * @constructor
 */
function Time(time) {
   assert(!isNullOrUndefined(time), "value of a Time must be defined");

   this.time = DateTimeFormatter.isoDateToDate(time);
}

Time.prototype.toJSON = function () {
   return {
      time: DateTimeFormatter.dateToIsoDate(this.time)
   };
};

/**
 * Creates an empty Time
 * @constructor
 */
function EmptyTime() {
   this.time = "";
}

EmptyTime.prototype.toJSON = function () {
   return {
      time: this.time
   };
};
