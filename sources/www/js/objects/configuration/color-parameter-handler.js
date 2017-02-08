/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a color parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function ColorParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or #000000
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = currentValue;
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null))
      this.value = content.defaultValue;
   else
      this.value = "#000000";

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
ColorParameterHandler.prototype.getDOMObject = function () {
   var input = "<div class=\"input-group\" id=\"" + this.uuid + "\" >" +
                  "<input type=\"text\"" +
                     "class=\"form-control enable-validation\" " +
                     "data-content=\"" + this.description + "\" " +
                     "value =\"" + this.value + "\" " +
                     "data-i18n=\"[data-content]" + this.i18nContext + this.paramName + ".description" + "\" />" +
                  "<span class=\"input-group-addon\"><i></i></span>" +
               "</div>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Apply script after DOM object has been added to the page
 */
ColorParameterHandler.prototype.applyScript = function () {
   $("div#" + this.uuid).colorpicker();
};

/**
 * Locate parameter in dom
 * @returns {*|jQuery|HTMLElement}
 */
ColorParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
ColorParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
ColorParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;
    if (enabled) {
            $("#" + self.uuid + " > input").addClass("enable-validation");
            $("div#" + this.uuid).colorpicker('enable');
    } else {
            $("#" + self.uuid + " > input").removeClass("enable-validation");
            $("div#" + this.uuid).colorpicker('disable');
    }
}

/**
 * Get the current configuration in the form
 * @returns {string}
 */
ColorParameterHandler.prototype.getCurrentConfiguration = function () {
   var d = new $.Deferred();
   
   this.value = $("div#" + this.uuid + " > input").val();
   d.resolve(this.value);
   return d.promise();
};