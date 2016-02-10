/**
 * The AutomationEditorBlockly implements IAutomationRuleEditor
 * This implementation permit to have a standard code GUI
 * @type {IAutomationRuleEditor}
 */
AutomationEditorBlockly.prototype = new IAutomationRuleEditor();
AutomationEditorBlockly.prototype.constructor = AutomationEditorCode;

AutomationEditorBlockly.getSupportedInterpreters = function() {
   return ["python", "dart", "javascript"];
};

/**
 * Ctor
 * @constructor
 */
function AutomationEditorBlockly(interpreters) {
   var self = this;
   self.uuid = createUUID();

   //we compare interpreters and getSupportedInterpreters() static method to keep only active supported interpreters
   self.activeSupportedInterpreters = [];
   $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
      if(!isNullOrUndefined(interpreters) && !isNullOrUndefined(interpreters[value.toLowerCase()])) {
         self.activeSupportedInterpreters.push(interpreters[value.toLowerCase()]);
      }
   });
}

/**
 * Obtain active supported interpreters of the editor
 */
AutomationEditorBlockly.prototype.getActiveSupportedInterpreters = function() {
   return this.activeSupportedInterpreters;
};

/**
 * Obtain Unique identifier of the editor
 */
AutomationEditorBlockly.prototype.getUuid = function() {
   return this.uuid;
};

/**
 * Obtain DOM structure to insert in editor's page
 */
AutomationEditorBlockly.prototype.getDOMStructure = function() {
   return ("<div id=\"" + this.uuid + "\"></div>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorBlockly.prototype.applyScript = function() {
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
AutomationEditorBlockly.prototype.setRule = function(rule) {
   //we add the .code and go to the end of code
   this.rule = rule;

   //the setRule function must return a promise
   var d = new $.Deferred();
   d.resolve();
   return d.promise();
};

/**
 * Permit to fire an event when the modal is shown
 * @param rule
 */
AutomationEditorBlockly.prototype.onModalShown = function($modal) {
   var self = this;
   var $mainDiv = $("div#" + this.getUuid());

   //fix input field
   $(document).off('focusin.modal');

   //load content
   var content = self.rule.content;

   //clear main div (content and css classes)
   $mainDiv.empty().removeClass();
   
   //load blockly
   Blockly.Yadoms.Initialize($mainDiv, content, 1);
};

/**
 * Permit to fire an event when the modal is hidden
 * @param rule
 */
AutomationEditorBlockly.prototype.onModalHidden = function ($modal) {
   //remove remainging divs
   var $widgetDiv = $(".blocklyWidgetDiv");
   if ($widgetDiv)
      $widgetDiv.remove();

   var $tooltipDiv = $(".blocklyTooltipDiv");
   if ($tooltipDiv)
      $tooltipDiv.remove();
};


/**
 * Permit to update the current rule with editor content
 */
AutomationEditorBlockly.prototype.updateRule = function() {
   var self = this;
   self.rule.content = "";
   self.rule.code = "";

   Blockly.Yadoms.GetResult(self.rule.interpreter.toLowerCase(), function(xmlString,code){
      self.rule.content = xmlString;
      self.rule.code = code;

      console.log("------------------> START");
      console.log(code);
      console.log("------------------> END");
   });

};

/**
 * Permit to the object to run a custom validator engine
 */
AutomationEditorBlockly.prototype.validate = function() {
   var result = false;
   Blockly.Validation.validateMainWorkspace(function(isValid, msg) {
      if(!isValid) {
         console.warn("Fail to validate blockly : " + msg);
      }
      result = isValid;
   });
   return result;
};

/**
 * Permit to change the interpreter
 * @param newInterpreter
 */
AutomationEditorBlockly.prototype.setInterpreter = function(newInterpreter) {
   var found = false;
   $.each(AutomationEditorBlockly.getSupportedInterpreters(), function (key, value) {
      if (value == newInterpreter)
         found = true;
   });

   if (found) {
      this.rule.interpreter = newInterpreter;
   }
};

/**
 * Obtain class icon for the editor. ie "fa fa-code"
 * @returns {string}
 */
AutomationEditorBlockly.prototype.getClassIcon = function() {
   return "fa fa-puzzle-piece";
};

/**
 * Obtain name of editor
 * @returns {string}
 */
AutomationEditorBlockly.prototype.getName = function() {
   return "blockly";
};
