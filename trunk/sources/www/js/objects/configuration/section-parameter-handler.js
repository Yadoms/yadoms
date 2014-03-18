/**
 * Created by Nicolas on 01/03/14.
 */

function SectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   this.configurationHandlers = new Array();
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;

   var self = this;
   //for each key in package
   $.each(content.content, function (key, value) {
      var v = undefined;
      if ((self.configurationValues !== undefined) && (self.configurationValues != null))
         v = self.configurationValues[key];

      var newI18nContext = i18nContext + self.paramName + ".";
      var handler = ConfigurationHelper.createParameterHandler(objectToConfigure, newI18nContext, key, value, v);
      self.configurationHandlers.push(handler);
   });
}

SectionParameterHandler.prototype.getDOMObject = function () {
   var input = "<div class=\"control-group configuration-section well\">" +
                  "<div class=\"configuration-header\" data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                     this.name +
                  "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div>";

   //we append each param in the section
   $.each(this.configurationHandlers, function (key, value) {
      input += value.getDOMObject();
      input += "\n";
   });
                  "</div>" +
               "</div>"

   return input;
};

SectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

SectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   $.each(self.configurationHandlers, function (key, value) {
      self.configurationValues[value.getParamName()] = value.getCurrentConfiguration();
   });
   return self.configurationValues;
};