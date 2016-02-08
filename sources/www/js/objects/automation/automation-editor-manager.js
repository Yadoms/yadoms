/**
 * Created by nhilaire on 16/03/2015.
 */

/**
 * This class create editors which implements IAutomationruleEditor
 * @constructor
 */
function AutomationEditorManager(){}

/**
 * Create the list of all IAutomationEditor managed
 * @constructor
 */
AutomationEditorManager.getAll = function() {
   var d = new $.Deferred();

   AutomationInterpreterManager.getAll()
   .done(function (interpreters) {
      var editors = [];

      //to add an editor in the return list it must support at least one available interpreter

      var interpreterFound = false;
      //Code editor
      $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
         if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value.toLowerCase()])) {
            interpreterFound = true;
         }            
      });

      if (interpreterFound) {
         //we can use this editor
         editors.push(new AutomationEditorCode(interpreters));
      }

      interpreterFound = false;
      //Blockly editor
      $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
         if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value.toLowerCase()])) {
            interpreterFound = true;
         }            
      });

      if (interpreterFound) {
         //we can use this editor
         editors.push(new AutomationEditorBlockly(interpreters));
      }

      d.resolve(editors);

   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Create the list of all IAutomationEditor managed
 * @constructor
 */
AutomationEditorManager.getByName = function(name) {
   assert(!isNullOrUndefined(name), "name must be defined");
   var d = new $.Deferred();

   AutomationEditorManager.getAll()
   .done(function (editors) {
      var editorFound = false;
      $.each(editors, function (key, value) {
         if (!editorFound) {
            if (value.getName().toLowerCase() == name.toLowerCase()) {
               editorFound = value;
            }
         }
      });
      //if we reach this code, no matching editor has been found
      d.resolve(editorFound);
   })
   .fail(d.reject);

   return d.promise();
}