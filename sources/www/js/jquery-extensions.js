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
    * @param {number} compressor : width coefficient (default: 0.3)
    * @returns {} 
    */
   $.fn.fitText = function (minFont) {
      var minimumFontSize = minFont || 14;

      return this.each(function () {

         // Store the object
         var $this = $(this);

         if (!$.isFunction($this.resizer)) {
            // Resizer() resizes items based on the object width divided by the compressor * 10
            $this.resizer = function() {
               var text = $this.text();
               if (text === "")
                  text = "W";

               //determine the width for a large fontsize
               var px300 = getTextWidth(text, "300px " + $this.css("text-family"));
               //estimate the size relative to the item width, and apply a 0.8 factor to ensure the text will fit
               var fontSizeFromWidth = ($this.width() * 300 / px300) * 0.8;

               $this.css('font-size', 1);
               var lh = parseFloat($this.css("line-height"));
               var fontSizeFromHeight = $this.height() / lh;
               $this.css('font-size', Math.max(Math.min(fontSizeFromWidth, fontSizeFromHeight), minimumFontSize));
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
