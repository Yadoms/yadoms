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

      //Blockly editor
      var interpreterFound = false;
      $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
         if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value])) {
            interpreterFound = true;
         }            
      });

      if (interpreterFound) {
         //we can use this editor
         editors.push(new AutomationEditorBlockly(interpreters));
      }

      //Code editor
      interpreterFound = false;
      $.each(AutomationEditorCode.getSupportedInterpreters(), function (key, value) {
         if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value])) {
            interpreterFound = true;
         }            
      });

      if (interpreterFound) {
         //we can use this editor
         editors.push(new AutomationEditorCode(interpreters));
      }

      d.resolve(editors);

   })
   .fail(d.reject);

   return d.promise();
};


AutomationEditorManager.loadRequiredFile_ = function(editor) {
  var d = new $.Deferred();
  if(editor) {
	 var toLoad = editor.getRequiredJsFiles();
	 if(toLoad) {
		asyncLoadJSLibs(toLoad)
		   .done(d.resolve)
		   .fail(d.reject);
	 }
	 else {
		d.resolve();
	 }         
  } else {
	 d.resolve();
  }
  return d.promise();
}

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
            if (value.getName() == name) {
               editorFound = value;
			   AutomationEditorManager.loadRequiredFile_(value)
			   .done(function() {
				   d.resolve(value);
			   })
			   .fail(d.reject);
            }
         }
      });
	  
	  if (!editorFound) {
		 d.reject();
	  }
   })
   .fail(d.reject);

   return d.promise();
}