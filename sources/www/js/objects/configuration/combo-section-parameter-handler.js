/**
 * Created by Nicolas on 17-11-16.
 */

/**
 * Create a combo section parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function ComboSectionParameterHandler(i18nContext, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(Object.keys(content.content).length >= 2, "You must have at least two sub sections into a comboSection");

   this.configurationHandlers = [];
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
   this.uuid = createUUID();
   this.containerUuid = createUUID();
   this.comboUuid = createUUID();
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
      assert(ConfigurationHelper.isContainer(value), "Content section of the configuration " + self.name + " must contain only section items");
      assert((value.type.toLowerCase() !== "section") || ((value.type.toLowerCase() === "section") && (!value.enableWithCheckBox)), "enableWithCheckBox parameter can't be used in section inside comboSection");

      if (value.show !== undefined && value.show.result === "false")
         return;
      
	   var handler = ConfigurationHelper.createParameterHandler(newI18nContext, key, value, v);
      if (!isNullOrUndefined(handler))
         self.configurationHandlers.push(handler);
   });
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
ComboSectionParameterHandler.prototype.getDOMObject = function () {

   var input = "<div class=\"control-group configuration-radio-section well\" id=\"" + this.uuid + "\">" +
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
   
   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div>" +
                     "<select class=\"form-control\" id=\"" + this.comboUuid + "\">";
   //we add each sub containers name in the combo
   $.each(this.configurationHandlers, function (key, value) {
      input +=             "<option value=\"" + value.uuid + "\" data-i18n=\"" + value.i18nContext + value.paramName + ".name\">" + value.name + "</option>";
   });

    input +=         "</select>" +
                  "</div>" +
                  "<div id=\"" + this.containerUuid + "\" >";

   //for each param in the section we create a radio button and a div that contain the item
   $.each(this.configurationHandlers, function (key, value) {
      input +=             value.getDOMObject();
   });

   input +=       "</div>" +
               "</div>";

   return input;
};

ComboSectionParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
ComboSectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Apply script after DOM object has been added to the page
 */
ComboSectionParameterHandler.prototype.applyScript = function () {
  var self = this;

  if (self.parentRadioButtonSectionName) {
      $("#" + self.selectorUuid).change(function () {
         if ($("input#" + self.selectorUuid + ":checked").val() == "on") {
            //we hide all sections-content in the radioSection\n" +
		        var $parentSection = $("div#" + self.parentRadioButtonSectionName);
            var radioSections = $parentSection.find(" > div.toggle-btn-grp > div.configuration-section > div.section-content");
            radioSections.addClass("hidden");
            $parentSection.removeClass("has-warning has-error");
            $parentSection.find("input,select,textarea").removeClass("enable-validation");
		        //Disable all existing sub-buttons
		        $parentSection.find("button").attr("disabled", true);

            //We save all items that are "required" with a special class name : required-for-validation
            var $requiredFields = radioSections.find("[required]");
            //we remove attr required and save it using class "required-for-validation"
            $requiredFields.addClass("required-for-validation");
            $requiredFields.removeAttr("required");

            //we show current
            var $activeContainer = $("div#" + self.containerUuid);
            $activeContainer.removeClass("hidden");
            $activeContainer.find("input,select,textarea").addClass("enable-validation");
            //Enable all existing sub-buttons
		        $activeContainer.find("button").removeAttr("disabled");
            //we restore required items
            $activeContainer.find(".required-for-validation").attr("required", "required");
         }
      });
   }

    //we manage sub containers items
    //and make all disappear except the current one
    $("#" + self.comboUuid).change(function () {
          //we hide all sub-containers except the active one
          var $subContainers = $("#" + self.containerUuid + " > div");
          $subContainers.addClass("hidden");
          $subContainers.removeClass("has-warning has-error");
          $subContainers.find("input,select,textarea").removeClass("enable-validation");
          //Disable all existing sub-buttons
          $subContainers.find("button").attr("disabled", true);

          //We save all items that are "required" with a special class name : required-for-validation
          var $requiredFields = $subContainers.find("[required]");
          //we remove attr required and save it using class "required-for-validation"
          $requiredFields.addClass("required-for-validation");
          $requiredFields.removeAttr("required");

          //we show the active one
          var uuidOfActive = $("#" + self.comboUuid + " option:selected").val();
          var $activeContainer = $("#" + uuidOfActive);
          $activeContainer.removeClass("hidden").removeClass("has-warning has-error");
          $activeContainer.find("input,select,textarea").addClass("enable-validation");
          //Enable all existing sub-buttons
          $activeContainer.find("button").removeAttr("disabled");
          //we restore required items
          $activeContainer.find(".required-for-validation").attr("required", "required");
      });
    //we apply script in each children
    $.each(this.configurationHandlers, function (key, value) {
      if ($.isFunction(value.applyScript))
          value.applyScript();
    });

    //select the active item in the combo
    //we look for the active one
    var activeSectionUuid;
    $.each(this.configurationHandlers, function (key, value) {
      if (self.configurationValues.activeSection === value.getParamName()) {
        activeSectionUuid = value.uuid;
      }
    });
    
    //is the active value doesn't match we take the first one
    if (!activeSectionUuid)
      activeSectionUuid = this.configurationHandlers[0].uuid;
    
    $("#" + self.comboUuid).val(activeSectionUuid).change();
};

/**
 * Get the current configuration in the form
 * @returns {object}
 */
ComboSectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = {};
   self.configurationValues.content = {};
   //we save the uuid of the active sub section
   var uuidOfActive = $("#" + self.comboUuid + " option:selected").val();

   $.each(self.configurationHandlers, function (key, value) {
      var currentConfiguration = value.getCurrentConfiguration();
      self.configurationValues.content[value.getParamName()] = currentConfiguration;
      if (value.uuid == uuidOfActive) {
         //it's the active section
         self.configurationValues.activeSection = value.getParamName();
      }
   });

   return self.configurationValues;
};