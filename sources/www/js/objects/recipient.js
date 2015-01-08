/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function Recipient(id, firstName, lastName, fields) {
   assert(id !== undefined, "id of a Recipient must be defined");
   assert(firstName !== undefined, "firstName of a recipient must be defined");
   assert(lastName !== undefined, "lastName of a recipient must be defined");

   this.id = id;
   this.firstName = firstName;
   this.lastName = lastName;
   this.fields = fields;
}

Recipient.prototype.toJSON = function () {
   return {
      id : this.firstName,
      firstName: encodeURIComponent(this.firstName),
      lastName: encodeURIComponent(this.lastName)
   };
};
