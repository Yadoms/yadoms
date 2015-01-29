/** @module Device class */

/**
 * Creates an instance of Device
 * @constructor
 */
function AutomationRuleOperatorAnd(content, treeviewContainer) {
   assert(!isNullOrUndefined(content), "content of a AutomationRuleOperatorAnd must be defined");
   assert(!isNullOrUndefined(treeviewContainer), "treeviewContainer of a AutomationRuleOperatorAnd must be defined");

   this.treeviewContainer = treeviewContainer;
}

AutomationRuleOperatorAnd.i18nContext = "objects.AutomationCenter.Rule.Condition.And.Configuration.";

AutomationRuleOperatorAnd.prototype.toJSON = function () {
   //no specific content
   return {
   };
};

AutomationRuleOperatorAnd.prototype.getName = function () {
   //TODO : I18N
   return "AND";
};

AutomationRuleOperatorAnd.prototype.isValid = function () {
   //a AND is always valid
   return true;
};

AutomationRuleOperatorAnd.prototype.updateConfiguration = function () {
   //nothing to do
};

AutomationRuleOperatorAnd.prototype.toString = function () {
   //no text
   return "";
};

AutomationRuleOperatorAnd.prototype.getNumberOfChildren = function () {
   return 2;
};

AutomationRuleOperatorAnd.prototype.displayConfiguration = function($container) {
   var self = this;

   //no conf
};