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

RecipientManager.get = function (recipientId, callback) {
   assert(!isNullOrUndefined(recipientId), "recipientId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   $.getJSON("rest/recipient/" + recipientId)
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


RecipientManager.list = function(callback) {
   assert($.isFunction(callback), "callback must be defined");

   $.getJSON("rest/recipient")
       .done(function(data) {
          //we parse the json answer
          if (data.result != "true")
          {
             notifyError($.t("objects.recipient.errorGettingList"), JSON.stringify(data));
             return;
          }
          var recipientList = [];

          //foreach result we append a <tr>
          $.each(data.data.recipient, function(index, value) {
             var recip = RecipientManager.factory(value);
             recipientList.push(recip);
          });

          callback(recipientList);
       })
          .fail(function() {
          notifyError($.t("modals.recipient.errorGettingList"));
          callback(null);
       });
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
            callback(true);
      })
      .fail(function() {
         notifyError($.t("objects.generic.errorUpdating", {objectName : recip.firstName + " " + recip.lastName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};