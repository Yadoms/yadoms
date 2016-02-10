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

/**
 * Function which make any dom object to be visible
 * @param {Dom} element The dom element
 * @param {Boolean} complete If true ensure the element height must be totally visible, else ensure the top of element is visible
 * @return {Boolean} true if the element was visible, false if scroll animation is in progress
 */
$.ensureVisible = function(element, complete) {
   //check if element top is visible
   var offset = element.offset().top - $(window).scrollTop();
   var isCompletlyOutOfInnerView = (offset > window.innerHeight);

   if (complete === true && !isCompletlyOutOfInnerView) {
      //if element top is visible, ensure element bottom is visible
      isCompletlyOutOfInnerView = (offset + element.height() > window.innerHeight);
   }

   if (isCompletlyOutOfInnerView) {
      // Not in view
      $('html,body').animate({ scrollTop: offset }, 1000);
      return false;
   }
   return true;
}