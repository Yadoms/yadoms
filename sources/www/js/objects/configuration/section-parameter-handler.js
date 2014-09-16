/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a section parameter handler
 * @param objectToConfigure
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @param radioButtonSectionName
 * @param radioSectionActive
 * @constructor
 */
function SectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue, radioButtonSectionName, radioSectionActive) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   this.configurationHandlers = [];
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
   this.cbValue = undefined;
   this.uuid = createUUID();
   this.selectorUuid = createUUID();

   //we look for radio button
   if (radioButtonSectionName !== undefined) {
      this.radioButtonSectionName = radioButtonSectionName;
      this.radioSectionActive = false;
      if (radioSectionActive !== undefined)
         this.radioSectionActive = parseBool(radioSectionActive);
   }

   var self = this;

   //we look if the section has a checkbox to enable the section
   this.enableWithCheckBox = parseBool(content.enableWithCheckBox);

   //if the option has been not enabled the default value is false
   if (this.enableWithCheckBox === undefined)
      this.enableWithCheckBox = false;

   if (this.enableWithCheckBox) {
      //we have to add a checkbox
      //we look for the current value of the checkbox
      if ((self.configurationValues !== undefined) && (self.configurationValues != null))
         this.cbValue = parseBool(self.configurationValues.checkbox);
      else if (!isNullOrUndefined(content.enableWithCheckBoxDefaultValue))
         this.cbValue = parseBool(content.enableWithCheckBoxDefaultValue);
      else
         this.cbValue = false;
   }

   //if radioButtonSectionName is defined enableWithCheckBox can't
   assert((this.radioButtonSectionName === undefined) || (!this.enableWithCheckBox), "enableWithCheckBox parameter can't be used in section in radioSection");

   //for each key in package
   if (!isNullOrUndefined(content.content)) {
      $.each(content.content, function (key, value) {
         var v = undefined;
         if ((self.configurationValues !== undefined) && (self.configurationValues != null) && (self.configurationValues.content != null) && (self.configurationValues.content != undefined))
            v = self.configurationValues.content[key];

         var newI18nContext = i18nContext + self.paramName + ".content.";
         var handler = ConfigurationHelper.createParameterHandler(objectToConfigure, newI18nContext, key, value, v);
         self.configurationHandlers.push(handler);
      });
   }
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
SectionParameterHandler.prototype.getDOMObject = function () {
   var input = "<div class=\"control-group configuration-section well\" >" +
                  "<div class=\"configuration-header\" >";

   if (this.enableWithCheckBox) {
      input +=       "<div class=\"checkbox\">" +
                        "<label>" +
                           "<input type=\"checkbox\" id=\"" + this.selectorUuid + "\"";
      if (this.cbValue)
         input +=                "checked ";
      input +=             ">";
   }
   if (this.radioButtonSectionName) {
      input +=       "<div class=\"radio\">" +
                        "<label>" +
                           "<input type=\"radio\" id=\"" + this.selectorUuid + "\" name=\"" + this.radioButtonSectionName + "\" ";
      if (this.radioSectionActive)
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
                        "$(\"#" + this.selectorUuid + "\").change(function () {\n" +
                           "if ($(\"#" + this.selectorUuid + "\").prop(\"checked\")) {\n" +
                              "\t$(\"div#" + this.uuid + "\").removeClass(\"hidden\");\n" +
                              "\t$(\"div#" + this.uuid + "\").removeClass(\"has-warning\");\n" +
                              "\t$(\"div#" + this.uuid + "\").find(\"input,select,textarea\").addClass(\"enable-validation\");\n" +
                           "}\n" +
                           "else {\n" +
                              "\t$(\"div#" + this.uuid + "\").addClass(\"hidden\");\n" +
                              "\t$(\"div#" + this.uuid + "\").removeClass(\"has-warning\");\n" +
                              "\t$(\"div#" + this.uuid + "\").find(\"input,select,textarea\").removeClass(\"enable-validation\");\n" +
                           "}\n" +
                        "});\n" +
                     "</script>";
   }

   //if it's included in a radioSection
   if (this.radioButtonSectionName) {
      input +=          "</label>" +
                     "</div>" +
                     "<script>\n" +
                        "$(\"#" + this.selectorUuid + "\").change(function () {\n" +
                           "if ($(\"input#" + this.selectorUuid + ":checked\").val() == \"on\") {\n" +
                              "\t//we hide all sections-content in the radioSection\n" +
                              "\tvar radioSections = $(\"input#" + this.selectorUuid + "\").parents(\"div.configuration-radio-section\").find(\"div.section-content\");\n" +
                              "\tradioSections.addClass(\"hidden\");\n" +
                              "\tradioSections.removeClass(\"has-warning\");\n" +
                              "\tradioSections.find(\"input,select,textarea\").removeClass(\"enable-validation\");\n" +
                              "\t//we show current\n" +
                              "\t$(\"div#" + this.uuid + "\").removeClass(\"hidden\");\n" +
                              "\t$(\"div#" + this.uuid + "\").removeClass(\"has-warning\");\n" +
                              "\t$(\"div#" + this.uuid + "\").find(\"input,select,textarea\").addClass(\"enable-validation\");\n" +
                           "}\n" +
                        "});\n" +
                     "</script>";
   }

   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div id=\"" + this.uuid + "\" ";

   //if checkbox is unchecked or the radio button is unselected
   if (((this.enableWithCheckBox) && (!this.cbValue)) || ((this.radioSectionActive !== undefined) && (!this.radioSectionActive))) {
      input +=       "class=\"section-content hidden\" ";
   }
   else {
      input +=       "class=\"section-content\" ";
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

/**
 * Get the param name
 * @returns {string}
 */
SectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {object}
 */
SectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   self.configurationValues.content = {};
   $.each(self.configurationHandlers, function (key, value) {
      self.configurationValues.content[value.getParamName()] = value.getCurrentConfiguration();
   });

   //we get the checkbox value if used
   if (this.enableWithCheckBox) {
      self.configurationValues.checkbox = parseBool($("input#" + this.selectorUuid).prop("checked"));
   }

   //we get the radioButton value if used
   if (this.radioButtonSectionName) {
      self.configurationValues.radio = parseBool($("input#" + this.selectorUuid + ":checked").val(), false);
   }

   return self.configurationValues;
};