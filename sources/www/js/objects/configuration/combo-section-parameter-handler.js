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

   this.configurationHandlers = [];
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.i18nKey = i18nKey || paramName;
   this.content = content;
   this.uuid = createUUID();
   this.dropdownUuid = createUUID();
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
'<div class="dropdown">' +
'   <button class="btn btn-default dropdown-toggle btn-combo" type="button" id="' + this.comboUuid + '" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true"><div class="current-selected-item pull-left"></div><span class="caret"></span></button>'+
'   <ul class="dropdown-menu" id="' + this.dropdownUuid + '" aria-labelledby="' + this.comboUuid + '">';

   $.each(this.configurationHandlers, function (key, value) {
      input += '<li>'+
               '<a href="#" data-value="' + value.uuid + '">' + 
                  '<span class="combo-item-title" data-i18n="' + value.i18nContext + value.i18nKey + '.types.' + value.paramName + '.title">' + value.name + '</span>' + '<br/>' +
                  '<span class="combo-item-description" data-i18n="' + value.i18nContext + value.i18nKey + '.types.' + value.paramName + '.description"></span>' +
               '</a>' + 
            '</li>';
   });  

input +=   '</ul></div>';
                  
    input +=      "</div>" +
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
   var $ul = this.locateInDOM().find("#" + this.dropdownUuid);
   
   var $li = $ul.find("li");
   //calling detach to avoid removing any data/events associated with the li nodes.
   $li.detach().sort(function(a, b) {
      let aTxt = $(a).find("a span").first().text();
      let bTxt = $(b).find("a span").first().text();
      
      return aTxt.toLowerCase().localeCompare(bTxt.toLowerCase());
   });
   $ul.append($li);
   
   $.each($("#" + this.dropdownUuid).find(".combo-item-description"), function (key, value) {
      if($(value).text() === "") {
         $(value).siblings(".combo-item-title").removeClass("combo-item-title").addClass("combo-item-title-without-description");
      }
   });   
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

   $("#" + self.dropdownUuid).find("li a").click(function(){
     $("#" + self.comboUuid).find(".current-selected-item").html($(this).html());
     $("#" + self.comboUuid).val($(this).data('value')).change();
   });
    
    //we manage sub containers items
    //and make all disappear except the current one
    $("#" + self.comboUuid).change(function () {
          var uuidOfActive = $("#" + self.comboUuid).val();
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
    
    $("#" + self.dropdownUuid).find("li a[data-value='" + activeSectionUuid +"']").trigger('click');
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

    var uuidOfActive = $("#" + self.comboUuid).val();
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
   var d= new $.Deferred();
   var self = this;
   self.configurationValues = {};
   self.configurationValues.content = {};
   var deferredArray =[];
   
   //we save the uuid of the active sub section
   var uuidOfActive = $("#" + self.comboUuid).val();
   
   $.each(self.configurationHandlers, function (key, value) {
      var deferred = value.getCurrentConfiguration();
      deferredArray.push(deferred);
      deferred.done(function (currentConfiguration) {
         self.configurationValues.content[value.getParamName()] = currentConfiguration;
         if (value.uuid == uuidOfActive) {
            //it's the active section
            self.configurationValues.activeSection = value.getParamName();
            self.configurationValues.activeSectionText = value.name; //value.name is available
         }
      });      
   });

   $.whenAll(deferredArray)
   .done(function() {
      d.resolve(self.configurationValues);
   });   
   
   return d.promise();       
};