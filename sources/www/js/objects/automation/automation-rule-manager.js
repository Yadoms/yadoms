/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function AutomationRuleManager(){}

AutomationRuleManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   return new AutomationRule(json.id, decodeURIComponent(json.name), decodeURIComponent(json.description), json.interpreter, json.editor, json.model, json.content, json.configuration, parseBool(json.enabled), json.state, json.errorMessage, json.startDate, json.stopDate);
};

AutomationRuleManager.createToServer = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   $.ajax({
      type: "POST",
      url: "/rest/automation/rule",
      data: JSON.stringify({  name: encodeURIComponent(rule.name),
                              description: encodeURIComponent(rule.description),
                              interpreter: rule.interpreter,
                              editor: rule.editorName,
                              model: rule.model,
                              content: rule.content,
                              configuration: rule.configuration,
                              enabled: rule.enabled,
                              code: rule.code
                           }),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorCreating", {objectName : rule.name}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }

         //we update our information from the server
         var newRule = AutomationRuleManager.factory(data.data);
         //we copy other items
         newRule.code = rule.code;
         if ($.isFunction(callback))
            callback(newRule);
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorCreating", {objectName : rule.name}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};

AutomationRuleManager.get = function (callback) {
   assert($.isFunction(callback), "callback must be a function");
   $.getJSON("rest/automation/rule")
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "automation rules"}), JSON.stringify(data));
            return;
         }

           var allRules = [];
           $.each(data.data.rules, function(index, value) {
               allRules.push(AutomationRuleManager.factory(value));
           });

         callback(allRules);
      })
      .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "automation rules"}));});
};

AutomationRuleManager.getOne = function (ruleId, callback) {
   assert(!isNullOrUndefined(ruleId), "ruleId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   $.getJSON("rest/automation/rule/" + ruleId)
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "automation rules"}), JSON.stringify(data));
            return;
         }
         callback(AutomationRuleManager.factory(data.data));
      })
      .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "automation rules"}));});
}

AutomationRuleManager.deleteFromServer = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   $.ajax({
      type: "DELETE",
      url: "/rest/automation/rule/" + rule.id,
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorDeleting", {objectName : rule.name}), JSON.stringify(data));
            return;
         }

         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.generic.errorDeleting", {objectName : rule.name}));});
};

AutomationRuleManager.updateToServer = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   $.ajax({
      type: "PUT",
      url: "/rest/automation/rule/" + rule.id,
      data: JSON.stringify(rule),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorUpdating", {objectName : rule.name}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }
         //it's okay
         //we update our information from the server
         var newRule = AutomationRuleManager.factory(data.data);
         //we copy other items
         newRule.code = rule.code;
         if ($.isFunction(callback))
            callback(newRule);

      })
      .fail(function() {
         notifyError($.t("objects.generic.errorUpdating", {objectName : rule.name}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};

AutomationRuleManager.disable = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   rule.enabled = false;
   AutomationRuleManager.updateToServer(rule, callback);
};

AutomationRuleManager.enable = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   rule.enabled = true;
   AutomationRuleManager.updateToServer(rule, callback);
};

/**
 * Obtain the code of the rule
 * @param rule
 * @param callback
 * @param sync
 */
AutomationRuleManager.getCode = function(rule, callback, sync) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/automation/rule/" + rule.id + "/code",
      async: async
   })
       .done(function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorGetting", {objectName : "automation code rule"}), JSON.stringify(data));
             return;
          }

          rule.code = decodeURIComponent(data.data.code);
          rule.codeHasBeenDownloaded = true;

          callback();
       })
       .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "automation code rule"}));});
};

/**
 * Update the code of the rule
 * @param rule
 * @param code
 * @param callback
 * @param sync
 */
AutomationRuleManager.updateCode = function(rule, callback, sync) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      type: "PUT",
      url: "rest/automation/rule/" + rule.id + "/code",
      data: JSON.stringify({"code" : encodeURIComponent(rule.code)}),
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      async: async
   })
       .done(function(data) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorUpdating", {objectName : rule.name}), JSON.stringify(data));
             //launch callback with false as ko result
             if ($.isFunction(callback))
                callback(false);
             return;
          }
          //it's okay

          rule.code = decodeURIComponent(data.data.code);
          //we call the callback with true as a ok result
          if ($.isFunction(callback))
             callback(true);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorUpdating", {objectName : rule.name}));
          //launch callback with false as ko result
          if ($.isFunction(callback))
             callback(false);
       });
};