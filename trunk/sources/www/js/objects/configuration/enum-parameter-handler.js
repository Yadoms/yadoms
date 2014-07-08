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
   assert(content.values !== undefined, "values field must be defined");
   assert(Object.keys(content.values).length >= 2, "values field must have at least two values");

   this.values = content.values;
   this.value = currentValue;

   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      //we set the default value
      this.value = content.defaultValue;
   }

   //if it is still not defined we take the first item in the list
   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      this.value = content.values[0];
   }

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

   //we iterate through the values collection
   $.each(self.values, function (key, value) {
      input += "<option value=\"" + key + "\" data-i18n=\"" + self.i18nContext + self.paramName + ".values." + key + "\"";
      if (key == self.value)
         input += " selected";
      input += " >" + value + "</option>\n";
   });

   input += "</select>";
   return ConfigurationHelper.createControlGroup(self, input);
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