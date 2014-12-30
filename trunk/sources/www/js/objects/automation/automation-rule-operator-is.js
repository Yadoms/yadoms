/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRuleOperatorIs(content) {
   assert(!isNullOrUndefined(content), "content of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.operator), "operator of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.keywordId), "keywordId of a AutomationRuleOperatorIs must be defined");
   assert(!isNullOrUndefined(content.refValue), "refValue of a AutomationRuleOperatorIs must be defined");

   this.operator = content.operator;
   this.keyword = null;
   this.setKeyword(content.keywordId);
   this.refValue = content.refValue;
   this.keywordParameter = null;
}

AutomationRuleOperatorIs.keywordParamName = "keyword";

AutomationRuleOperatorIs.prototype.toJSON = function () {
   return {
      operator : this.operator,
      keywordId: encodeURIComponent(this.keywordId),
      refValue: this.refValue
   };
};

AutomationRuleOperatorIs.prototype.setKeyword = function (keywordId) {
   assert(!isNullOrUndefined(keywordId), "keywordId must be defined");

   var self = this;
   this.keywordId = keywordId;
   //we ask for the keyword synchronously
   KeywordManager.get(this.keywordId, function (keyword) {
      self.keyword = keyword;
   }, true);
};

AutomationRuleOperatorIs.prototype.isValid = function () {
   return (!isNullOrUndefined(this.keyword)) && (!isNullOrUndefined(this.conditionalValue));
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
   return this.keyword.friendlyName + "<b> " + operator + " </b>"+ this.refValue;
}

AutomationRuleOperatorIs.prototype.getTreeviewItem = function () {
   return "<span><i class=\"glyphicon glyphicon-export\"></i> " + this.toString() + "</span>";
}

AutomationRuleOperatorIs.prototype.displayConfiguration = function($container) {
   var i18nContext = "objects.AutomationCenter.Rule.Condition.Is.Configuration.";

   //configuration item creation
   this.keywordParameter = new DeviceParameterHandler(i18nContext,
                                                      AutomationRuleOperatorIs.keywordParamName,
                                                      {"expectedKeywordAccess" : "get"},
                                                      {deviceId : this.keyword.deviceId, keywordId : this.keyword.id});

   //TODO : ajouter les autres champs de configuration

   //add to gui
   $container.append(this.keywordParameter.getDOMObject());

   //we finish controls instantiation
   if ($.isFunction(this.keywordParameter.applyScript))
      this.keywordParameter.applyScript();

   //we i18n the form
   $container.i18n();

}