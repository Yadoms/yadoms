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
}

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
   "mobile": ""
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

      //create a new field
      var currentField = {
         fieldName : systemField,
         pluginName : "system",
         name :  $.t("objects.recipient.fields." + systemField + ".name" ),
         description :  $.t("objects.recipient.fields." + systemField + ".description" ),
         regexErrorMessage :  $.t("objects.recipient.fields." + systemField + ".regexErrorMessage" ),
         verificationRegex : Recipient.systemFields[systemField],
         value : ""
      };

      //search the field value in definedFields (search if the recipient is already configured for this field)
      $.each(definedFields, function(index, field) {
         if(field.pluginName.toUpperCase() == "system".toUpperCase() && field.fieldName.toUpperCase() == systemField.toUpperCase()) {
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

         var currentField = {
            name : recipientFieldsFromPlugin[recipientField].name,
            description : recipientFieldsFromPlugin[recipientField].description,
            verificationRegex : recipientFieldsFromPlugin[recipientField].verificationRegex,
            regexErrorMessage : recipientFieldsFromPlugin[recipientField].regexErrorMessage
         }

         //force the field value to empty string
         currentField.value = "";

         //search the field value in definedFields (search if the recipient is already configured for this field)
         $.each(definedFields, function(index, field) {
            if(field.pluginName.toUpperCase() == plugin.name.toUpperCase() && field.fieldName.toUpperCase() == recipientField.toUpperCase()) {
               //the field match an already saved one, just reuse the value
               currentField.value = field.value;
            }
         });

         currentField.fieldName = recipientField;
         currentField.pluginName = plugin.name;

         self.fields.push(currentField);
      }
   })

};

