/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a decimal parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function DecimalParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //if value is setted we use it else we use the default value else we use 0
   this.value = parseFloat(currentValue);

   if (isNaN(this.value)) {
      this.value = parseFloat(content.defaultValue);
      if (isNaN(this.value)) {
         console.warn("Unable to parse as int content.defaultValue of " + name + " parameter. Set to 0");
         this.value = 0;
      }
   }

   //we search min and max value
   this.minValue = parseFloat(content.minimumValue);
   this.maxValue = parseFloat(content.maximumValue);
   this.precision = parseInt(content.precision);

   //we round the actual value with max and min
   this.value = Math.max((isNaN(this.minValue)?-Infinity:this.minValue) , Math.min((isNaN(this.maxValue)?Infinity:this.maxValue), this.value));

   //if precision is enable we set it
   if (!isNaN(this.precision))
      this.value = this.value.toFixed(this.precision);

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
DecimalParameterHandler.prototype.getDOMObject = function () {
   //we provide a SpinEdit
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required " +
                        "pattern=\"[-+]?[0-9]*[\.,]?[0-9]*\"";
   var i18nOptions = {};
   var i18nData = " data-i18n=\"";

   i18nData += "[data-content]" + this.i18nContext + this.paramName + ".description";
   i18nData += ";[data-validation-required-message]widgets.configuration.validationForm.incorrectValue";
   i18nData += ";[data-validation-pattern-message]widgets.configuration.validationForm.onlyNumberAllowed";

   if (!isNaN(this.maxValue)) {
      input += "max=\"" + this.maxValue + "\" ";
      i18nData += ";[data-validation-max-message]widgets.configuration.validationForm.maxValueExceeded";
      i18nOptions["maxValue"] = this.maxValue;
   }
   if (!isNaN(this.minValue)) {
      input += "min=\"" + this.minValue + "\" ";
      i18nData += ";[data-validation-min-message]widgets.configuration.validationForm.minValueExceeded";
      i18nOptions["minValue"] = this.minValue;
   }

   i18nData += "\" ";
   input += i18nData;
   input += "data-i18n-options=\'" + JSON.stringify(i18nOptions) + "\'";
   input += " value =\"" + this.value + "\" >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Get the param name
 * @returns {string}
 */
DecimalParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {double}
 */
DecimalParameterHandler.prototype.getCurrentConfiguration = function () {
   //we allow "," and "."
   var v = $("input#" + this.uuid).val().replace(',', '.');
   this.value = parseFloat(v);
   if (!isNaN(this.precision))
      this.value = this.value.toFixed(this.precision);
   return this.value;
};