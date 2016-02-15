/**
 * Function which wait for all deferred to end (either resolved or rejected)
 * @param {Array} arrayOfDeffered The array of deferred to wait for
 * @return {Promise} The promise
 */
$.whenAll = function (arrayOfDeffered) {
    return $.when.apply($, $.map(arrayOfDeffered, function (d) {
        var wrapDeferred = $.Deferred();
        // you can add .done and .fail if you want to keep track of each results individualy
        d.always(function () {
            wrapDeferred.resolve();
        });
        return wrapDeferred.promise();
    }));
};

(function ($) {
    /**
     * Ensure that text contained will fit
     * @param {number} compressor : width coefficient (default: 0.3)
     * @returns {} 
     */
    $.fn.fitText = function (compressor) {

        // Setup options
        var comp = compressor || 0.3;

        return this.each(function () {

            // Store the object
            var $this = $(this);

            // Resizer() resizes items based on the object width divided by the compressor * 10
            var resizer = function () {
                //we let the container fit the right height
                $this.css('font-size', 1);
                //width coeff must be 0.3
                var fontSizeFromWidth = $this.width() / (comp * 10);
                //height coeeff
                var lh = parseFloat($this.css("line-height"));
                var fontSizeFromHeight = $this.height() / lh;
                $this.css('font-size', Math.min(fontSizeFromWidth, fontSizeFromHeight));
            };

            // Call once to set.
            resizer();

            // Call on resize. Opera debounces their resize by default.
            $(window).on('resize.fittext orientationchange.fittext', resizer);

        });
    };

    /**
     * Function which make any dom object to be visible
     * @param {Dom} element The dom element
     * @param {Boolean} complete If true ensure the element height must be totally visible, else ensure the top of element is visible
     * @return {Boolean} true if the element was visible, false if scroll animation is in progress
     */
    $.fn.ensureVisible = function (complete) {
        return this.each(function() {
            // Store the object
            var $this = $(this);

            //check if element top is visible
            var offset = $this.offset().top - $(window).scrollTop();
            var isCompletlyOutOfInnerView = (offset > window.innerHeight);

            if (complete === true && !isCompletlyOutOfInnerView) {
                //if element top is visible, ensure element bottom is visible
                isCompletlyOutOfInnerView = (offset + $this.height() > window.innerHeight);
            }

            if (isCompletlyOutOfInnerView) {
                // Not in view
                $('html,body').animate({ scrollTop: offset }, 1000);
            }
        });
    };

})(jQuery);
