/**
 * Function which wait for all deferred to end (either resolved or rejected)
 * @param {Array} arrayOfDeffered The array of deferred to wait for
 * @return {Promise} The promise
 */
$.whenAll = function (arrayOfDeffered) {
    return $.when.apply($, $.map(arrayOfDeffered, function(d) {
        var wrapDeferred = $.Deferred();
        // you can add .done and .fail if you want to keep track of each results individualy
        d.always(function() {
            wrapDeferred.resolve();
        });
        return wrapDeferred.promise();
    }));
}