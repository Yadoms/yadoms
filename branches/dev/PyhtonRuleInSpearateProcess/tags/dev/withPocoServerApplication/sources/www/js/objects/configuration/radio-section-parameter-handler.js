/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a radio section parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function RadioSectionParameterHandler(i18nContext, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(Object.keys(content.content).length >= 2, "You must have at least two sub sections into a radioSection");

   this.configurationHandlers = [];
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
   this.uuid = createUUID();
   this.radioGroupUuid = createUUID();
   var self = this;

   //we look for parent radio button
   if (parentRadioButtonSectionName !== undefined) {
      this.parentRadioButtonSectionName = parentRadioButtonSectionName;
      this.parentRadioSectionActive = false;
      if (parentRadioSectionActive !== undefined)
         this.parentRadioSectionActive = parseBool(parentRadioSectionActive);
   }
   
   if (isNullOrUndefined(self.configurationValues))
      self.configurationValues = {};

   if (isNullOrUndefined(self.configurationValues.activeSection)) {
      //for the moment no section have been defined so we select the first one
      self.configurationValues.activeSection = Object.keys(content.content)[0];
   }


   //for each key in package
   $.each(content.content, function (key, value) {
      var v = undefined;
      if ((self.configurationValues.content != null) && (self.configurationValues.content != undefined))
         v = self.configurationValues.content[key];

      var newI18nContext = i18nContext + self.paramName + ".content.";

      //all items in content must be section values
      assert((value.type !== undefined), "Content section of the configuration " + self.name + " must be defined");
      assert((value.type.toLowerCase() == "section") || (value.type.toLowerCase() == "radiosection"), "Content section of the configuration " + self.name + " must contain only section items");
	  
      var radioActive = false;
      if (self.configurationValues.activeSection == key)
         radioActive = true;

	  var handler;
	  if (value.type.toLowerCase() == "section") {
		 handler = new SectionParameterHandler(newI18nContext, key, value, v, self.radioGroupUuid, radioActive);
	  }
	  else
	  {
	     handler = new RadioSectionParameterHandler(newI18nContext, key, value, v, self.radioGroupUuid, radioActive);
	  }
      
      self.configurationHandlers.push(handler);
   });
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
RadioSectionParameterHandler.prototype.getDOMObject = function () {

   var input = "<div class=\"control-group configuration-radio-section well\" id=\"" + this.radioGroupUuid + "\">" +
                  "<div class=\"configuration-header\" >";
	
   if (this.parentRadioButtonSectionName) {
      input +=       "<div class=\"radio\">" +
                        "<label>" +
                           "<input type=\"radio\" id=\"" + this.selectorUuid + "\" name=\"" + this.parentRadioButtonSectionName + "\" ";
      if (this.parentRadioSectionActive)
         input +=                "checked ";
      input +=             ">";
   }
	
   input +=           "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                        this.name +
                     "</span>";
   //if it's included in a parent radioSection 
   if (this.parentRadioButtonSectionName) {
      input +=          "</label>" +
                     "</div>";
   }
   
   input +=          "</div>" +
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

RadioSectionParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.radioGroupUuid);
}

/**
 * Get the param name
 * @returns {string}
 */
RadioSectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Apply script after DOM object has been added to the page
 * @returns {}
 */
RadioSectionParameterHandler.prototype.applyScript = function () {
   var self = this;
   
   if (self.parentRadioButtonSectionName) {
      $("#" + self.selectorUuid).change(function () {
         if ($("input#" + self.selectorUuid + ":checked").val() == "on") {
            //we hide all sections-content in the parent radioSection\n" +
            var radioSections = $("div#" + self.radioButtonSectionName + " > div.toggle-btn-grp > div.configuration-section > div.section-content");
            radioSections.addClass("hidden");
            radioSections.removeClass("has-warning");
            radioSections.find("input,select,textarea").removeClass("enable-validation");
            //we show current
            $("div#" + self.uuid).removeClass("hidden").removeClass("has-warning");
            $("div#" + self.uuid).find("input,select,textarea").addClass("enable-validation");
         }
      });
   }
   
   //we apply script in each children
   $.each(this.configurationHandlers, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });
}

/**
 * Get the current configuration in the form
 * @returns {object}
 */
RadioSectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   self.configurationValues.content = {};
   $.each(self.configurationHandlers, function (key, value) {
      var currentConfiguration = value.getCurrentConfiguration();
      self.configurationValues.content[value.getParamName()] = currentConfiguration;
      if (currentConfiguration.radio) {
         //it's the active section
         self.configurationValues.activeSection = value.getParamName();
      }
   });

   return self.configurationValues;
};