/** @module Page class */

/**
 * Creates an instance of Page
 * @param id database id of the page
 * @param name name of the page
 * @param pageOrder
 * @constructor
 */
function SystemConfigurationItem(section, name, value, default_value, description, securityAccess, last_modification_date) {
   assert(!isNullOrUndefined(section), "section must be defined");
   assert(!isNullOrUndefined(name), "name must be defined");
   assert(!isNullOrUndefined(value), "value must be defined");
   assert(!isNullOrUndefined(default_value), "default_value must be defined");
   assert(!isNullOrUndefined(description), "description must be defined");
   assert(!isNullOrUndefined(securityAccess), "securityAccess must be defined");
   assert(!isNullOrUndefined(last_modification_date), "last_modification_date must be defined");

   this.section = section;
   this.name = name;
   this.value = value;
   this.default_value = default_value;
   this.description = description;
   this.securityAccess = securityAccess;
   this.last_modification_date = last_modification_date;
}

SystemConfigurationItem.prototype.toJSON = function () {
   return {
      section : this.section,
      name: this.name,
      value: encodeURIComponent(this.value),
      default_value: encodeURIComponent(this.default_value),
      description: encodeURIComponent(this.description),
      securityAccess: this.securityAccess,
      last_modification_date: this.last_modification_date
   };
};

SystemConfigurationItem.prototype.updateValue = function (newValue) {
   this.value = newValue;
   //this.last_modification_date=
}
