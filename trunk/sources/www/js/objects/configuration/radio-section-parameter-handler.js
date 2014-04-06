/**
 * Created by Nicolas on 01/03/14.
 */

function RadioSectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   this.objectToConfigure = objectToConfigure;
   this.configurationHandlers = new Array();
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
   this.uuid = createUUID();
   this.radioGroupUuid = createUUID();
   var self = this;

   //for each key in package
   $.each(content.content, function (key, value) {
      var v = undefined;
      if ((self.configurationValues !== undefined) && (self.configurationValues != null) && (self.configurationValues.values != null) && (self.configurationValues.values != undefined))
         v = self.configurationValues.values[key];

      var newI18nContext = i18nContext + self.paramName + ".";

      //all items in content must be section values
      assert((value.type !== undefined), "Content section of the configuration " + self.name + " must be defined");
      assert((value.type.toLowerCase() == "section"), "Content section of the configuration " + self.name + " must contain only section items");
      var radioActive = false;
      if ((self.configurationValues !== undefined) && (self.configurationValues.activeSection == key))
         radioActive = true;
      var handler = new SectionParameterHandler(self.objectToConfigure, newI18nContext, key, value, v, self.radioGroupUuid, radioActive);
      self.configurationHandlers.push(handler);
   });
}

RadioSectionParameterHandler.prototype.getDOMObject = function () {
   var self = this;

   var input = "<div class=\"control-group configuration-radio-section well\" >" +
                  "<div class=\"configuration-header\" >" +
                     "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                        this.name +
                     "</span>" +
                  "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div class=\"toggle-btn-grp\" id=\"" + this.uuid + "\" >";

   //for each param in the section we create a radio button and a div that contain the item
   $.each(this.configurationHandlers, function (key, value) {
      input +=             value.getDOMObject();
   });

   input +=       "</div>" +
               "</div>";

   return input;
};

RadioSectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

RadioSectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   self.configurationValues.values = {};
   $.each(self.configurationHandlers, function (key, value) {
      var currentConfiguration = value.getCurrentConfiguration();
      self.configurationValues.values[value.getParamName()] = currentConfiguration;
      if (currentConfiguration.radio) {
         //it's the active section
         self.configurationValues.activeSection = value.getParamName();
      }
   });

   return self.configurationValues;
};