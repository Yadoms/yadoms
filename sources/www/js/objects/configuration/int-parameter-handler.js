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
      if (isNaN(this.value)) {
         console.warn("Unable to parse as int content.defaultValue of " + name + " parameter. Set to 0");
         this.value = 0;
      }
   }

   //we search min and max value
   this.minValue = parseInt(content.minimumValue);
   this.maxValue = parseInt(content.maximumValue);

   this.name = content.name;
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

IntParameterHandler.prototype.getDOMObject = function () {
   //we provide a SpinEdit
   var input = "<input " +
                        "type=\"number\" " +
                        "class=\"form-control\" " +
                        "id=\"" + this.paramName + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required " +
                        "pattern=\"-?[0-9]+\"";
   var i18nOptions = " i18n-options=\"";
   var i18nData = " data-i18n=\"";

   i18nData += "[data-content]" + this.i18nContext + this.paramName + ".description";
   i18nData += ";[data-validation-required-message]widgets.configuration.validationForm.incorrectValue";
   i18nData += ";[data-validation-pattern-message]widgets.configuration.validationForm.onlyIntegerNumberAllowed";

   if (!isNaN(this.maxValue)) {
      input += "max=\"" + this.maxValue + "\" ";
      i18nData += ";[data-validation-max-message]widgets.configuration.validationForm.maxValueExceeded";
   }
   if (!isNaN(this.minValue)) {
      input += "min=\"" + this.minValue + "\" ";
      i18nData += ";[data-validation-min-message]widgets.configuration.validationForm.minValueExceeded";
   }

   i18nOptions += "\" ";
   i18nData += "\" ";
   input += i18nData + i18nOptions;
   input += "value =\"" + this.value + "\" >";
   input += "</input><p class=\"help-block\"></p>";

   var self = this;
   return ConfigurationHelper.createFormGroup(self, input);
};

IntParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

IntParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("input#" + this.paramName).val();
   return this.value;
};