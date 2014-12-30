/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function AutomationRuleConditionFactory(){}

AutomationRuleConditionFactory.createItem = function (content) {
   assert(!isNullOrUndefined(content), "content must be defined");

   //content must contain a key / value pair item and only one
   var counter = 0;
   $.map(content, function(val, key) {
      counter++;
   });

   assert (counter == 1, "Can't create a conditional item with content that do not have exactly one key value pair item : " + JSON.stringify(content));

   for (var key in content) {
      switch (key.toLowerCase()) {
         case "is" :
            return new AutomationRuleOperatorIs(content[key]);
            break;

         default :
            throw Error("type " + key + " is unsupported");
            break;
      }
   }
};
