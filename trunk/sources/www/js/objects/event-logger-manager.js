/**
 * Created by nicolasHILAIRE on 02/11/2015.
 */

/**
 * Obtain last event log item
 */
EventLoggerManager.getLast = function (dontNotifyFailure, additionnalJsonParameters) {
    if (!dontNotifyFailure)
        dontNotifyFailure = false;

    var d = $.Deferred();

    $.ajax($.extend({
            dataType: "json",
            url: "/rest/eventLogger/last"
        }, additionnalJsonParameters))
    .done(function (data) {
        //we parse the json answer
        if (data.result != "true") {
            if (!dontNotifyFailure)
                notifyError($.t("objects.generic.errorGetting", { objectName: $.t("core.event") }, JSON.stringify(data)));
            d.reject();
        } else {
            d.resolve(data.data);
        }
    })
    .fail(function () {
        if (!dontNotifyFailure)
            notifyError($.t("objects.generic.errorGetting", { objectName: $.t("core.event") }));
        d.reject();
    });

    return d.promise();
};

/**
 * Obtain all event log items occured after eventId
 */
EventLoggerManager.getFrom = function (eventId) {
    var d = $.Deferred();

    $.ajax({
        dataType: "json",
        url: "/rest/eventLogger/from/" + eventId,
    })
    .done(function (data) {
        //we parse the json answer
        if (data.result != "true") {
            notifyError($.t("objects.generic.errorGetting", { objectName: $.t("core.event") }, JSON.stringify(data)));
            d.reject();
        } else {
            d.resolve(data.data);
        }
    })
    .fail(function () {
        notifyError($.t("objects.generic.errorGetting", { objectName: $.t("core.event") }));
        d.reject();
    });

    return d.promise();
};

/**
 *
 * @constructor
 */
function EventLoggerManager() {
}

