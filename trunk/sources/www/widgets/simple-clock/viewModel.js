widgetViewModelCtor =

/**
 * Create a SimpleClock ViewModel
 * @constructor
 */
function SimpleClockViewModel() {
   /**
    * Observable data
    * if not used must be set to ko.observable()
    */
   this.data = ko.observable();

   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;

      var target = this;
      setInterval(function() {
         SimpleClockViewModel_updateTime(target)
      }, 100);
      SimpleClockViewModel_updateTime(target);
   };

   /**
   * Callback for a resized event
   */
   this.resized = function() {
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

   this.configurationChanged = function() {

   };

   this.getDevicesToListen = function() {
      //we are not linked to any device
      return new Array();
   }

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
