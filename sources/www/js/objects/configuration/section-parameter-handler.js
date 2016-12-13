/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a section parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @param parentRadioButtonSectionName
 * @param parentRadioSectionActive
 * @constructor
 */
function SectionParameterHandler(i18nContext, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
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
   this.containerUuid = createUUID();
   this.selectorUuid = createUUID();

   //we look for radio button
   if (parentRadioButtonSectionName !== undefined) {
      this.parentRadioButtonSectionName = parentRadioButtonSectionName;
      this.parentRadioSectionActive = false;
      if (parentRadioSectionActive !== undefined)
         this.parentRadioSectionActive = parseBool(parentRadioSectionActive);
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

   //if parentRadioButtonSectionName is defined enableWithCheckBox can't
   assert((this.parentRadioButtonSectionName === undefined) || (!this.enableWithCheckBox), "enableWithCheckBox parameter can't be used in section inside radioSection");

   //for each key in package
   if (!isNullOrUndefined(content.content)) {
      $.each(content.content, function (key, value) {
         var v = undefined;
		 var newI18nContext = undefined;
		 
         if ((self.configurationValues !== undefined) && (self.configurationValues != null) && (self.configurationValues.content != null) && (self.configurationValues.content != undefined))
            v = self.configurationValues.content[key];

		if (!isNullOrUndefined(self.paramName)){
           newI18nContext = i18nContext + self.paramName + ".content.";
		}
		else{
		   newI18nContext = i18nContext + "content.";
		}
		 
         var handler = ConfigurationHelper.createParameterHandler(newI18nContext, key, value, v);
         if (!isNullOrUndefined(handler))
            self.configurationHandlers.push(handler);
      });
   }
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
SectionParameterHandler.prototype.getDOMObject = function () {
	
   var input = "<div class=\"control-group configuration-section well\" id=\"" + this.uuid + "\">" +
                  "<div class=\"configuration-header\" >";

   if (this.enableWithCheckBox) {
      input +=       "<div class=\"checkbox\">" +
                        "<label>" +
                           "<input type=\"checkbox\" id=\"" + this.selectorUuid + "\"";
      if (this.cbValue)
         input +=                "checked ";
      input +=             ">";
   }
   if (this.parentRadioButtonSectionName) {
      input +=       "<div class=\"radio\">" +
                        "<label>" +
                           "<input type=\"radio\" id=\"" + this.selectorUuid + "\" name=\"" + this.parentRadioButtonSectionName + "\" ";
      if (this.parentRadioSectionActive)
         input +=                "checked ";
      input +=             ">";
   }
   
   if (!isNullOrUndefined(this.paramName)){
	   input +=                "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
								  this.name +
							   "</span>";
   }
   else{
	   input +=                "<span data-i18n=\"" + this.i18nContext + "name\" >" +
								  this.name +
							   "</span>";
   }
   //if it's included in a radioSection or in a checkbox section
   if ((this.enableWithCheckBox) || (this.parentRadioButtonSectionName)) {
      input +=          "</label>" +
                     "</div>";
   }
   
   if (!isNullOrUndefined(this.paramName)){
	   input +=       "</div>" +
					  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
						 this.description +
					  "</div>" +
					  "<div id=\"" + this.containerUuid + "\" ";
   }
   else{
	   input +=       "</div>" +
					  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + "description\" >" +
						 this.description +
					  "</div>" +
					  "<div id=\"" + this.containerUuid + "\" ";	   
   }

   //if checkbox is unchecked or the radio button is unselected
   if (((this.enableWithCheckBox) && (!this.cbValue)) || ((this.parentRadioSectionActive !== undefined) && (!this.parentRadioSectionActive))) {
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
 * Apply script after DOM object has been added to the page
 */
SectionParameterHandler.prototype.applyScript = function () {
   var self = this;
   //TODO : remove
   if (self.enableWithCheckBox) {
      $("#" + self.selectorUuid).change(function () {
         if ($("#" + self.selectorUuid).prop("checked")) {
            $("div#" + self.containerUuid).removeClass("hidden").removeClass("has-warning");
            $("div#" + self.containerUuid).find("input,select,textarea").addClass("enable-validation");
         }
         else {
            $("div#" + self.containerUuid).addClass("hidden").removeClass("has-warning");
            $("div#" + self.containerUuid).find("input,select,textarea").removeClass("enable-validation");
         }
      });
   }

   //we apply script in each children
   $.each(self.configurationHandlers, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });
};

SectionParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
SectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the comnfiguration item
 */
SectionParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    //we disable / enable the control of the radio or the checkbox
    if (enabled) {
            //if there is a radio we must hide container, if not we must hide all 
            if (self.parentRadioButtonSectionName)
                  $("#" + self.containerUuid).addClass("enable-validation").removeClass("hidden");
            else
                  $("#" + self.uuid).addClass("enable-validation").removeClass("hidden");

    } else {
            //if there is a radio we must show container, if not we must show all 
            if (self.parentRadioButtonSectionName)
                  $("#" + self.containerUuid).removeClass("enable-validation").addClass("hidden");
            else
                  $("#" + self.uuid).removeClass("enable-validation").addClass("hidden");
    }

    $.each(self.configurationHandlers, function (key, value) {
       if ($.isFunction(value.setEnabled))
          value.setEnabled(enabled);
    });
}

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

   //we get the parentRadioButtonSectionName value if used
   if (this.parentRadioButtonSectionName) {
      self.configurationValues.radio = ($("input#" + this.selectorUuid + ":checked").val() == 'on');
   }

   return self.configurationValues;
};