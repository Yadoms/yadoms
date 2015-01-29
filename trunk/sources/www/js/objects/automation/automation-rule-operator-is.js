/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRuleOperatorIs(content, treeviewContainer) {
   assert(!isNullOrUndefined(content), "content of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.operator), "operator of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.keywordId), "keywordId of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.refValue), "refValue of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(treeviewContainer), "treeviewContainer of a AutomationRuleOperatorIs must be defined");

   this.operator = content.operator;
   this.keyword = null;
   this.device = null;
   this.plugin = null;
   this.setKeyword(content.keywordId);
   this.refValue = content.refValue;
   this.keywordParameter = null;
   this.operatorParameter = null;
   this.refValueParameter = null;
   this.treeviewContainer = treeviewContainer;
}

AutomationRuleOperatorIs.keywordParamName = "keyword";
AutomationRuleOperatorIs.operatorParamName = "operator";
AutomationRuleOperatorIs.refValueParamName = "refValue";
AutomationRuleOperatorIs.i18nContext = "objects.AutomationCenter.Rule.Condition.Is.Configuration.";

AutomationRuleOperatorIs.prototype.toJSON = function () {
   return {
      operator : this.operator,
      keywordId: encodeURIComponent(this.keywordId),
      refValue: this.refValue
   };
};

AutomationRuleOperatorIs.prototype.setKeyword = function (keywordId) {

   var self = this;
   //we make the job only if it is necessary
   if (self.keywordId != keywordId) {
      self.keywordId = keywordId;
      self.keyword = null;
      self.device = null;
      self.plugin = null;
      if (!isNullOrUndefinedOrEmpty(keywordId)) {
         //we ask for the keyword synchronously only if it is valid
         KeywordManager.get(this.keywordId, function (keyword) {
            self.keyword = keyword;
            //we ask for the device synchronously
            DeviceManager.get(self.keyword.deviceId, function (device) {
               self.device = device;
               //we ask for the plugin synchronously
               PluginInstanceManager.get(self.device.pluginId, function (device) {
                  self.plugin = device;
               }, true);
            }, true);
         }, true);
      }
   }
};

AutomationRuleOperatorIs.prototype.getName = function () {
   return "IS";
};

AutomationRuleOperatorIs.prototype.isValid = function () {
   return (!isNullOrUndefined(this.keyword)) && (!isNullOrUndefined(this.refValueParameter));
};

AutomationRuleOperatorIs.prototype.updateConfiguration = function () {
   //we take configuration from configuration items to set object
   var configuration = this.keywordParameter.getCurrentConfiguration();

   //we change keyword only if it is != from null "" is acceptable
   if (!isNullOrUndefined(configuration.keywordId))
      this.setKeyword(configuration.keywordId);

   if (!isNullOrUndefined(this.operatorParameter)) {
      this.operator = this.operatorParameter.getCurrentConfiguration();
   }

   if (!isNullOrUndefined(this.refValueParameter)) {
      this.refValue = this.refValueParameter.getCurrentConfiguration();
   }
};

AutomationRuleOperatorIs.prototype.toString = function () {
   var operator;
   switch (this.operator.toLowerCase())
   {
      case "lower" :
         operator = "<";
         break;

      case "lowerorequal" :
         operator = "<=";
         break;

      case "equal" :
         operator = "=";
         break;

      case "different" :
         operator = "<>";
         break;

      case "greater" :
         operator = ">";
         break;

      case "greaterorequal" :
         operator = ">=";
         break;
   }
   var keyword;
   if ((!isNullOrUndefined(this.plugin) && (!isNullOrUndefined(this.device) && (!isNullOrUndefined(this.keyword))))) {
      keyword = this.plugin.name + "." + this.device.friendlyName + "." + this.keyword.friendlyName;
      return keyword + "<b> " + operator + " </b>"+ this.refValue;
   }
   else {
      return "invalid";
   }
};
/*
AutomationRuleOperatorIs.prototype.updateDom = function () {
   var self = this;
   var $content = self.locateInDOM().find(".content");

   $content.html(self.toString());

   if ((!isNullOrUndefined(this.plugin) && (!isNullOrUndefined(this.device) && (!isNullOrUndefined(this.keyword))))) {
      $content.removeClass("label-danger");
   }
   else {
      $content.addClass("label-danger");
   }
};*/
/*
AutomationRuleOperatorIs.prototype.getChilds = function () {
   return [];
};
*/

