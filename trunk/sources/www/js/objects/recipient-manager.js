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
 * @param allAvailableFields
 * @returns {Recipient} a recipient object
 */
RecipientManager.factory = function (json, allAvailableFields) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.firstName), "json.firstName must be defined");
    assert(!isNullOrUndefined(json.lastName), "json.lastName must be defined");
    assert(!isNullOrUndefined(json.fields), "json.fields must be defined");

    var r = new Recipient(json.id, decodeURIComponent(json.firstName), decodeURIComponent(json.lastName), json.fields);
    RecipientManager.completeRecipientWithAllFields_(r, allAvailableFields);
    return r;
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

    return new RecipientField(json.idRecipient, decodeURIComponent(json.pluginType), decodeURIComponent(json.fieldName), decodeURIComponent(json.value));
};


/**
 * Get a recipient from the server by Id
 * @param recipientId The Id to find
 * @param callback The callback to receive result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.get = function (recipientId, callback, sync) {
    assert(!isNullOrUndefined(recipientId), "recipientId must be defined");
    assert($.isFunction(callback), "callback must be a function");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        type: "GET",
        url: "/rest/recipient/" + +recipientId,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("objects.generic.errorGetting", {objectName: "Recipient with Id = " + recipientId}), JSON.stringify(data));
                return;
            }
            callback(RecipientManager.factory(data.data));
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorGetting", {objectName: "Recipient with Id = " + recipientId}));
        });
};


/**
 * Create an empty recipient (with all fields empty)
 */
RecipientManager.createEmptyRecipient = function () {
    var emptyRecipient = new Recipient(undefined, "", "", []);
    RecipientManager.completeRecipientWithAllFields_(emptyRecipient);
    return emptyRecipient;
};


/**
 * Get all the fields available (both system and plugins)
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 * @private
 */
RecipientManager.getAllPluginFields_ = function (callback, sync) {
    assert($.isFunction(callback), "callback must be a function");
    PluginManager.get(callback, sync);
};


/**
 * Complete a recipient with fields
 * @param recipientToComplete The recipient to complete
 * @param allAvailableFields The list of all available fields (if undefined, request it from server)
 * @private
 */
RecipientManager.completeRecipientWithAllFields_ = function(recipientToComplete, allAvailableFields) {
    if(!isNullOrUndefined(allAvailableFields)) {
        recipientToComplete.mergeFields(allAvailableFields);
    } else {
        //we start by requesting all plugin fields
        RecipientManager.getAllPluginFields_(function (allPlugin) {
            recipientToComplete.mergeFields(allPlugin);
        }, true);
    }

};


/**
 * Get the list of all recipients
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.getAll = function (callback, sync) {
    assert($.isFunction(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    //we start by requesting all plugin fields
    RecipientManager.getAllPluginFields_(function (allPlugin) {

        $.ajax({
            type: "GET",
            url: "/rest/recipient",
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            async: async
        })
            .done(
            /**
             * Receive result from server
             * @param {{result:string}, {data: {recipient : object}}} data
             */
            function (data) {
                //we parse the json answer
                if (data.result != "true") {
                    notifyError($.t("objects.recipient.errorGettingList"), JSON.stringify(data));
                    return;
                }

                var recipientList = [];
                $.each(data.data.recipient, function (index, value) {
                    recipientList.push(RecipientManager.factory(value, allPlugin));
                });

                if ($.isFunction(callback))
                    callback(recipientList);
            })
            .fail(function () {
                notifyError($.t("modals.recipient.errorGettingList"));
                callback(null);
            });

    }, sync);

};

/**
 * Get the list of recipients containing a specific field
 * @param field The field to search for
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.getByField = function (field, callback, sync) {
   assert($.isFunction(callback), "callback must be defined");
   assert(!isNullOrUndefined(field), "field must be defined");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      type: "GET",
      url: "/rest/recipient/field/" + field,
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      async: async
   })
      .done(function (data) {
         //we parse the json answer
         if (data.result != "true") {
            notifyError($.t("objects.generic.errorGetting", {objectName: "Recipient fields with name = " + field}), JSON.stringify(data));
            return;
         }

         var recipientFieldList = [];
         $.each(data.data.field, function (index, value) {
            recipientFieldList.push(RecipientManager.factoryField(value));
         });

         //we call the callback with true as a ok result
         if ($.isFunction(callback))
            callback(recipientFieldList);
         
      })
      .fail(function () {
         notifyError($.t("objects.generic.errorGetting", {objectName: "Recipient fields with name = " + field}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(null);
      });
};

/**
 * Delete a recipient
 * @param recipientToDelete The recipient to delete
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.deleteFromServer = function (recipientToDelete, callback, sync) {
    assert(!isNullOrUndefined(recipientToDelete), "recipient must be defined");
    assert($.isFunction(callback), "callback must be defined");
    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        type: "DELETE",
        url: "/rest/recipient/" + recipientToDelete.id,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("objects.generic.errorDeleting", {objectName: recipientToDelete.firstName + " " + recipientToDelete.lastName}), JSON.stringify(data));
                return;
            }
            callback();
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorDeleting", {objectName: recipientToDelete.firstName + " " + recipientToDelete.lastName}));
        });
};

/**
 * Update a recipient
 * @param recipientToUpdate The recipient to update
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.updateToServer = function (recipientToUpdate, callback, sync) {
    assert(!isNullOrUndefined(recipientToUpdate), "recipient must be defined");
    assert($.isFunction(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;
    $.ajax({
        type: "PUT",
        url: "/rest/recipient/" + recipientToUpdate.id,
        data: JSON.stringify(recipientToUpdate),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("objects.generic.errorUpdating", {objectName: recipientToUpdate.firstName + " " + recipientToUpdate.lastName}), JSON.stringify(data));
                //launch callback with false as ko result
                if ($.isFunction(callback))
                    callback(false);
                return;
            }
            //it's okay
            //we update our information from the server
            recipientToUpdate = RecipientManager.factory(data.data);

            //we call the callback with true as a ok result
            if ($.isFunction(callback))
                callback(true, recipientToUpdate);
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorUpdating", {objectName: recipientToUpdate.firstName + " " + recipientToUpdate.lastName}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
                callback(false);
        });
};

/**
 * Add a recipient on the server
 * @param recipientToAdd The recipient to add
 * @param callback The callback for the result
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.addToServer = function (recipientToAdd, callback, sync) {
    assert(!isNullOrUndefined(recipientToAdd), "recipient must be defined");
    assert($.isFunction(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        type: "POST",
        url: "/rest/recipient",
        data: JSON.stringify(recipientToAdd),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("objects.generic.errorUpdating", {objectName: recipientToAdd.firstName + " " + recipientToAdd.lastName}), JSON.stringify(data));
                //launch callback with false as ko result
                if ($.isFunction(callback))
                    callback(false);
                return;
            }
            //it's okay
            //we update our information from the server
            recipientToAdd = RecipientManager.factory(data.data);

            //we call the callback with true as a ok result
            if ($.isFunction(callback))
                callback(true, recipientToAdd);
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorUpdating", {objectName: recipientToAdd.firstName + " " + recipientToAdd.lastName}));
            //launch callback with false as ko result
            if ($.isFunction(callback))
                callback(false);
        });
};