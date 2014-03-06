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
                        "id=\"" + this.name + "\" " +
                        "data-i18n=\"[data-content]" + this.i18nContext + this.name + ".description\" " +
                        "data-content=\"" + this. + "\"" +
                        "required data-validation-required-message=\"Please fill out this field\" " +
                        "pattern=\"-?[0-9]+\" data-validation-pattern-message=\"Seulement un nombre entier\" ";

   if (!isNaN(this.maxValue)) {
      input += "max=\"" + this.maxValue + "\" ";
      input += "data-validation-max-message=\"" + "Valeur maximum depassée" + "\""
   }
   if (!isNaN(this.minValue)) {
      input += "min=\"" + this.minValue + "\" ";
      input += "data-validation-min-message=\"" + "Valeur mini depassée" + "\""
   }

   input += " ><p class=\"help-block\"></p>";

   var self = this;

   return ConfigurationHelper.createFormGroup(self, input);
}