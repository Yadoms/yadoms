/** @module RecipientField class */


/**
 * Creates an instance of RecipientField
 * @param idRecipient The recipient ID containing the field
 * @param pluginName The plugin which created the field
 * @param fieldName The field name
 * @param value The field value
 * @constructor
 */
function RecipientField(idRecipient, pluginName, fieldName, value) {
   assert(idRecipient !== undefined, "idRecipient of a recipient field must be defined");
   assert(pluginName !== undefined, "pluginName of a recipient field must be defined");
   assert(fieldName !== undefined, "fieldName of a recipient field must be defined");

   this.idRecipient = idRecipient;
   this.pluginName = pluginName;
   this.fieldName = fieldName;
   this.value = value;
}

/**
 * Give a string representation of a recipient field (concatenate plugin name and field name)
 * @return {string}
 */
RecipientField.prototype.toString = function() {
    return this.pluginName + "." + this.fieldName;
};

/**
 * Convert the recipient field to JSON format
 * @return {{idRecipient: *, pluginName: string, fieldName: string, value: *}}
 */
RecipientField.prototype.toJSON = function () {
   return {
      idRecipient : this.idRecipient,
      pluginName: encodeURIComponent(this.pluginName),
      fieldName: encodeURIComponent(this.fieldName),
      value : encodeURIComponent(this.value)
   };
};
