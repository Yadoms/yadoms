/** @module Recipient class */


/**
 * Creates an instance of Recipient
 * @param id The recipient ID
 * @param firstName the first name
 * @param lastName the last name
 * @param fields all additional fields (email, mobile phone,....)
 * @constructor
 */
function Recipient(id, firstName, lastName, fields) {
   assert(firstName !== undefined, "firstName of a recipient must be defined");
   assert(lastName !== undefined, "lastName of a recipient must be defined");

   this.id = id;
   this.firstName = firstName;
   this.lastName = lastName;
   this.fields = fields;
}

/**
 * Give a string representation of a recipient (concatenate first and last names)
 * @return {string}
 */
Recipient.prototype.toString = function() {
    return this.firstName + " " + this.lastName;
};

/**
 * Convert the recipient to JSON format
 * @return {{id: *, firstName: string, lastName: string, fields: *}}
 */
Recipient.prototype.toJSON = function () {
   return {
      id : this.id,
      firstName: encodeURIComponent(this.firstName),
      lastName: encodeURIComponent(this.lastName),
      fields : this.fields
   };
};


/**
 * Define the systemFields
 * @type {string[]}
 */
Recipient.systemFields = {
   "email": "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+.[a-zA-Z0-9-.]+$",
   "mobile": "\+(9[976]\d|8[987530]\d|6[987]\d|5[90]\d|42\d|3[875]\d|2[98654321]\d|9[8543210]|8[6421]|6[6543210]|5[87654321]|4[987654310]|3[9643210]|2[70]|7|1)\d{1,14}$"
};


/**
 * Merge current recipient data with plugin fields
 * @param allPlugins
 */
Recipient.prototype.mergeFields = function (allPlugins) {
   var self = this;

   var definedFields = [];
   $.each(this.fields, function(index, f){
      definedFields.push(f);
   });

   this.fields = [];

   //add system fields
   for(var systemField in Recipient.systemFields) {
       var currentField = {
           fieldName: systemField,
           pluginName: "system",
           pluginDefaultDisplayName : "system",
           name: $.t("objects.recipient.fields." + systemField + ".name"),
           description: $.t("objects.recipient.fields." + systemField + ".description"),
           regexErrorMessage: $.t("objects.recipient.fields." + systemField + ".regexErrorMessage"),
           verificationRegex: Recipient.systemFields[systemField],
           value: ""
       };

      //search the field value in definedFields (search if the recipient is already configured for this field)
      $.each(definedFields, function(index, field) {
         if(field.pluginName.toLowerCase() == "system" && field.fieldName.toUpperCase() == systemField.toUpperCase()) {
            //the field match an already saved one, just reuse the value
            currentField.value = field.value;
         }
      });

      self.fields.push(currentField);
   }



   //add plugin fields
   $.each(allPlugins, function(index, plugin) {
      //for each recipient fields in the plugin
      var recipientFieldsFromPlugin = plugin.getRecipientFields();
      for(var recipientField in recipientFieldsFromPlugin) {

         //make a copy of the object
         var currentField = $.extend(true, {}, recipientFieldsFromPlugin[recipientField]);

         //force the field value to empty string
         currentField.value = currentField.defaultValue;

         //search the field value in definedFields (search if the recipient is already configured for this field)
         $.each(definedFields, function(index, field) {
            if(field.pluginName.toUpperCase() == plugin.name.toUpperCase() && field.fieldName.toUpperCase() == recipientField.toUpperCase()) {
               //the field match an already saved one, just reuse the value
               currentField.value = field.value;
            }
         });

         currentField.fieldName = recipientField;
         currentField.pluginName = plugin.name;
         currentField.pluginDefaultDisplayName = plugin.package.name;

         self.fields.push(currentField);
      }
   });


};

