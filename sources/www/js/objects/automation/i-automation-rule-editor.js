/**
 * This interface describe a AutomationRuleEditor
 * @constructor
 */
function IAutomationRuleEditor() {
}

/**
 * Obtain Unique identifier of the editor
 */
IAutomationRuleEditor.prototype.getUuid = function() {
   assert(false, "getUuid() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Obtain active supported interpreters of the editor
 */
IAutomationRuleEditor.prototype.getActiveSupportedInterpreters = function() {
   assert(false, "getActiveSupportedInterpreters() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Obtain class icon for the editor. ie "fa fa-code"
 * @returns {string}
 */
IAutomationRuleEditor.prototype.getClassIcon = function() {
   assert(false, "getClassIcon() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Obtain the list of all required js files to load
 * @returns {Array{string}}
 */
IAutomationRuleEditor.prototype.getRequiredJsFiles = function() {
   return undefined; //nothing required to load
};


/**
 * Obtain name of editor
 * @returns {string}
 */
IAutomationRuleEditor.prototype.getName = function() {
   assert(false, "getName() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Obtain DOM structure to insert in editor's page
 */
IAutomationRuleEditor.prototype.getDOMStructure = function() {
   assert(false, "getDOMObject() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to execute javascript action after inserting DOM structure in the page
 */
IAutomationRuleEditor.prototype.applyScript = function() {
   assert(false, "applyScript() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to set the active rule to edit
 * @param rule
 */
IAutomationRuleEditor.prototype.setRule = function(rule) {
   assert(false, "setRule() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to fire an event when the modal is shown
 * @param rule
 */
IAutomationRuleEditor.prototype.onModalShown = function($modal) {
   assert(false, "onModalShown() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to fire an event when the modal is hidden
 * @param rule
 */
IAutomationRuleEditor.prototype.onModalHidden = function($modal) {
   assert(false, "onModalHidden() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to update the current rule with editor content
 */
IAutomationRuleEditor.prototype.updateRule = function() {
   assert(false, "updateRule() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Get the help button label (no help if empty)
 */
IAutomationRuleEditor.prototype.getHelpButtonLabel = function() {
   assert(false, "getHelpButtonLabel() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Show the help
 */
IAutomationRuleEditor.prototype.showHelp = function() {
   assert(false, "showHelp() of IAutomationRuleEditor must be implemented in derived class");
};


/**
 * Permit to change the interpreter
 * @param newInterpreter
 */
IAutomationRuleEditor.prototype.setInterpreter = function(newInterpreter) {
   assert(false, "setRule() of IAutomationRuleEditor must be implemented in derived class");
};

/**
 * Permit to the object to run a custom validator engine
 */
IAutomationRuleEditor.prototype.validate = function() {
   assert(false, "validate() of IAutomationRuleEditor must be implemented in derived class");
};
