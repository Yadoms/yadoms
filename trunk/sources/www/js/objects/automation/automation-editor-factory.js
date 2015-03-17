/**
 * Created by nhilaire on 16/03/2015.
 */

/**
 * This class create editors which implements IAutomationruleEditor
 * @constructor
 */
function AutomationEditorFactory(){}

/**
 * Create the list of all IAutomationEditor managed
 * @constructor
 */
AutomationEditorFactory.createList = function(interpreterList, callback) {
   assert(!isNullOrUndefined(interpreterList), "interpreterList must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var editors = [];

   //to add an editor in the return list it must support at least one available interpreter

   var interpreterFound = false;
   //Code editor
   $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
      $.each(interpreterList, function (inputKey, inputValue) {
         if (value.toLowerCase() ==  inputValue.name.toLowerCase())
            interpreterFound = true;
      });
   });

   if (interpreterFound) {
      //we can use this editor
      editors.push(new AutomationEditorCode(interpreterList));
   }

   interpreterFound = false;
   //Blockly editor
   $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
      $.each(interpreterList, function (inputKey, inputValue) {
         if (value.toLowerCase() ==  inputValue.name.toLowerCase())
            interpreterFound = true;
      });
   });

   if (interpreterFound) {
      //we can use this editor
      editors.push(new AutomationEditorBlockly(interpreterList));
   }

   callback(editors);
};