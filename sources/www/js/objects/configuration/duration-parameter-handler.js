/**
 * Created by Jean-Michel BERHAULT on 11/06/18.
 */

/**
 * Create a duration parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function DurationParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, autocomplete) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or empty value
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = currentValue;
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null))
      this.value = content.defaultValue;
   else
      this.value = "00:00:00";

   //we set the current value of the default value or empty string
   if (!isNullOrUndefinedOrEmpty(autocomplete))
      this.autocomplete = autocomplete;
   else
      this.autocomplete = "00:00:00";

   //we get max length value
   this.maximumLength = parseInt(content.maximumLength);

   //we cut the actual value in the maximumLength allowed if greater
   if (!isNaN(this.maximumLength))
      this.value = this.value.substr(0, this.maximumLength);

   //regex for hour
   hhmmssRegex = "([0-1][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])";
   decimalRegex = "[0-9]+(.[0-9]+)?";
   this.regex = "^(" + decimalRegex + "|" + hhmmssRegex + ")$";

   //we cannot read directly from the sub item set-value-keyword-keyword-configuration, because this element is a numeric, but the display is different
   this.regexErrorMessage = $.t("modals.set-value-keyword.duration-regex-error");

   //we look if the field mustn't be blank
   if (!isNullOrUndefined(content.required))
      this.required = parseBool(content.required);
   else
      this.required = false;

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description) ? "" : content.description;
   this.i18nContext = i18nContext;
   this.i18nKey = i18nKey || paramName;
   this.content = content;
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
DurationParameterHandler.prototype.getDOMObject = function () {
   var input = "<input ";

   input += "type=\"text\" ";

   if (this.autocomplete)
      input += "autocomplete=\"" + this.autocomplete + "\" ";

   input += "class=\"form-control enable-validation\" " +
      "id=\"" + this.uuid + "\" " +
      "name=\"" + this.uuid + "\" " +
      "data-content=\"" + this.description + "\" ";
   if (this.required) {
      input += "required aria-required ";
      //this line MUST be translated before rendering
      input += "data-validation-required-message=\"" + $.t("configuration.validationForm.required") + "\" ";
   }

   var dataI18n = "data-i18n=\"";
   dataI18n += "[data-content]" + this.i18nContext + this.i18nKey + ".description";

   if (!isNaN(this.maximumLength))
      input += "maxlength=\"" + this.maximumLength + "\" ";

   if (!isNullOrUndefined(this.regex)) {
      input += "pattern=\"" + this.regex + "\" data-validation-pattern-message=\"" + this.regexErrorMessage + "\"";
      dataI18n += ";[data-validation-pattern-message]" + this.i18nContext + this.i18nKey + ".regexErrorMessage";
   }

   dataI18n += "\"";

   input += " value =\"";
   input += this.value;
   input += "\" ";
   input += dataI18n + " >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

DurationParameterHandler.prototype.locateInDOM = function () {
   return $("input#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
DurationParameterHandler.prototype.getParamName = function () {
   return this.paramName;
};

/**
 * Enable / Disable the content of the configuration item
 */
DurationParameterHandler.prototype.setEnabled = function (enabled) {
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
DurationParameterHandler.prototype.getCurrentConfiguration = function () {
   var val = $("input#" + this.uuid).val();

   if (val.match(hhmmssRegex)) {
      // Convert to seconds
      val = this.hhmmssToSeconds(val);
   }

   this.value = val;
   var d = new $.Deferred();
   d.resolve(this.value);
   return d.promise();
};

DurationParameterHandler.prototype.secondsToHhmmss = function (val) {
   return moment.utc(moment.duration(val, 'seconds').as('milliseconds')).format('HH:mm:ss');
}

DurationParameterHandler.prototype.hhmmssToSeconds = function (val) {
   return moment.duration(val).asSeconds();
}