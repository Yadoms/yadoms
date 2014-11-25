/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, name, type, configuration, autoStart, category) {
   assert(!isNullOrUndefined(name), "name of a pluginInstance must be defined");
   assert(!isNullOrUndefined(type), "type of a pluginInstance must be defined");
   assert(!isNullOrUndefined(configuration), "configuration of a pluginInstance must be defined");
   assert(!isNullOrUndefined(autoStart), "autoStart of a pluginInstance must be defined");
   assert(!isNullOrUndefined(category), "category of a pluginInstance must be defined");

   this.id = id;
   this.name = name;
   this.type = type;
   this.configuration = configuration;
   this.autoStart = autoStart;
   this.lastRunningStatus = null;
   this.category = category;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, name: *, type: *, configuration: *, autoStart: *}}
 */
PluginInstance.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      type: this.type,
      configuration: this.configuration,
      autoStart: this.autoStart,
      category: this.category
   };
};

PluginInstance.prototype.getBindedPackageConfigurationSchema = function() {
   if (!isNullOrUndefined(this.package)) {
      var tmp = this.package.configurationSchema;
      return this.applyBindingPrivate(tmp);
   }
}

PluginInstance.prototype.getBindedManuallyDeviceCreationConfigurationSchema = function() {
   if (!isNullOrUndefined(this.package)) {
      var tmp = this.package.manuallyDeviceCreationConfigurationSchema;
      return this.applyBindingPrivate(tmp);
   }
}

PluginInstance.prototype.applyBindingPrivate = function(item) {
   var self = this;
   for (var key in item) {
      console.log("parcours de " + key);

      if ($.isPlainObject(item[key])) {
         if (!isNullOrUndefined(item[key].__Binding__)) {
            //we've got binding
            assert(!isNullOrUndefined(item[key].__Binding__.type), "type must be defined in binding");
            assert((item[key].__Binding__.type.toLowerCase() == "system") || (item[key].__Binding__.type.toLowerCase() == "plugin") , "type must be defined to 'system' or 'plugin' in binding");
            assert(!isNullOrUndefined(item[key].__Binding__.query), "query must be defined in binding");

            var destList = {};

            switch (item[key].__Binding__.type.toLowerCase())
            {
               case "system":

                  //we ask synchronously the binded value
                  $.ajax({
                     dataType: "json",
                     url: "/rest/system/binding/" + item[key].__Binding__.query,
                     async: false
                  })
                  .done(function(data) {
                     //we parse the json answer
                     if (data.result != "true")
                     {
                        notifyError($.t("objects.pluginInstance.errorApplyingBinding"), JSON.stringify(data));
                        return;
                     }

                     //we replace the binded value by the result
                     item[key] = data.data;
                  })
                  .fail(function() {notifyError($.t("objects.pluginInstance.errorApplyingBinding"));});
                  break;
               case "plugin":
                  //we ask synchronously the binded value
                  $.ajax({
                     dataType: "json",
                     url: "/rest/plugin/" + self.id + "/binding/" + item[key].__Binding__.query,
                     async: false
                  })
                  .done(function(data) {
                     //we parse the json answer
                     if (data.result != "true")
                     {
                        notifyError($.t("objects.pluginInstance.errorApplyingBinding"), JSON.stringify(data));
                        return;
                     }

                     //we replace the binded value by the result
                     item[key] = data.data;
                  })
                  .fail(function() {notifyError($.t("objects.pluginInstance.errorApplyingBinding"));});
                  break;
            }
         }
         else {
            item[key] = this.applyBindingPrivate(item[key]);
         }
      }
   }
   return item;
}