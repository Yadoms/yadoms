/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a string parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
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
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
StringParameterHandler.prototype.getDOMObject = function () {
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        " ";
   var dataI18n = "data-i18n=\"";
   dataI18n += "[data-content]" + this.i18nContext + this.paramName + ".description";

   if (!isNaN(this.maximumLength))
      input += "maxlength=\"" + this.maximumLength + "\" ";

   if (!isNullOrUndefined(this.regex)) {
      input += "pattern=\"" + this.regex + "\" data-validation-pattern-message=\"" + this.regexErrorMessage + "\"";
      dataI18n += ";[data-validation-pattern-message]" + this.i18nContext + this.paramName + ".regexErrorMessage";
   }

   dataI18n += "\"";

   input += " value =\"" + this.value + "\" ";
   input += dataI18n + " >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Get the param name
 * @returns {string}
 */
StringParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {string}
 */
StringParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("input#" + this.uuid).val();
   return this.value;
};