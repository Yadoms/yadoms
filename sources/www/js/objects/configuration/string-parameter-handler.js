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
   if (!isNullOrUndefinedOrEmpty(content.regex)) {
      this.regex = content.regex;
      this.regexErrorMessage = content.regexErrorMessage;
   }

   //we look if the field mustn't be blank
   if (!isNullOrUndefined(content.required))
       this.required = parseBool(content.required);
   else
       this.required = false;

   //we look if the text has to be encrypted or not
   if (!isNullOrUndefined(content.encrypted)) {
      this.encrypted = parseBool(content.encrypted);
      //if it is encrypted, the crypting method depend on the decryptable attribute.
      //if it is decryptable we use Xor encryption method, if not we can use md5 method
      if (!isNullOrUndefined(content.decryptable)) {
         this.decryptable = parseBool(content.decryptable);
      }
      else {
         this.decryptable = true;
      }
   }
   else {
      this.encrypted = false;
   }

   //we look if the text has to match to another named input
   if (!isNullOrUndefined(content.mustMatchTo)) {
      this.mustMatchTo = content.mustMatchTo;
   }
   else {
      this.mustMatchTo = null;
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
    var input = "<input ";

    if (this.encrypted)
        input +=        "type=\"password\" ";
    else
        input +=        "type=\"text\" ";

    input +=            "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "name=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\" ";
   if (this.required)
    input +=            "required aria-required ";
   var dataI18n = "data-i18n=\"";
   dataI18n += "[data-content]" + this.i18nContext + this.paramName + ".description";

   if (!isNaN(this.maximumLength))
      input += "maxlength=\"" + this.maximumLength + "\" ";

   if (!isNullOrUndefined(this.regex)) {
      input += "pattern=\"" + this.regex + "\" data-validation-pattern-message=\"" + this.regexErrorMessage + "\"";
      dataI18n += ";[data-validation-pattern-message]" + this.i18nContext + this.paramName + ".regexErrorMessage";
   }

   if (!isNullOrUndefined(this.mustMatchTo)) {
      input += "data-validation-match-match=\"" + this.mustMatchTo + "\"";
      dataI18n += ";[data-validation-match-message]" + this.i18nContext + this.paramName + ".matchToErrorMessage";
   }

   dataI18n += "\"";

   input += " value =\"";
   if ((this.encrypted) && (this.decryptable))
       input += EncryptionManager.decryptBase64(this.value, EncryptionManager.key);
   else
       input += this.value;
   input +=  "\" ";
   input += dataI18n + " >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

StringParameterHandler.prototype.locateInDOM = function () {
   return $("input#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
StringParameterHandler.prototype.getParamName = function() {
   return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
StringParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation");
    } else {
            $("#" + self.uuid).removeClass("enable-validation");
    }
}

/**
 * Get the current configuration in the form
 * @returns {string}
 */
StringParameterHandler.prototype.getCurrentConfiguration = function () {
   var val = $("input#" + this.uuid).val();
   if (this.encrypted) {
      if (this.decryptable) {
         this.value = EncryptionManager.encryptBase64(val, EncryptionManager.key);
      }
      else {
         this.value = $.md5(val);
      }
   }
   else
        this.value = val;
   return this.value;
};