/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create an enum parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function EnumParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //values can be empty
   this.values = content.values;
   this.value = currentValue;

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
EnumParameterHandler.prototype.getDOMObject = function () {
   //Now values can't be empty
   assert(this.values !== undefined, "values field must be defined");

   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      //we set the default value
      this.value = this.content.defaultValue;
   }

   //if it is still not defined we take the first item in the list
   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      this.value = this.content.values[0];
   }

   var input = "<select " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   var i18nData = " data-i18n=\"";

   var self = this;
   i18nData += "[data-content]" + self.i18nContext + self.paramName + ".description";

   i18nData += "\" ";
   input += i18nData + " >";

   input += "</select>";
   return ConfigurationHelper.createControlGroup(self, input);
};

EnumParameterHandler.prototype.setValues = function (values) {
   this.values = values;
   this.updateValues();
};

EnumParameterHandler.prototype.updateValues = function () {
   var self = this;
   var $select = self.locateInDOM();
   $select.empty();

   //we iterate through the values collection
   $.each(self.values, function (key, value) {
      var line = "<option value=\"" + key + "\" data-i18n=\"" + self.i18nContext + self.paramName + ".values." + key + "\"";
      if (key == self.value)
         line += " selected";
      line += " >" + value + "</option>";
      $select.append(line);
   });
};

EnumParameterHandler.prototype.applyScript = function () {
   this.updateValues();
};

EnumParameterHandler.prototype.locateInDOM = function () {
   return $("select#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
EnumParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {string}
 */
EnumParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("select#" + this.uuid).val();
   return this.value;
};