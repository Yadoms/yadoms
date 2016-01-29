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
RecipientManager.get = function (recipientId, sync) {
    assert(!isNullOrUndefined(recipientId), "recipientId must be defined");

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
        async = !sync;

    $.ajax({
        type: "GET",
        url: "/rest/recipient/" + recipientId,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        async: async
    })
        .done(function (data) {
            //we parse the json answer
            if (data.result != "true") {
                notifyError($.t("objects.generic.errorGetting", { objectName: "Recipient with Id = " + recipientId }), JSON.stringify(data));
                d.fail();
                return;
            }
            RecipientManager.factory(data.data)
                .done(function (newRecipient) {
                    d.resolve(newRecipient);
                });
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorGetting", { objectName: "Recipient with Id = " + recipientId }));
            d.fail();
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
  * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.getAll = function (sync) {

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
        async = !sync;

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
                d.fail();
                return;
            }

            var recipientList = [];
            var arrayOfDeffered = [];
            $.each(data.data.recipient, function (index, value) {
                var deffered = RecipientManager.factory(value);
                arrayOfDeffered.push(deffered);
                deffered.done(function(newRecipient) {
                    recipientList.push(newRecipient);
                });
            });

            //when all deffered have been finished (and even if one of them is rejected)
            $.whenAll(arrayOfDeffered).done(function () {
                d.resolve(recipientList);
            });
        })
    .fail(function () {
        notifyError($.t("modals.recipient.errorGettingList"));
        d.fail();
    });

    return d.promise();
};

/**
 * Get the list of recipients containing a specific field
 * @param field The field to search for
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.getByField = function (field, sync) {
    assert(!isNullOrUndefined(field), "field must be defined");

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
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
               notifyError($.t("objects.generic.errorGetting", { objectName: "Recipient fields with name = " + field }), JSON.stringify(data));
               d.fail();
               return;
           }

           var recipientFieldList = [];
           var arrayOfDeffered = [];

           $.each(data.data.field, function (index, value) {
               var deffered = RecipientManager.factoryField(value);
               arrayOfDeffered.push(deffered);
               deffered.done(function (newRecipient) {
                   recipientFieldList.push(newRecipient);
               });
           });

           //when all deffered have been finished (and even if one of them is rejected)
           $.whenAll(arrayOfDeffered).done(function () {
               d.resolve(recipientFieldList);
           });


       })
       .fail(function () {
           notifyError($.t("objects.generic.errorGetting", { objectName: "Recipient fields with name = " + field }));
           d.fail();
       });

    return d.promise();
};

/**
 * Delete a recipient
 * @param recipientToDelete The recipient to delete
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.deleteFromServer = function (recipientToDelete, sync) {
    assert(!isNullOrUndefined(recipientToDelete), "recipient must be defined");

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
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
                notifyError($.t("objects.generic.errorDeleting", { objectName: recipientToDelete.firstName + " " + recipientToDelete.lastName }), JSON.stringify(data));
                d.fail();
                return;
            }
            d.resolve();
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorDeleting", { objectName: recipientToDelete.firstName + " " + recipientToDelete.lastName }));
            d.fail();
        });

    return d.promise();
};

/**
 * Update a recipient
 * @param recipientToUpdate The recipient to update
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.updateToServer = function (recipientToUpdate, sync) {
    assert(!isNullOrUndefined(recipientToUpdate), "recipient must be defined");

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
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
                notifyError($.t("objects.generic.errorUpdating", { objectName: recipientToUpdate.firstName + " " + recipientToUpdate.lastName }), JSON.stringify(data));
                d.fail();
                return;
            }
            //it's okay
            //we update our information from the server
            RecipientManager.factory(data.data)
            .done(function(recipientUpdated) {
                d.resolve(recipientUpdated);
            });
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorUpdating", { objectName: recipientToUpdate.firstName + " " + recipientToUpdate.lastName }));
            d.fail();
        });
    return d.promise();
};

/**
 * Add a recipient on the server
 * @param recipientToAdd The recipient to add
 * @param sync True to get the result synchronously, false asynchronously
 */
RecipientManager.addToServer = function (recipientToAdd, sync) {
    assert(!isNullOrUndefined(recipientToAdd), "recipient must be defined");

    var d = $.Deferred();

    var async = true;
    if (!isNullOrUndefined(sync) && $.type(sync) === "boolean")
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
                notifyError($.t("objects.generic.errorUpdating", { objectName: recipientToAdd.firstName + " " + recipientToAdd.lastName }), JSON.stringify(data));
                d.fail();
                return;
            }
            //it's okay
            //we update our information from the server
            RecipientManager.factory(data.data)
           .done(function (recipientAdded) {
               d.resolve(recipientAdded);
           });
        })
        .fail(function () {
            notifyError($.t("objects.generic.errorUpdating", { objectName: recipientToAdd.firstName + " " + recipientToAdd.lastName }));
            d.fail();
        });
    return d.promise();
};