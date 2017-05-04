/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create an int parameter handler
 * @param i18nContext
 * @param paramName
 * @param {{minimumValue:string}, {maximumValue:string}, {defaultValue:string}, {name:string}, {description:string}} content
 * @param currentValue
 * @constructor
 */
function IntParameterHandler(i18nContext, i18nKey, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //if value is set we use it else we use the default value else we use 0
   this.value = parseInt(currentValue);

   if (isNaN(this.value)) {
      this.value = parseInt(content.defaultValue);
      if (isNaN(this.value))
         this.value = 0;
   }

   //we search min and max value
   this.minValue = parseInt(content.minimumValue);
   this.maxValue = parseInt(content.maximumValue);

   //we round the actual value with max and min
   this.value = Math.max((isNaN(this.minValue)?-Infinity:this.minValue) , Math.min((isNaN(this.maxValue)?Infinity:this.maxValue), this.value));

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.i18nKey = i18nKey || paramName;
   this.content = content;
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
IntParameterHandler.prototype.getDOMObject = function () {
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required " +
                        "pattern=\"-?[0-9]+\"";

   var i18nOptions = {};
   var i18nData = " data-i18n=\"";

   i18nData += "[data-content]" + this.i18nContext + this.i18nKey + ".description";
   i18nData += ";[data-validation-required-message]configuration.validationForm.incorrectValue";
   i18nData += ";[data-validation-pattern-message]configuration.validationForm.onlyIntegerNumberAllowed";

   if (!isNaN(this.maxValue)) {
      input += " max=\"" + this.maxValue + "\" ";
      i18nData += ";[data-validation-max-message]configuration.validationForm.maxValueExceeded";
      i18nOptions["maxValue"] = this.maxValue;
   }
   if (!isNaN(this.minValue)) {
      input += " min=\"" + this.minValue + "\" ";
      i18nData += ";[data-validation-min-message]configuration.validationForm.minValueExceeded";
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

IntParameterHandler.prototype.locateInDOM = function () {
   return $("input#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
IntParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
IntParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation");
    } else {
            $("#" + self.uuid).removeClass("enable-validation");
    }
}

/**
 * Get the current configuration in the form
 * @returns {int}
 */
IntParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = parseInt($("input#" + this.uuid).val());
   return this.value;
};