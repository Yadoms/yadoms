/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 * Obtain from the server the available plugin list from the yadoms.com web site
 * @param callback
 * @param objectType
 * @param sync
 */
YadomsUpdateInformationManager.getList = function() {
   var d = $.Deferred();

   $.ajax({
      dataType: "json",
      url: "rest/update/yadoms/list/" + i18n.lng()
   })
   .done(function( data ) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.update-information.errorListing"), JSON.stringify(data));
         d.reject();
      } else {
         d.resolve(data.data);
      }
   })
   .fail(function() {
      notifyError($.t("objects.update-information.errorListing"));
      d.reject();
   });

   return d.promise();
};

YadomsUpdateInformationManager.update = function(yadomsUpdateInformation) {
   assert(!isNullOrUndefined(yadomsUpdateInformation), "yadomsUpdateInformation must be defined");

   var d = $.Deferred();

   $.ajax({
      dataType: "json",
      url: "rest/update/yadoms/update/",
      data: JSON.stringify({ "versionData": yadomsUpdateInformation }),
      type: "POST",
      contentType: "application/json; charset=utf-8"
   })
       .done(function( data ) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorUpdating", {objectName : "yadoms"}), JSON.stringify(data));
             d.reject();
          } else {
             d.resolve(data.data.taskId);
          }
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorUpdating", {objectName : "yadoms"}));
          d.reject();
       });

   return d.promise();
};

/**
 * Compare the two UpdateInformationObjects. Return > 0 if item1 is lower, =0 if the same and <0 if item1 is higher
  */
YadomsUpdateInformationManager.compareVersion = function(item1, item2) {
   return item2.version - item1.version;
};

/**
 *
 * @constructor
 */
function YadomsUpdateInformationManager()
{
}

