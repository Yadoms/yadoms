/**
 * Created by Nicolas on 01/03/14.
 */

function IntParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //if value is setted we use it else we use the default value else we use 0
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
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

IntParameterHandler.prototype.getDOMObject = function () {
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control\" " +
                        "id=\"" + this.paramName + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required " +
                        "pattern=\"-?[0-9]+\"";

   var i18nOptions = {};
   var i18nData = " data-i18n=\"";

   i18nData += "[data-content]" + this.i18nContext + this.paramName + ".description";
   i18nData += ";[data-validation-required-message]widgets.configuration.validationForm.incorrectValue";
   i18nData += ";[data-validation-pattern-message]widgets.configuration.validationForm.onlyIntegerNumberAllowed";

   if (!isNaN(this.maxValue)) {
      input += " max=\"" + this.maxValue + "\" ";
      i18nData += ";[data-validation-max-message]widgets.configuration.validationForm.maxValueExceeded";
      i18nOptions["maxValue"] = this.maxValue;
   }
   if (!isNaN(this.minValue)) {
      input += " min=\"" + this.minValue + "\" ";
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

IntParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

IntParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = parseInt($("input#" + this.paramName).val());
   return this.value;
};