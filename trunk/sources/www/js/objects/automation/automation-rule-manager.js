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
   return new AutomationRule(json.id, decodeURIComponent(json.name), decodeURIComponent(json.description), json.type, json.model, json.content, json.configuration, json.enabled, decodeURIComponent(json.code));
};

AutomationRuleManager.createToServer = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");

   $.ajax({
      type: "POST",
      url: "/rest/automation/rule",
      data: JSON.stringify({  name: encodeURIComponent(rule.name),
                              description: encodeURIComponent(rule.description),
                              type: rule.type,
                              model: rule.model,
                              content: rule.content,
                              configuration: rule.configuration,
                              enabled: rule.enabled,
                              code : encodeURIComponent(rule.code)
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
         rule = AutomationRuleManager.factory(data.data);

         if ($.isFunction(callback))
            callback(true);
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
   //TODO : test
   //debugger;

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
   //TODO : test
   debugger;

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
   //TODO : test
   debugger;

   $.ajax({
      type: "DELETE",
      url: "/rest/automation-rule/" + rule.id,
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
   //TODO : test
   debugger;

   $.ajax({
      type: "PUT",
      url: "/rest/automation-rule/" + rule.id,
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
         rule = DeviceManager.factory(data.data);

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

AutomationRuleManager.disable = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   //TODO : test
   debugger;
   rule.enabled = false;
   AutomationRuleManager.updateToServer(rule, callback);
};

AutomationRuleManager.enable = function(rule, callback) {
   assert(!isNullOrUndefined(rule), "rule must be defined");
   //TODO : test
   debugger;
   rule.enabled = true;
   AutomationRuleManager.updateToServer(rule, callback);
};