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

/**
 * Tells if current instance is system category
 * @returns {boolean}
 */
PluginInstance.prototype.isSystemCategory = function() {
    return PluginInstanceManager.isSystemCategory(this);
};

/**
 * Get the bound package configuration schema
 * @returns {*} The configuration schema or undefined
 */
PluginInstance.prototype.getBoundPackageConfigurationSchema = function() {
   if (!isNullOrUndefined(this.package)) {
      var tmp = this.package.configurationSchema;
      return this.applyBindingPrivate(tmp, ["system"]);
   }
   return undefined;
};

/**
 *  Get the bound manually device creation configuration schema
 * @returns {*}
 */
PluginInstance.prototype.getBoundManuallyDeviceCreationConfigurationSchema = function() {
   if (!isNullOrUndefined(this.package)) {
      var tmp = this.package.manuallyDeviceCreationConfigurationSchema;
      return this.applyBindingPrivate(tmp, ["plugin", "system"]);
   }
   return undefined;
};

/**
 * Apply binding
 * @param item The configuration item
 * @param allowedTypes Allowed types for this item
 * @returns {*}
 */
PluginInstance.prototype.applyBindingPrivate = function(item, allowedTypes) {
   assert(!isNullOrUndefined(item), "item must be defined");
   assert(!isNullOrUndefined(allowedTypes), "allowedTypes must be defined");
   var self = this;
   for (var key in item) {
      if ($.isPlainObject(item[key])) {
         if (!isNullOrUndefined(item[key].__Binding__)) {
            //we've got binding
            assert(!isNullOrUndefined(item[key].__Binding__.type), "type must be defined in binding");
            //is the binding type is allowed

            assert ($.inArray(item[key].__Binding__.type.toLowerCase(), allowedTypes) != -1, "Binding of type " + item[key].__Binding__.type + " is not allowed here");
            assert(!isNullOrUndefined(item[key].__Binding__.query), "query must be defined in binding");

            var destList = {};

            switch (item[key].__Binding__.type.toLowerCase())
            {
               case "system":

                  //we ask synchronously the bound value
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
            item[key] = this.applyBindingPrivate(item[key], allowedTypes);
         }
      }
   }
   return item;
};