AutomationRuleOperatorIs.prototype.getNumberOfChildren = function () {
   return 2;
};

AutomationRuleOperatorIs.prototype.updateOperatorListItems = function () {
   //depending on the type of the keyword, the operator item list must change
   if (!isNullOrUndefined(this.keyword)) {
      switch (this.keyword.type) {
         case "numeric" :
            this.operatorParameter.setValues({
               "lower" : "<",
               "lowerorequal" : "<=",
               "equal" : "=",
               "different" : "<>",
               "greater" : ">",
               "greaterorequal" : ">="
            });
            break;

         case "bool" :
         case "string" :
            this.operatorParameter.setValues({
               "equal" : "=",
               "different" : "<>"
            });
            break;
      }
   }
   else {
      this.operatorParameter.setValues({
         "equal" : "=",
         "different" : "<>"
      });
   }
};

AutomationRuleOperatorIs.prototype.updateValueParameter = function () {
   var self = this;
   self.$refValueContainer.empty();
   //we create the right parameter object depending on the type of the keyword
   if (!isNullOrUndefined(self.keyword)) {
      switch (self.keyword.type) {
         case "numeric" :
             self.refValueParameter = new DecimalParameterHandler(AutomationRuleOperatorIs.i18nContext,
                                                                 AutomationRuleOperatorIs.refValueParamName,
                                                                 {}, self.refValue);
             break;

         case "bool" :
            self.refValueParameter = new EnumParameterHandler(AutomationRuleOperatorIs.i18nContext,
                AutomationRuleOperatorIs.refValueParamName,
                {"values" : {"true" : "true", "false" : "false"}}, self.refValue);
            break;

         case "string" :
            self.refValueParameter = new StringParameterHandler(AutomationRuleOperatorIs.i18nContext,
                AutomationRuleOperatorIs.refValueParamName,
                {}, self.refValue);
            break;
      }

      self.$refValueContainer.append(self.refValueParameter.getDOMObject());
      if ($.isFunction(self.refValueParameter.applyScript))
         self.refValueParameter.applyScript();

      self.refValueParameter.locateInDOM().bind("change", self.configurationChanged());
      self.$refValueContainer.i18n();
   }
};

AutomationRuleOperatorIs.prototype.displayConfiguration = function($container) {
   var self = this;

   //configuration item creation
   this.keywordParameter = new DeviceParameterHandler(AutomationRuleOperatorIs.i18nContext,
                                                      AutomationRuleOperatorIs.keywordParamName,
                                                      {"expectedKeywordAccess" : "get", "expectedKeywordType" : ["numeric", "bool", "string"]},
                                                      {deviceId : (this.keyword)?this.keyword.deviceId:null,
                                                       keywordId : (this.keyword)?this.keyword.id:null});

   this.operatorParameter = new EnumParameterHandler(AutomationRuleOperatorIs.i18nContext,
                                                      AutomationRuleOperatorIs.operatorParamName,
                                                      {"values" : {}}, this.operator);

   this.refValueParameter = null;

   //add to gui
   $container.append(this.keywordParameter.getDOMObject());
   $container.append(this.operatorParameter.getDOMObject());
   $container.append("<div class=\"refValue\"></div>");

   self.$refValueContainer = $container.find("div.refValue");

   //we finish controls instantiation
   if ($.isFunction(this.keywordParameter.applyScript))
      this.keywordParameter.applyScript();

   this.keywordParameter.locateInDOM().bind("change", self.keywordChanged());
   this.operatorParameter.locateInDOM().bind("change", self.configurationChanged());

   this.updateOperatorListItems();
   this.updateValueParameter();

   //we i18n the form
   $container.i18n();

};

AutomationRuleOperatorIs.prototype.keywordChanged = function() {
   var self = this;
   return function() {
      //we update configuration before display it
      self.updateConfiguration();
      //depend on the type of the keyword, the enum and the expected value change
      self.updateOperatorListItems();
      //the value field depend also on the type of the keyword
      self.updateValueParameter();

      //the double () is used to call returned method
      self.configurationChanged()();
   }
};

AutomationRuleOperatorIs.prototype.configurationChanged = function() {
   var self = this;
   return function() {
      //we update configuration before display it
      self.updateConfiguration();
      self.treeviewContainer.locateInDOM().change();
      //self.updateDom();
   }
};