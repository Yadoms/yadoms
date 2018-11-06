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
   var d = new $.Deferred();

   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.type), "json.type must be defined");

   var wp = new WidgetPackage();
   wp.package = json;
   wp.type = json.type;
   d.resolve(wp);

   return d.promise();
};

WidgetPackageManager.loadLanguage = function(type) {
   var d = new $.Deferred();
   
   //we manage i18n
   if (!isNullOrUndefined(WidgetPackageManager.packageList[type]) && !WidgetPackageManager.packageList[type].languageHaveBeenDownloaded){
       i18nManager.loadNamespace("widgets", type).done(function(){
          WidgetPackageManager.packageList[type].languageHaveBeenDownloaded = true;
          d.resolve();
       })
       .fail(function(){
          d.reject();
       });
   }else
      d.resolve();
   return d.promise();
};

WidgetPackageManager.packageList = [];

WidgetPackageManager.getAll = function () {
   var d = new $.Deferred();
   RestEngine.getJson("rest/widget/package")
      .done(function(data) {
         //we add it to the package list
         var newWidgetPackages = [];

         var deferredArray = [];
         $.each(data.package, function(index, value) {
            var pck = WidgetPackageManager.factory(value);
            deferredArray.push(pck);
            pck.done(function(wp) {
               newWidgetPackages.push(wp);
            })
            .fail(function(f) {
               notifyError($.t("objects.widgetPackageManager.incorrectPackage"), value);
            });
         });
         
        $.when.apply($,deferredArray)
         .done(function() {
         //
         $.each(newWidgetPackages, function(index, newPackage) {
            if(WidgetPackageManager.packageList[newPackage.type]) {
               //if already exists and newer
                  if(newPackage.version != WidgetPackageManager.packageList[newPackage.type].version) {
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
         });        

      })
      .fail(d.reject);
   return d.promise();
};

WidgetPackageManager.packageExists = function(packageName) {
   return (!isNullOrUndefined(WidgetPackageManager.packageList[packageName]));
};