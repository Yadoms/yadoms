/** @module Device class */

/**
 * Creates an instance of Acquisition
 * @constructor
 */
function Acquisition(date, keywordId, value) {
   assert(!isNullOrUndefined(date), "date of a Acquisition must be defined");
   assert(!isNullOrUndefined(keywordId), "keywordId of a Acquisition must be defined");
   assert(!isNullOrUndefined(value), "value of a Acquisition must be defined");

   this.date = DateTimeFormatter.isoDateToDate(date);
   this.keywordId = keywordId;
   this.value = value;
}

Acquisition.prototype.toJSON = function () {
   return {
      date: DateTimeFormatter.dateToIsoDate(this.date),
      keywordId: this.keywordId,
      value: this.value
   };
};

/**
 * Creates an empty Acquisition
 * @constructor
 */
function EmptyAcquisition() {
   this.date = "";
   this.keywordId = "";
   this.value = "";
}

EmptyAcquisition.prototype.toJSON = function () {
   return {
      date: this.date,
      keywordId: this.keywordId,
      value: this.value
   };
};
