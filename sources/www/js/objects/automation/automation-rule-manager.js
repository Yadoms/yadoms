/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AutomationRuleManager(){}

/**
 * Create a rule from server json data
 * @param {Object} json The rule data given by server (json format)
 * @returns {AutomationRule} 
 */
AutomationRuleManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   return new AutomationRule(json.id, json.name, json.description, json.interpreter, json.editor, json.model, json.content, json.configuration, parseBool(json.autoStart), json.state, json.errorMessage, json.startDate, json.stopDate);
};

/**
 * Create a rule to the server
 * @param {Object} rule The rule to create
 * @returns {Promise} 
 */
AutomationRuleManager.createToServer = function(rule) {
    assert(!isNullOrUndefined(rule), "rule must be defined");

   var d = new $.Deferred();

   var requestData = {
      name: rule.name,
      description: rule.description,
      interpreter: rule.interpreter,
      editor: rule.editorName,
      model: rule.model,
      content: rule.content,
      configuration: rule.configuration,
      autoStart: rule.autoStart,
      code: rule.code
   };

   RestEngine.postJson("/rest/automation/rule", { data: JSON.stringify(requestData)})
   .done(function(data) {
      //we update our information from the server
      var newRule = AutomationRuleManager.factory(data);
      //we copy other items
      newRule.code = rule.code;
      d.resolve(newRule);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Get all rules from server
 * @returns {Promise} 
 */
AutomationRuleManager.get = function () {

   var d = new $.Deferred();

   RestEngine.getJson("rest/automation/rule")
   .done(function (data) {
      var allRules = [];
      $.each(data.rules, function (index, value) {
         allRules.push(AutomationRuleManager.factory(value));
      });

      d.resolve(allRules);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Delete a rule from server
 * @param {Object} rule The rule to delete
 * @returns {Promise} 
 */
AutomationRuleManager.deleteFromServer = function (rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   return RestEngine.deleteJson("/rest/automation/rule/" + rule.id);
};

/**
 * Update a rule
 * @param {Object} rule The rule to update (with updated fields)
 * @returns {Promsie} 
 */
AutomationRuleManager.updateToServer = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   var d = new $.Deferred();

   RestEngine.putJson("/rest/automation/rule/" + rule.id, { data: JSON.stringify(rule) })
   .done(function (data) {
      //it's okay
      //we update our information from the server
      var updatedRule = AutomationRuleManager.factory(data);
      //we copy other items
      updatedRule.code = rule.code;
      d.resolve(updatedRule);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Stop a rule
 * @param {Object} rule The rule to update
 * @returns {Promise} 
 */
AutomationRuleManager.stop = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   
   var d = new $.Deferred();

   RestEngine.getJson("/rest/automation/rule/" + rule.id + "/stop")
   .done(function (data) {
      //it's okay
      //we update our information from the server
      var updatedRule = AutomationRuleManager.factory(data);
      //we copy other items
      updatedRule.code = rule.code;
      d.resolve(updatedRule);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Start a rule
 * @param {Object} rule The rule to update
 * @returns {Promise} 
 */
AutomationRuleManager.start = function (rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   var d = new $.Deferred();

   RestEngine.getJson("/rest/automation/rule/" + rule.id + "/start")
   .done(function (data) {
      //it's okay
      //we update our information from the server
      var updatedRule = AutomationRuleManager.factory(data);
      //we copy other items
      updatedRule.code = rule.code;
      d.resolve(updatedRule);
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Change autostart of a rule
 * @param {Object} rule The rule to update
 * @param {Boolean} autoStartValue The new autoStart value to set
 * @returns {Promise} 
 */
AutomationRuleManager.setAutoStart = function(rule, autoStartValue) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   rule.autoStart = autoStartValue;
   return AutomationRuleManager.updateToServer(rule, callback);
};

/**
 * Get the code of the rule
 * @param {Object} rule The rule to get the code
 * @return {Promise}
 */
AutomationRuleManager.getCode = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   var d = new $.Deferred();

   RestEngine.getJson("rest/automation/rule/" + rule.id + "/code")
   .done(function (data) {
      rule.code = data.code;
      rule.codeHasBeenDownloaded = true;
      d.resolve();
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Get the template code associated to interpreter
 * @param {Object} rule The rule to get the template code
 * @return {Promise}
 */
AutomationRuleManager.getTemplateCode = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   assert(!isNullOrUndefined(rule.interpreter), "rule interpreter must be defined");

   var d = new $.Deferred();
   RestEngine.getJson("rest/automation/rule/" + rule.interpreter + "/codeTemplate")
   .done(function (data) {
      rule.code = data.code;
      rule.codeHasBeenDownloaded = true;
      d.resolve();
   })
   .fail(d.reject);
   return d.promise();
};

/**
 *Update the rule code
 * @param {Object} rule The rule to update the code
 * @return {Promise}
 */
AutomationRuleManager.updateCode = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   
   var d = new $.Deferred();
   RestEngine.putJson("rest/automation/rule/" + rule.id + "/code", { data: JSON.stringify({ "code": rule.code }) })
   .done(function (data) {
      rule.code = data.code;
      d.resolve();
   })
   .fail(d.reject);
   return d.promise();
};

/**
 * Get the log content of a rule
 * @param {Object} rule The rule to get the log content
 * @return {Promise}
 */
AutomationRuleManager.getLog = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   return RestEngine.getJson("rest/automation/rule/" + rule.id + "/log");
};

/**
 * Delete the log content of a rule
 * @param {Object} rule The rule to delete the log content
 * @return {Promise}
 */
AutomationRuleManager.deleteLog = function(rule) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   return RestEngine.deleteJson("rest/automation/rule/" + rule.id + "/log");
};
