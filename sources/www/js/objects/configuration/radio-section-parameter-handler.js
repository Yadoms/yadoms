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
function RadioSectionParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
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
   this.i18nKey = i18nKey || paramName;
   this.content = content;
   this.containerUuid = createUUID();
   this.uuid = createUUID();
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

      var radioActive = false;
      if (self.configurationValues.activeSection == key)
         radioActive = true;

      if (value.show !== undefined && !parseBool(value.show.result))
         return;
      
	    var handler = ConfigurationHelper.createParameterHandler(newI18nContext, value.i18nKey, key, value, v, self.uuid, radioActive);
      if (!isNullOrUndefined(handler))
         self.configurationHandlers.push(handler);
   });
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
RadioSectionParameterHandler.prototype.getDOMObject = function () {

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
   
   input +=          "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.i18nKey + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div class=\"toggle-btn-grp\" id=\"" + this.containerUuid + "\" >";

   //for each param in the section we create a radio button and a div that contain the item
   $.each(this.configurationHandlers, function (key, value) {
      input +=             value.getDOMObject();
   });

   input +=       "</div>" +
               "</div>";

   return input;
};

RadioSectionParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
RadioSectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Apply script after DOM object has been added to the page
 */
RadioSectionParameterHandler.prototype.applyScript = function () {
   var self = this;

   //we manage change of children radio button
   $("#" + self.containerUuid + " [name=" + self.uuid + "]").change(function (item) {
      self.setEnabled(true);
   });

   //we apply script in each children
   $.each(this.configurationHandlers, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });

   //can't factorize must happen after intialize all handlers
   $.each(this.configurationHandlers, function (key, value) {
      //if this child is the active one we fire the event that we have changed to it the radio button
      if (value.parentRadioSectionActive)
         $("#" + value.selectorUuid).change();
   });
};

/**
 * Enable / Disbale the content of the comnfiguration item
 */
RadioSectionParameterHandler.prototype.setEnabled = function (enabled) {
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

    var uuidOfActive = $("#" + self.containerUuid + " [name=" + self.uuid + "]:checked").attr('id');
    $.each(self.configurationHandlers, function (key, value) {
          //we set enable children with true only if it has to be neabled and if it is the active radio item selected 
          if ($.isFunction(value.setEnabled)) {
                value.setEnabled((value.selectorUuid == uuidOfActive) && enabled);
          }
    });
}

/**
 * Get the current configuration in the form
 * @returns {object}
 */
RadioSectionParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var d= new $.Deferred();
   var self = this;
   self.configurationValues = {};
   self.configurationValues.content = {};
   var deferredArray =[];
   
   $.each(self.configurationHandlers, function (key, value) {
      var deferred = value.getCurrentConfiguration();
      deferredArray.push(deferred);
      deferred.done(function (currentConfiguration) {
         self.configurationValues.content[value.getParamName()] = currentConfiguration;
         if (currentConfiguration.radio) {
            //it's the active section
            self.configurationValues.activeSection = value.getParamName();
         }
      });      
   });

   $.whenAll(deferredArray)
   .done(function() {
      //we get the parentRadioButtonSectionName value if used (only for nested radio section into another one)
      if (self.parentRadioButtonSectionName) {
         self.configurationValues.radio = ($("input#" + self.selectorUuid + ":checked").val() == 'on');
      }
      
      d.resolve(self.configurationValues);
   });   
   
   return d.promise();      
};