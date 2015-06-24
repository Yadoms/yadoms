/** @module RecipientField class */


/**
 * Creates an instance of RecipientField
 * @param idRecipient The recipient ID containing the field
 * @param pluginType The plugin which created the field
 * @param fieldName The field name
 * @param value The field value
 * @constructor
 */
function RecipientField(idRecipient, pluginType, fieldName, value) {
   assert(idRecipient !== undefined, "idRecipient of a recipient field must be defined");
   assert(pluginType !== undefined, "pluginType of a recipient field must be defined");
   assert(fieldName !== undefined, "fieldName of a recipient field must be defined");

   this.idRecipient = idRecipient;
   this.pluginType = pluginType;
   this.fieldName = fieldName;
   this.value = value;
}

/**
 * Give a string representation of a recipient field (concatenate plugin name and field name)
 * @return {string}
 */
RecipientField.prototype.toString = function() {
    return this.pluginType + "." + this.fieldName;
};

/**
 * Convert the recipient field to JSON format
 * @return {{idRecipient: *, pluginType: string, fieldName: string, value: *}}
 */
RecipientField.prototype.toJSON = function () {
   return {
      idRecipient : this.idRecipient,
      pluginType: encodeURIComponent(this.pluginType),
      fieldName: encodeURIComponent(this.fieldName),
      value : encodeURIComponent(this.value)
   };
};
