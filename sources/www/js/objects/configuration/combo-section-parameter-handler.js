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
function ComboSectionParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
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
   this.i18nKey = i18nKey || paramName;
   this.content = content;
   this.uuid = createUUID();
   this.containerUuid = createUUID();
   this.comboUuid = createUUID();
   this.selectorUuid = createUUID();
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

      var newI18nContext = i18nContext + self.i18nKey + ".content.";

      //all items in content must be section values
      assert(ConfigurationHelper.isContainer(value), "Content section of the configuration " + self.name + " must contain only section items");
      assert((value.type.toLowerCase() !== "section") || ((value.type.toLowerCase() === "section") && (!value.enableWithCheckBox)), "enableWithCheckBox parameter can't be used in section inside comboSection");

      if (value.show !== undefined && value.show.result === "false")
         return;
      
	    var handler = ConfigurationHelper.createParameterHandler(newI18nContext, value.i18nKey, key, value, v);
      if (!isNullOrUndefined(handler))
         self.configurationHandlers.push(handler);
   });

   self.setEnabled(true);
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
	
   input +=           "<span data-i18n=\"" + this.i18nContext + this.i18nKey + ".name\" >" +
                        this.name +
                     "</span>";
   //if it's included in a parent radioSection 
   if (this.parentRadioButtonSectionName) {
      input +=          "</label>" +
                     "</div>";
   }
   
   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.i18nKey + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div>" +
                     "<select class=\"form-control\" id=\"" + this.comboUuid + "\">";
   //we add each sub containers name in the combo
   $.each(this.configurationHandlers, function (key, value) {
      input +=             "<option value=\"" + value.uuid + "\" data-i18n=\"" + value.i18nContext + value.i18nKey + ".name\">" + value.name + "</option>";
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

ComboSectionParameterHandler.prototype.afterI18n = function() {
   var $cmb = this.locateInDOM().find("#" + this.comboUuid);
   
   var my_options = $cmb.find("option");
   my_options.sort(function(a,b) {
       if (a.text > b.text) return 1;
       else if (a.text < b.text) return -1;
       else return 0
   });

   $cmb.empty().append(my_options);
   $cmb.find("option:eq(0)").prop("selected", true);
}

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

    //we manage sub containers items
    //and make all disappear except the current one
    $("#" + self.comboUuid).change(function () {
          var uuidOfActive = $("#" + self.comboUuid + " option:selected").val();
          //we enable sub components of active combo
          $.each(self.configurationHandlers, function (key, value) {
            if ($.isFunction(value.setEnabled)) {
              value.setEnabled(value.uuid == uuidOfActive);
            }
          });
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
 * Enable / Disbale the content of the comnfiguration item
 */
ComboSectionParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    //we disable / enable the control of the radio or the checkbox
    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation").removeClass("hidden");
    } else {
            $("#" + self.uuid).removeClass("enable-validation").addClass("hidden");
    }

    var uuidOfActive = $("#" + self.comboUuid + " option:selected").val();
    //we enable sub components of active combo
    $.each(self.configurationHandlers, function (key, value) {
      if ($.isFunction(value.setEnabled)) {
        value.setEnabled((value.uuid == uuidOfActive)  && enabled);
      }
    });
}

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
         self.configurationValues.activeSectionText = value.name; //value.name is available
      }
   });

   return self.configurationValues;
};