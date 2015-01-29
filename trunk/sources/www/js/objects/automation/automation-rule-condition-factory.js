/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function AutomationRuleConditionFactory(){}

AutomationRuleConditionFactory.getKeyOfConfiguration = function(configuration) {
   assert(!isNullOrUndefined(configuration), "configuration must be defined");

   //configuration must contain a key / value pair item and only one
   var counter = 0;
   $.map(configuration, function(val, key) {
      counter++;
   });

   assert (counter == 1, "Can't create a conditional item with configuration that do not have exactly one key value pair item : " + JSON.stringify(configuration));

   for (var key in configuration) {
      return key;
   }
};

AutomationRuleConditionFactory.getContentOfConfiguration = function(configuration) {
   assert(!isNullOrUndefined(configuration), "configuration must be defined");

   var key = AutomationRuleConditionFactory.getKeyOfConfiguration(configuration);
   return configuration[key];
};

AutomationRuleConditionFactory.createItem = function (configuration, treeviewContainer) {
   assert(!isNullOrUndefined(configuration), "configuration must be defined");
   assert(!isNullOrUndefined(treeviewContainer), "treeviewContainer must be defined");

   var key = AutomationRuleConditionFactory.getKeyOfConfiguration(configuration);
   var content = AutomationRuleConditionFactory.getContentOfConfiguration(configuration);

   switch (key.toLowerCase()) {
      case "is" :
         return new AutomationRuleOperatorIs(content, treeviewContainer);
         break;

      case "and" :
         return new AutomationRuleOperatorAnd(content, treeviewContainer);
         break;

      default :
         throw Error("type " + key + " is unsupported");
         break;
   }
};
