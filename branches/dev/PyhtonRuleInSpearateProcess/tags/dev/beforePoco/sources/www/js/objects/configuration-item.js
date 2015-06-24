/** @module Page class */

/**
 * Creates an instance of Page
 * @param id database id of the page
 * @param name name of the page
 * @param pageOrder
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
      value: encodeURIComponent(this.value),
      defaultValue: encodeURIComponent(this.defaultValue),
      description: encodeURIComponent(this.description),
      securityAccess: this.securityAccess,
      lastModificationDate: this.lastModificationDate
   };
};

ConfigurationItem.prototype.updateValue = function (newValue) {
   this.value = newValue;
}
