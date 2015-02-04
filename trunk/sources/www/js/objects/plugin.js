/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function Plugin(name, author, description, nameInformation, identity, releaseType, url, version, supportManuallyCreatedDevice) {
   assert(!isNullOrUndefined(name), "name of a plugin must be defined");
   assert(!isNullOrUndefined(author), "author of a plugin must be defined");
   assert(!isNullOrUndefined(description), "description of plugin");
   assert(!isNullOrUndefined(nameInformation), "Full name of the plugin must be defined");
   assert(!isNullOrUndefined(identity), "Identity must be defined");
   assert(!isNullOrUndefined(releaseType), "release type must be defined");
   assert(!isNullOrUndefined(url), "url must be defined");
   assert(!isNullOrUndefined(version), "version must be defined");
   assert(!isNullOrUndefined(supportManuallyCreatedDevice), "supportManuallyCreatedDevice must be defined");

   this.name = name;
   this.author = author;
   this.description = description;
   this.nameInformation = nameInformation;
   this.identity = identity;
   this.releaseType = releaseType;
   this.url = url;
   this.version = version;
   this.supportManuallyCreatedDevice = supportManuallyCreatedDevice;
}


Plugin.prototype.downloadPackage= function (callback, sync) {
   var async = true;
   if (!isNullOrUndefined(sync))
      async = sync;

   var self = this;

   $.ajax({
      dataType: "json",
      url: "plugins/" + this.name + "/package.json",
      async: async
   })
       .done(function (data) {

          self.package = data;

          //we manage i18n
          i18n.options.resGetPath = 'plugins/__ns__/locales/__lng__.json';
          i18n.loadNamespace(self.name);

          //we restore the resGetPath
          i18n.options.resGetPath = "locales/__lng__.json";

          if ($.isFunction(callback))
             callback();

       })
       .fail(function () {
          notifyError($.t("objects.pluginInstance.errorGettingPackage", {pluginName: self.name}));
       });
};

Plugin.prototype.getRecipientFields = function() {

   if (!isNullOrUndefined(this.package)) {
      if (!isNullOrUndefined(this.package.recipientFields)) {
         return this.package.recipientFields;
      }
   }
}