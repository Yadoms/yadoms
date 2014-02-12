/**
 * Create a SimpleClock ViewModel
 * @constructor
 */
function SimpleClockViewModel() {
   /**
    * Observable data
    */
   this.data = null;

   /**
    * Widget identifier
    */
   this.widget = null;
}

/**
 * Initialization method
 * @param page page where the widget is
 * @param widget widget class object
 */
SimpleClockViewModel.prototype.initialize = function(page, widget) {
   this.widget = widget;

   var target = this;
   setInterval(function() {
      SimpleClockViewModel_updateTime(target)
   }, 1000);
   SimpleClockViewModel_updateTime(target);
};

/**
 * Callback for a resized event
 */
SimpleClockViewModel.prototype.resized = function() {
   if (this.widget.$gridsterWidget.width() <= 200) {
      this.widget.$gridsterWidget.find(".simpleClockWidget").css("font-size", "1.5em");
   }
   else if (this.widget.$gridsterWidget.width() <= 300)
      this.widget.$gridsterWidget.find(".simpleClockWidget").css("font-size", "3.0em");
   else if (this.widget.$gridsterWidget.width() <= 400)
      this.widget.$gridsterWidget.find(".simpleClockWidget").css("font-size", "5.0em");
   else
      this.widget.$gridsterWidget.find(".simpleClockWidget").css("font-size", "7.0em");
};

/**
 * Update the time onto the widget
 * @param target
 */
function SimpleClockViewModel_updateTime(target) {
   var currentTime = new Date ( );
   var currentHours = currentTime.getHours ( );
   var currentMinutes = currentTime.getMinutes ( );
   var currentSeconds = currentTime.getSeconds ( );

   // Pad the minutes and seconds with leading zeros, if required
   currentMinutes = ( currentMinutes < 10 ? "0" : "" ) + currentMinutes;
   currentSeconds = ( currentSeconds < 10 ? "0" : "" ) + currentSeconds;

   // Compose the string for display
   var currentTimeString = currentHours + ":" + currentMinutes + ":" + currentSeconds ;

   // Update the time display
   target.widget.$div.find(".simpleClockWidget").text(currentTimeString);
}

/**
 * Widget ViewModel instantiation
 */
widgetViewModel = new SimpleClockViewModel();
widgetViewModel.data = ko.observable();
