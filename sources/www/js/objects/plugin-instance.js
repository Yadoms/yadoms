/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, displayName, type, configuration, autoStart, category) {
    //id is optional because null is used when creating new instance
    assert(!isNullOrUndefined(displayName), "displayName of a pluginInstance must be defined");
    assert(!isNullOrUndefined(type), "type of a pluginInstance must be defined");
    assert(!isNullOrUndefined(configuration), "configuration of a pluginInstance must be defined");
    assert(!isNullOrUndefined(autoStart), "autoStart of a pluginInstance must be defined");
    assert(!isNullOrUndefined(category), "category of a pluginInstance must be defined");

    this.id = id;
    this.displayName = displayName;
    this.type = type;
    this.configuration = configuration;
    this.autoStart = autoStart;
    this.lastState = null;
	this.lastMessageId = null;
    this.category = category;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, displayName: *, type: *, configuration: *, autoStart: *}}
 */
PluginInstance.prototype.toJSON = function () {
   return {
      id : this.id,
      displayName: this.displayName,
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
   var d = new $.Deferred();

   if (!isNullOrUndefined(this.package)) {
      if (this.package.configurationSchema && Object.keys(this.package.configurationSchema).length > 0) {
         var tmp = this.package.configurationSchema;
         this.applyBindingPrivate(tmp, ["system"])
            .done(d.resolve)
            .fail(d.reject);
      } else {
         //if configurationSchema is not defined, to not try to do any binding...
         //just resolve with undefined configurationSchema
         d.resolve();
      }
   } else {
      d.reject("undefined package");
   }
   return d.promise();
};

/**
 *  Get the bound manually device creation configuration schema
 * @returns {*}
 */
PluginInstance.prototype.getBoundManuallyDeviceCreationConfigurationSchema = function () {
   var d = new $.Deferred();

   if (!isNullOrUndefined(this.package)) {
      if (this.package.manuallyDeviceCreationConfigurationSchema && Object.keys(this.package.manuallyDeviceCreationConfigurationSchema).length > 0) {
         var tmp = this.package.manuallyDeviceCreationConfigurationSchema;
         this.applyBindingPrivate(tmp, ["plugin", "system"])
         .done(d.resolve)
         .fail(d.reject);
      } else {
         //if manuallyDeviceCreationConfigurationSchema is not defined, to not try to do any binding...
         //just resolve with undefined manuallyDeviceCreationConfigurationSchema
         d.resolve();
      }
   } else {
      d.reject("undefined package");
   }
   return d.promise();
};


/**
 *  Get the bound extra command configuration schema
 * @returns {*}
 */
PluginInstance.prototype.getBoundExtraCommand = function () {
   var d = new $.Deferred();

   if (!isNullOrUndefined(this.package)) {
      if (this.package.extraCommands && Object.keys(this.package.extraCommands).length > 0) {
         var tmp = this.package.extraCommands;
         this.applyBindingPrivate(tmp, ["plugin", "system"])
            .done(d.resolve)
            .fail(d.reject);
      } else {
         //if extra commands are not defined, to not try to do any binding...
         //just resolve with undefined extraCommands
         d.resolve();
      }
   } else {
      d.reject("undefined package");
   }
   return d.promise();
};


/**
 * Tells if this instance contains extra commands
 * @returns {Boolean}
 */
PluginInstance.prototype.containsExtraCommand = function () {
   return (this.package && this.package.extraCommands && Object.keys(this.package.extraCommands).length > 0);
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

   var d = new $.Deferred();

   var arrayOfDeffered = [];

   $.each(item, function(key, confItem) {
      if ($.isPlainObject(confItem)) {
         if (!isNullOrUndefined(confItem.__Binding__)) {
            //we've got binding
            assert(!isNullOrUndefined(confItem.__Binding__.type), "type must be defined in binding");
            //is the binding type is allowed

            assert($.inArray(confItem.__Binding__.type.toLowerCase(), allowedTypes) != -1, "Binding of type " + confItem.__Binding__.type + " is not allowed here");
            assert(!isNullOrUndefined(confItem.__Binding__.query), "query must be defined in binding");

            switch (confItem.__Binding__.type.toLowerCase()) {
            case "system":
               //we ask synchronously the bound value
               var deffered = RestEngine.getJson("/rest/system/binding/" + confItem.__Binding__.query);
               arrayOfDeffered.push(deffered);
               deffered.done(function(data) {
                     //we replace the binded value by the result
                  item[key] = data;
                  })
                  .fail(function(error) {
                     notifyError($.t("objects.pluginInstance.errorApplyingBinding"), error);
                  });
               break;
            case "plugin":
               //we ask synchronously the binded value
               var defferedPlugin = RestEngine.getJson("/rest/plugin/" + self.id + "/binding/" + confItem.__Binding__.query);
               arrayOfDeffered.push(defferedPlugin);
               defferedPlugin.done(function(data) {
                     //we replace the binded value by the result
                  item[key] = data;
                  })
                  .fail(function(error) {
                     notifyError($.t("objects.pluginInstance.errorApplyingBinding"), error);
                  });
               break;
            }
         } else {
            var innerDeferred = self.applyBindingPrivate(confItem, allowedTypes);
            arrayOfDeffered.push(innerDeferred);
            innerDeferred.done(function(data) {
               confItem = data;
            });
         }
      }
   });

   if (arrayOfDeffered.length > 0) {
      $.whenAll(arrayOfDeffered).done(function() {
         d.resolve(item);
      });
   } else {
      d.resolve(item);
   }

   return d.promise();
};