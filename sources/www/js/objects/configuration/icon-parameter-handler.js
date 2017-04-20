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
function IconParameterHandler(i18nContext, i18nKey, aramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or '' (no icon)
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = currentValue;
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null))
      this.value = content.defaultValue;
   else
      this.value = "";

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
IconParameterHandler.prototype.getDOMObject = function () {
   var input = "<div class=\"input-group\" >" +
                  "<button class=\"btn btn-default\" data-iconset=\"glyphicon\" role=\"iconpicker\" " +
                     "id=\"" + this.uuid + "\"" +
                     "data-icon=\"" + this.value + "\" data-rows=\"3\" data-cols=\"6\" data-placement=\"top\" data-search=\"true\" data-search-text=\"" + $.t("configuration.iconpicker-search-prompt", {defaultValue:"Search icon..."}) + "\" >" +
                  "</button>" +
               "</div>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Apply script after DOM object has been added to the page
 */
IconParameterHandler.prototype.applyScript = function () {
   $("button#" + this.uuid).iconpicker();
};

IconParameterHandler.prototype.locateInDOM = function () {
   return $("button#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
IconParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
IconParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation").attr("disabled", false);
    } else {
            $("#" + self.uuid).removeClass("enable-validation").attr("disabled", true);
    }
}

/**
 * Get the current configuration in the form
 * @returns {string}
 */
IconParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("button#" + this.uuid + " > input").val();
   return this.value;
};