/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Classe permettant de gerer les parametre de type booleeen
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function BoolParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or false
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = parseBool(currentValue);
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null)) {
      this.defaultValue = parseBool(content.defaultValue);
      this.value = this.defaultValue;
   }
   else {
      this.defaultValue = false;
      this.value = this.defaultValue;
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
BoolParameterHandler.prototype.getDOMObject = function () {
   var self = this;

   var input = "<input class=\"enable-validation\" id=\"" + this.uuid + "\" type=\"checkbox\" ";
   if (this.value)
      input +=          "checked ";
   input +=           "/>";

   return ConfigurationHelper.createControlGroup(self, input, true, "checkbox");
};

/**
 * Get the param name
 * @returns {string}
 */
BoolParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {bool}
 */
BoolParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = parseBool($("input#" + this.uuid).prop("checked"));
   return this.value;
};