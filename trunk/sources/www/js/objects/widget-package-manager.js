/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetPackageManager(){}

/**
 * Contains all Ctor of all loaded viewModels
 * @type {Array}
 */
WidgetPackageManager.widgetPackages = [];

WidgetPackageManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");

   var wp = new WidgetPackage();
   wp.packageInformation = json;
   return wp;
};

WidgetPackageManager.getAll = function(callback) {
   $.getJSON("rest/widget/package")
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.widgetPackageManager.errorDuringGettingPackages"), JSON.stringify(data));
            return;
         }
         //we add it to the package list
         WidgetPackageManager.widgetPackages = [];

         $.each(data.data.package, function(index, value) {
            WidgetPackageManager.widgetPackages[value.name] = WidgetPackageManager.factory(value);
         });
         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.widgetPackageManager.errorDuringGettingPackages"));});
}

WidgetPackageManager.packageExists = function(packageName) {
   return (!isNullOrUndefined(WidgetPackageManager.widgetPackages[packageName]));
}
