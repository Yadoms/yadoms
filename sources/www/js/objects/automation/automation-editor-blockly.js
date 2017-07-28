/**
 * The AutomationEditorBlockly implements IAutomationRuleEditor
 * This implementation permit to have a standard code GUI
 * @type {IAutomationRuleEditor}
 */
AutomationEditorBlockly.prototype = new IAutomationRuleEditor();
AutomationEditorBlockly.prototype.constructor = AutomationEditorCode;

AutomationEditorBlockly.getSupportedInterpreters = function() {
   return ["ypython27", "dart", "javascript"];
};

/**
 * Ctor
 * @constructor
 */
function AutomationEditorBlockly(interpreters) {
   var self = this;
   self.uuid = createUUID();
   self.helpUuid = createUUID();
   
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
 * Obtain the list of all required js files to load
 * @returns {Array{string}}
 */
AutomationEditorBlockly.prototype.getRequiredJsFiles = function() {
   return [ "libs/blockly/js/blockly_compressed.js",
            "libs/blockly/js/blocks_compressed.js",
            "libs/blockly/js/python_compressed.js",
            "js/objects/automation/blockly/common.js",
            "js/objects/automation/blockly/data.js",
            "js/objects/automation/blockly/python.js",
            "js/objects/automation/blockly/toolbox.js",
            "js/objects/automation/blockly/validation.js",
            "js/objects/automation/blockly/customBlocks/enumeration.js",
            "js/objects/automation/blockly/customBlocks/get-info.js",
            "js/objects/automation/blockly/customBlocks/helper.js",
            "js/objects/automation/blockly/customBlocks/infinite-loop.js",
            "js/objects/automation/blockly/customBlocks/keyword-value-get.js",
            "js/objects/automation/blockly/customBlocks/keyword-value-set.js",
            "js/objects/automation/blockly/customBlocks/log.js",
            "js/objects/automation/blockly/customBlocks/not.js",
            "js/objects/automation/blockly/customBlocks/notification-simple.js",
            "js/objects/automation/blockly/customBlocks/sleep.js",
            "js/objects/automation/blockly/customBlocks/wait-for-event.js",
            "js/objects/automation/blockly/customBlocks/wait-for-event-result.js",
            "js/objects/automation/blockly/customBlocks/dates/helper.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_arithmetic.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_date.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_datetime.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_everyDay.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_extract.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_time.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_timespan.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_weekDay.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_today.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_now.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_sunrise.js",
            "js/objects/automation/blockly/customBlocks/dates/yadoms_date_sunset.js",
            "js/objects/automation/blockly/customBlocks/generator/python/enumeration.js",
            "js/objects/automation/blockly/customBlocks/generator/python/get-info.js",
            "js/objects/automation/blockly/customBlocks/generator/python/helper.js",
            "js/objects/automation/blockly/customBlocks/generator/python/infinite-loop.js",
            "js/objects/automation/blockly/customBlocks/generator/python/keyword-value-get.js",
            "js/objects/automation/blockly/customBlocks/generator/python/keyword-value-set.js",
            "js/objects/automation/blockly/customBlocks/generator/python/log.js",
            "js/objects/automation/blockly/customBlocks/generator/python/not.js",
            "js/objects/automation/blockly/customBlocks/generator/python/notification-simple.js",
            "js/objects/automation/blockly/customBlocks/generator/python/sleep.js",
            "js/objects/automation/blockly/customBlocks/generator/python/wait-for-event.js",
            "js/objects/automation/blockly/customBlocks/generator/python/wait-for-event-result.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_arithmetic.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_date.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_datetime.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_everyDay.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_extract.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_time.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_timespan.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_weekDay.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_today.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_now.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_sunrise.js",
            "js/objects/automation/blockly/customBlocks/generator/python/dates/yadoms_date_sunset.js"
   ];
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
   return ("<div id=\"" + this.uuid + "\"></div><button type=\"button\" class=\"btn btn-info btn-sm code-help-button\" id=\"" + this.helpUuid + "\" data-i18n=\"modals.edit-automation-rule.show-help-blockly\">doc</button>");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
AutomationEditorBlockly.prototype.applyScript = function() {
   var self = this;
	//Bind help
   $("#" + self.helpUuid).unbind('click').bind('click', function () {
       self.showDoc();
   });
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
AutomationEditorBlockly.prototype.onModalShown = function() {
   var self = this;
   var $mainDiv = $("div#" + this.getUuid());

   //fix input field
   $(document).off('focusin.modal');

   //load content
   var content = decodeURIComponent(self.rule.content);

   //clear main div (content and css classes)
   $mainDiv.empty().removeClass();
   
   //load blockly
   Blockly.Yadoms.Initialize($mainDiv, content, 1);
};

/**
 * Permit to fire an event when the modal is hidden
 * @param rule
 */
AutomationEditorBlockly.prototype.onModalHidden = function () {
   Blockly.WidgetDiv.hide();
   Blockly.Tooltip.hide();
};


/**
 * Permit to update the current rule with editor content
 */
AutomationEditorBlockly.prototype.updateRule = function() {
   var self = this;
   self.rule.content = "";
   self.rule.code = "";

   Blockly.Yadoms.GetResult(self.rule.interpreter.toLowerCase(), function(xmlString,code){
      self.rule.content = encodeURIComponent(xmlString);
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
      if (value === newInterpreter)
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

/**
 * Show the API documentation
 */
AutomationEditorBlockly.prototype.showDoc = function () {
    var url = "help.html?product=blockly";
    url += "&lang=" + Yadoms.systemConfiguration[ConfigurationManager.items.system.language].value;
    window.open(url);
};

