/** @module Device class */

/**
 * Creates an instance of Acquisition
 * @constructor
 */
function Acquisition(id, date, keywordId, value) {
   assert(!isNullOrUndefined(id), "id of a Acquisition must be defined");
   assert(!isNullOrUndefined(date), "date of a Acquisition must be defined");
   assert(!isNullOrUndefined(keywordId), "keywordId of a Acquisition must be defined");
   assert(!isNullOrUndefined(value), "value of a Acquisition must be defined");

   this.id = id;
   this.date = DateTimeFormatter.isoDateToDate(date);
   this.keywordId = keywordId;
   this.value = value;
}

Keyword.prototype.toJSON = function () {
   return {
      id: this.id,
      date: DateTimeFormatter.dateToIsoDate(this.date),
      keywordId: this.keywordId,
      value: this.value
   };
};
