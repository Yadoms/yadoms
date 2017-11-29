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
WidgetPackageManager.packageList = [];

WidgetPackageManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");

   var wp = new WidgetPackage();
   wp.package = json;
   wp.type = json.type;
   //we get i18n data
   i18n.options.resGetPath = '__ns__/locales/__lng__.json';
   i18n.loadNamespace("widgets/" + json.type);
   //we restore the resGetPath
   i18n.options.resGetPath = "locales/__lng__.json";

   wp.viewAnViewModelHaveBeenDownloaded = false;

   return wp;
};

WidgetPackageManager.packageList = [];

WidgetPackageManager.getAll = function () {

   var d = new $.Deferred();

   RestEngine.getJson("rest/widget/package")
      .done(function(data) {
         //we add it to the package list
         var newWidgetPackages = [];

         $.each(data.package, function(index, value) {
            try {
               newWidgetPackages.push(WidgetPackageManager.factory(value));
            } catch (err) {
               notifyError($.t("objects.widgetPackageManager.incorrectPackage"), value);
            }
         });
         
         //
         $.each(newWidgetPackages, function(index, newPackage) {
            if(WidgetPackageManager.packageList[newPackage.type]) {
               //if already exists and newer
               if(newPackage.package.version != WidgetPackageManager.packageList[newPackage.type].package.version) {
                  WidgetPackageManager.packageList[newPackage.type] = newPackage;
               } else {
                  //already exist, same version, so do nothing
               }
            } else {
               //if not exists
               WidgetPackageManager.packageList[newPackage.type] = newPackage;
            }
         });
         
         for(var i = WidgetPackageManager.packageList.length; i>0; i--) {
            if(!newWidgetPackages[ WidgetPackageManager.packageList[i].type ]) {
               WidgetPackageManager.packageList.slice(-1,1);
            }
         }

         d.resolve();
      })
      .fail(d.reject);
   return d.promise();
};

WidgetPackageManager.packageExists = function(packageName) {
   return (!isNullOrUndefined(WidgetPackageManager.packageList[packageName]));
};
