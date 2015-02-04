/**
 * Created by jeanmichelDECORET on 07/01/2015.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function RecipientManager(){}

RecipientManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.firstName), "json.firstName must be defined");
   assert(!isNullOrUndefined(json.lastName), "json.lastName must be defined");
   assert(!isNullOrUndefined(json.fields), "json.fields must be defined");
   return new Recipient(json.id, decodeURIComponent(json.firstName), decodeURIComponent(json.lastName), json.fields);
};

RecipientManager.get = function (recipientId, callback, sync) {
   assert(!isNullOrUndefined(recipientId), "recipientId must be defined");
   assert($.isFunction(callback), "callback must be a function");

    var async = true;
    if (!isNullOrUndefined(sync))
        async = sync;

    $.ajax({
        type: "GET",
        url: "/rest/recipient/" + + recipientId,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async : async
    })
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "Recipient with Id = " + recipientId}), JSON.stringify(data));
            return;
         }
         callback(RecipientManager.factory(data.data));
      })
      .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "Recipient with Id = " + recipientId}));});
}


RecipientManager.getAllFields = function (callback, sync) {

   var async = true;
   if (!isNullOrUndefined(sync))
      async = sync;
   PluginManager.getWithPackage(callback, async);
};

RecipientManager.list = function(callback, sync) {
   assert($.isFunction(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync))
        async = sync;

   //we start by requesting all pugin fields
   RecipientManager.getAllFields( function(allPlugin) {

       $.ajax({
           type: "GET",
           url: "/rest/recipient",
           contentType: "application/json; charset=utf-8",
           dataType: "json",
           async : async
       })
           .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                   notifyError($.t("objects.recipient.errorGettingList"), JSON.stringify(data));
                   return;
               }

               var recipientList = [];
               $.each(data.data.recipient, function(index, value) {
                   var r2 = RecipientManager.factory(value);
                   r2.mergeFields(allPlugin);
                   recipientList.push(r2);
               });

               if ($.isFunction(callback))
                   callback(recipientList);
           })
           .fail(function() {
               notifyError($.t("modals.recipient.errorGettingList"));
               callback(null);
           });

   }, sync);

}


RecipientManager.deleteFromServer = function(recip, callback) {
   assert(!isNullOrUndefined(recip), "recipient must be defined");
   $.ajax({
      type: "DELETE",
      url: "/rest/recipient/" + recip.id,
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorDeleting", {objectName : recip.firstName + " " + recip.lastName}), JSON.stringify(data));
            return;
         }

         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.generic.errorDeleting", {objectName : recip.firstName + " " + recip.lastName}));});
};

RecipientManager.updateToServer = function(recip, callback) {
   assert(!isNullOrUndefined(recip), "recipient must be defined");
   $.ajax({
      type: "PUT",
      url: "/rest/recipient/" + recip.id,
      data: JSON.stringify(recip),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorUpdating", {objectName : recip.firstName + " " + recip.lastName}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }
         //it's okay
         //we update our information from the server
          recip = RecipientManager.factory(data.data);

         //we call the callback with true as a ok result
         if ($.isFunction(callback))
            callback(true, recip);
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorUpdating", {objectName : recip.firstName + " " + recip.lastName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};


RecipientManager.addToServer = function(recip, callback) {
   assert(!isNullOrUndefined(recip), "recipient must be defined");
   $.ajax({
      type: "POST",
      url: "/rest/recipient",
      data: JSON.stringify(recip),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
       .done(function(data) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.generic.errorUpdating", {objectName : recip.firstName + " " + recip.lastName}), JSON.stringify(data));
             //launch callback with false as ko result
             if ($.isFunction(callback))
                callback(false);
             return;
          }
          //it's okay
          //we update our information from the server
          recip = RecipientManager.factory(data.data);

          //we call the callback with true as a ok result
          if ($.isFunction(callback))
             callback(true, recip);
       })
       .fail(function() {
          notifyError($.t("objects.generic.errorUpdating", {objectName : recip.firstName + " " + recip.lastName}));
          //launch callback with false as ko result
          if ($.isFunction(callback))
             callback(false);
       });
};