/**
 * Created by jeanmichelDECORET on 07/01/2015.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function RecipientManager() {
}

/**
 * Create a new recipient (from server data only)
 * @param json Json data from server
 * @returns {Recipient} a recipient object
 */
RecipientManager.factory = function (json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.firstName), "json.firstName must be defined");
   assert(!isNullOrUndefined(json.lastName), "json.lastName must be defined");
   assert(!isNullOrUndefined(json.fields), "json.fields must be defined");

   var d = $.Deferred();

   var r = new Recipient(json.id, json.firstName, json.lastName, json.fields);
   r.mergeFields()
   .done(function () {
      d.resolve(r);
   });

   return d.promise();
};

/**
 * Create a new recipient field (from server data only)
 * @param json Json data from server
 * @returns {RecipientField} a recipient field object
 */
RecipientManager.factoryField = function (json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.idRecipient), "json.idRecipient must be defined");
   assert(!isNullOrUndefined(json.pluginType), "json.pluginType must be defined");
   assert(!isNullOrUndefined(json.fieldName), "json.fieldName must be defined");
   assert(!isNullOrUndefined(json.value), "json.value must be defined");

   var d = $.Deferred();

   var r = new RecipientField(json.idRecipient, json.pluginType, json.fieldName, json.value);
   d.resolve(r);

   return d.promise();
};


/**
 * Get a recipient from the server by Id
 * @param recipientId The Id to find
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.get = function (recipientId) {
   assert(!isNullOrUndefined(recipientId), "recipientId must be defined");

   var d = $.Deferred();

   RestEngine.getJson("/rest/recipient/" + recipientId)
   .done(function (data) {
      RecipientManager.factory(data)
      .done(function (newRecipient) {
         d.resolve(newRecipient);
      });
   })
   .fail(function (error) {
      notifyError($.t("objects.generic.errorGetting", { objectName: "Recipient with Id = " + recipientId }), error);
      d.reject(error);
   });
   return d.promise();
};


/**
 * Create an empty recipient (with all fields empty)
 */
RecipientManager.createEmptyRecipient = function () {
   var d = $.Deferred();

   var emptyRecipient = new Recipient(undefined, "", "", []);
   emptyRecipient.mergeFields()
       .done(function () {
          d.resolve(emptyRecipient);
       });

   return d.promise();
};

/**
 * Get the list of all recipients
 */
RecipientManager.getAll = function () {

   var d = $.Deferred();

   RestEngine.getJson("/rest/recipient")
   .done(function (data) {
      var recipientList = [];
      var arrayOfDeffered = [];
      $.each(data.recipient, function (index, value) {
         var deffered = RecipientManager.factory(value);
         arrayOfDeffered.push(deffered);
         deffered.done(function (newRecipient) {
            recipientList.push(newRecipient);
         });
      });

      //when all deffered have been finished (and even if one of them is rejected)
      $.when.apply($, arrayOfDeffered).done(function () {
         d.resolve(recipientList);
      });
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Get the list of recipients containing a specific field
 * @param field The field to search for
 */
RecipientManager.getByField = function (field) {
   assert(!isNullOrUndefined(field), "field must be defined");

   var d = $.Deferred();

   RestEngine.getJson("/rest/recipient/field/" + field)
   .done(function (data) {
      var recipientFieldList = [];
      var arrayOfDeffered = [];

      $.each(data.field, function (index, value) {
         var deffered = RecipientManager.factoryField(value);
         arrayOfDeffered.push(deffered);
         deffered.done(function (newRecipient) {
            recipientFieldList.push(newRecipient);
         });
      });

      //when all deffered have been finished (and even if one of them is rejected)
      $.when.apply($, arrayOfDeffered).done(function () {
         d.resolve(recipientFieldList);
      });
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Delete a recipient
 * @param recipientToDelete The recipient to delete
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.deleteFromServer = function (recipientToDelete) {
   assert(!isNullOrUndefined(recipientToDelete), "recipient must be defined");
   return RestEngine.deleteJson("/rest/recipient/" + recipientToDelete.id);
};

/**
 * Update a recipient
 * @param recipientToUpdate The recipient to update
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.updateToServer = function (recipientToUpdate) {
   assert(!isNullOrUndefined(recipientToUpdate), "recipient must be defined");

   var d = $.Deferred();

   RestEngine.putJson("/rest/recipient/" + recipientToUpdate.id, { data: JSON.stringify(recipientToUpdate) })
   .done(function(data) {
      //it's okay
      //we update our information from the server
      RecipientManager.factory(data)
      .done(function (recipientUpdated) {
         d.resolve(recipientUpdated);
      });
   })
   .fail(d.reject);

   return d.promise();
};

/**
 * Add a recipient on the server
 * @param recipientToAdd The recipient to add
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.addToServer = function (recipientToAdd) {
   assert(!isNullOrUndefined(recipientToAdd), "recipient must be defined");

   var d = $.Deferred();

   RestEngine.postJson("/rest/recipient", { data: JSON.stringify(recipientToAdd) })
   .done(function (data) {
      //it's okay
      //we update our information from the server
      RecipientManager.factory(data)
      .done(function (recipientAdded) {
         d.resolve(recipientAdded);
      });
   })
   .fail(d.reject);

   return d.promise();
};