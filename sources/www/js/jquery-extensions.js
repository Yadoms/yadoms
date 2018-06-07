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


/**
* Uses canvas.measureText to compute and return the width of the given text of given font in pixels.
* 
* @param {String} text The text to be rendered.
* @param {String} font The css font descriptor that text is to be rendered with (e.g. "bold 14px verdana").
* 
* @see http://stackoverflow.com/questions/118241/calculate-text-width-with-javascript/21015393#21015393
*/
function getTextWidth(text, font) {
    // re-use canvas object for better performance
    var canvas = getTextWidth.canvas || (getTextWidth.canvas = document.createElement("canvas"));
    var context = canvas.getContext("2d");
    
    context.font = font;
    var metrics = context.measureText(text);
    return metrics.width;
};


(function ($) {

    /**
     * Ensure that text contained will fit
     * @returns {} 
     */
    $.fn.fitText = function () {
        return this.each(function () {

            // Store the object
            var $this = $(this);

            if (!$.isFunction($this.resizer)) {
                // Resizer() resizes items based on the object width divided by the compressor * 10
                $this.resizer = function () {
                    //we get full text of $this including :bfore and :after content for font awesome icons
                    var text = "";
                    var before = getComputedStyle($this[0], ':before');
                    if ((before) && (before.content) && (before.content!=="none") && (before.content[1]!=undefined))
                        text += before.content[1];
                    text += $this.text();
                    var after = getComputedStyle($this[0], ':after');
                    if ((after) && (after.content) && (after.content!=="none") && (after.content[1]!=undefined))
                        text += after.content[1];
                     
                    var minFont = $this.attr("min-font") || 9;
                    if (text === "")
                        text = "W";
                     
                    $this.css('font-size', 1);
                    
                    //determine the width for a large fontsize
                    var px300 = getTextWidth(text, "300px " + $this.css("font-family"));
                    
                    //estimate the size relative to the item width, and apply a 0.8 factor to ensure the text will fit
                    var fontSizeFromWidth = ($this.width() * 300 / px300) * .8;

                    var $temp = $this.parent();
                    while ($temp.hasClass("textfit-in-parent")) $temp = $temp.parent();
                    
                    if ($this.hasClass("textfit-in-parent")) {
                        fontSizeFromWidth = ($temp.width() * 300 / px300) * .8;
                    }

                    var fontSizeFromHeight = $this.height() / 1.42857143;
                    
                    if ($this.hasClass("textfit-in-parent")) {
                        fontSizeFromHeight = $temp.height() / 1.42857143;
                    }

                    $this.css('font-size', Math.max(Math.min(fontSizeFromWidth, fontSizeFromHeight), minFont));
                };
                // Call on resize. Opera debounces their resize by default.
                $(window).on('resize.fittext orientationchange.fittext', $this.resizer);
            }
            // Call once to set.
            $this.resizer();


        });
    };

    /**
     * Function which make any dom object to be visible
     * @param {Dom} element The dom element
     * @param {Boolean} complete If true ensure the element height must be totally visible, else ensure the top of element is visible
     * @return {Boolean} true if the element was visible, false if scroll animation is in progress
     */
    $.fn.ensureVisible = function (complete) {
        return this.each(function () {
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
