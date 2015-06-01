/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function UpdateInformation(name, author, description, releaseType, url, credits, version, downloadUrl, iconUrl) {
   assert(!isNullOrUndefined(name), "name of a plugin must be defined");
   assert(!isNullOrUndefined(author), "author of a plugin must be defined");
   assert(!isNullOrUndefined(description), "description of plugin");
   assert(!isNullOrUndefined(releaseType), "release type must be defined");
   assert(!isNullOrUndefined(version), "version must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   assert(!isNullOrUndefined(iconUrl), "iconUrl must be defined");

   this.name = name;
   this.author = author;
   this.description = description;
   this.releaseType = releaseType;
   this.url = url;
   this.credits = credits;
   this.version = version;
   this.downloadUrl = downloadUrl;
   this.iconUrl = iconUrl;
}

Plugin.prototype.downloadPackage= function (callback, sync) {
   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

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
};