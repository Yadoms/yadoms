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
   this.cbValue = undefined;
   this.uuid = createUUID();

   var self = this;

   //we look if the section has a checkbox to enable the section
   this.enableWithCheckBox = content.enableWithCheckBox;

   //if the option has been not enabled the default value is false
   if (this.enableWithCheckBox === undefined)
      this.enableWithCheckBox = false;

   if (this.enableWithCheckBox) {
      //we have to add a checkbox
      this.cbUuid = createUUID();
      //we look for the current value of the checkbox
      if ((self.configurationValues !== undefined) && (self.configurationValues != null))
         this.cbValue = self.configurationValues.checkbox;
      else if ((content.enableWithCheckBoxDefaultValue !== undefined) && (content.enableWithCheckBoxDefaultValue != null))
         this.cbValue = content.enableWithCheckBoxDefaultValue;
      else
         this.cbValue = false;

      //we get the behavior to have when the checkbox is unchecked (disabled, hidden, or collapsed)
      if ((content.checkBoxBehavior !== undefined) && (content.checkBoxBehavior != null))
         this.uncheckedBehavior = content.uncheckedBehavior;
      else
         this.uncheckedBehavior = "disabled";
   }

   //for each key in package
   $.each(content.content, function (key, value) {
      var v = undefined;
      if ((self.configurationValues !== undefined) && (self.configurationValues != null) && (self.configurationValues.values != null) && (self.configurationValues.values != undefined))
         v = self.configurationValues.values[key];

      var newI18nContext = i18nContext + self.paramName + ".";
      var handler = ConfigurationHelper.createParameterHandler(objectToConfigure, newI18nContext, key, value, v);
      self.configurationHandlers.push(handler);
   });
}

SectionParameterHandler.prototype.getDOMObject = function () {
   var input = "<div class=\"control-group configuration-section well\" >" +
                  "<div class=\"configuration-header\" >";

   if (this.enableWithCheckBox) {
      input +=       "<div class=\"checkbox\">" +
                        "<label>" +
                           "<input type=\"checkbox\" id=\"" + this.cbUuid + "\"";
      if (this.cbValue)
         input +=                "checked ";
      input +=             ">";
   }
   input +=                "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                              this.name +
                           "</span>";
   if (this.enableWithCheckBox) {
      input +=          "</label>" +
                     "</div>" +
                     "<script>\n" +
                        "$(\"#" + this.cbUuid + "\").change(function () {\n" +
                           "if ($(\"#" + this.cbUuid + "\").prop(\"checked\")) {\n" +
                              "$(\"#" + this.uuid + "\").removeClass(\"hidden\");" +
                           "}\n" +
                           "else {\n" +
                              "$(\"#" + this.uuid + "\").addClass(\"hidden\");" +
                           "}\n" +
                        "});\n" +
                     "</script>";
   }
   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div id=\"" + this.uuid + "\" ";

   //if checkbox is unchecked
   if ((this.enableWithCheckBox) && (!this.cbValue)) {
      input +=       "class=\"hidden\" ";
   }
   input +=       ">";

   //we append each param in the section
   $.each(this.configurationHandlers, function (key, value) {
      input += value.getDOMObject();
      input += "\n";
   });

   input +=       "</div>" +
               "</div>";

   return input;
};

SectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

SectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   self.configurationValues.values = {};
   $.each(self.configurationHandlers, function (key, value) {
      self.configurationValues.values[value.getParamName()] = value.getCurrentConfiguration();
   });

   //we get the checkbox value if used
   if (this.enableWithCheckBox) {
      self.configurationValues.checkbox = parseBool($("input#" + this.cbUuid).prop("checked"));
   }
   debugger;
   return self.configurationValues;
};