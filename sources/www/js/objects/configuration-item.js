/** @module Page class */

//TODO utile ?

/**
 * Creates an instance of Page
 * @param section
 * @param name name of the page
 * @param value
 * @param defaultValue
 * @param description
 * @param securityAccess
 * @param lastModificationDate
 * @constructor
 */
function ConfigurationItem(section, name, value, defaultValue, description, securityAccess, lastModificationDate) {
   assert(!isNullOrUndefined(section), "section must be defined");
   assert(!isNullOrUndefined(name), "name must be defined");
   assert(!isNullOrUndefined(value), "value must be defined");
   assert(!isNullOrUndefined(defaultValue), "defaultValue must be defined");
   assert(!isNullOrUndefined(description), "description must be defined");
   assert(!isNullOrUndefined(securityAccess), "securityAccess must be defined");
   assert(!isNullOrUndefined(lastModificationDate), "lastModificationDate must be defined");

   this.section = section;
   this.name = name;
   this.value = value;
   this.defaultValue = defaultValue;
   this.description = description;
   this.securityAccess = securityAccess;
   this.lastModificationDate = lastModificationDate;
}

ConfigurationItem.prototype.toJSON = function () {
   return {
      section : this.section,
      name: this.name,
      value: ConfigurationItem.encodeValue(this.value),
      defaultValue: ConfigurationItem.encodeValue(this.defaultValue),
      description: this.description,
      securityAccess: this.securityAccess,
      lastModificationDate: this.lastModificationDate
   };
};

ConfigurationItem.prototype.updateValue = function (newValue) {
   this.value = newValue;
};

ConfigurationItem.encodeValue = function(valueToEncode) {
   if ($.isPlainObject(valueToEncode))
   {
      //we iterate on each node of the object
      $.each(valueToEncode, function (key, value) {
         valueToEncode[key] = ConfigurationItem.encodeValue(value);
      });
      return JSON.stringify(valueToEncode);
   }
   else {
      return valueToEncode;
   }
};

ConfigurationItem.decodeValue = function(valueToDecode) {
   var decodedValue;
   //we try to create a json object, if it is not possible, it is a text field
   try {
      decodedValue = JSON.parse(valueToDecode);
      //we decode each value of key - value items in the json tree
      $.each(decodedValue, function (key, value) {
         decodedValue[key] = ConfigurationItem.decodeValue(value);
      });
   }
   catch (e) {
      decodedValue = valueToDecode;
   }

   return decodedValue;
};