/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 * Obtain from the server information
 * @param callback
 * @param objectType
 * @param sync
 */
YadomsInformationManager.getList = function() {
   var d = $.Deferred();

   $.ajax({
      dataType: "json",
      url: "rest/system/information"
   })
   .done(function( data ) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.generic.errorGetting", {objectName : $.t("core.yadoms.information")}, JSON.stringify(data)));
         d.reject();
      } else {
         d.resolve(data.data);
      }
   })
   .fail(function() {
          notifyError($.t("objects.generic.errorGetting", {objectName : $.t("core.yadoms.information")}));
      d.reject();
   });

   return d.promise();
};

/**
 *
 * @constructor
 */
function YadomsInformationManager()
{
}

