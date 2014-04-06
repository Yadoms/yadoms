/**
 * Created by Nicolas on 01/03/14.
 */

function StringParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or empty string
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = currentValue;
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null))
      this.value = content.defaultValue;
   else
      this.value = "";

   //we get max length value
   this.maximumLength = parseInt(content.maximumLength);

   //we cut the actual value in the maximumLength allowed if greater
   if (!isNaN(this.maximumLength))
      this.value =  this.value.substr(0,this.maximumLength);

   //we look for a regex pattern
   if (!isNullOrUndefined(content.regex)) {
      this.regex = content.regex;
      this.regexErrorMessage = content.regexErrorMessage;
   }

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

StringParameterHandler.prototype.getDOMObject = function () {
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   if (!isNaN(this.maximumLength))
      input += "maxlength=\"" + this.maximumLength + "\" ";

   if (!isNullOrUndefined(this.regex)) {
      input += "pattern=\"" + this.regex + "\" data-i18n=\"[data-validation-pattern-message]" + this.i18nContext + this.paramName + ".regexErrorMessage\" " +
               "data-validation-pattern-message=\"" + this.regexErrorMessage + "\"";
   }

   input += " value =\"" + this.value + "\" >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

StringParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

StringParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("input#" + this.uuid).val();
   return this.value;
};