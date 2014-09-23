widgetViewModelCtor =

/**
 * Create a Clock ViewModel
 * @constructor
 */
function ClockViewModel() {
   /**
    * Observable data
    */
   this.time = ko.observable("00:00");
   this.date = ko.observable("");

   this.timeFontSize = ko.observable("1.5em");
   this.timeFontSizeCSS = ko.computed(function() {
      return {"font-size": this.timeFontSize()};
   }, this);

   this.dateFontSize = ko.observable("1.0em");
   this.dateFontSizeCSS = ko.computed(function() {
      return {"font-size": this.dateFontSize()};
   }, this);

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
         ClockViewModel_updateTime(target)
      }, 1000);
      ClockViewModel_updateTime(target);
   };

   /**
   * Callback for a resized event
   */
   this.resized = function() {
      if (this.widget.width() <= 200) {
         this.dateFontSize("1.0em");
         this.timeFontSize("2.7em");
      }
      else if (this.widget.width() <= 300) {
         this.dateFontSize("1.0em");
         this.timeFontSize("5.0em");
      }
      else {
         this.dateFontSize("1.5em");
         this.timeFontSize("6.0em");
      }
   };

   this.configurationChanged = function() {

   };

   this.getDevicesToListen = function() {
      //we are not linked to any device
      return [];
   }

 };

/**
 * Update the time onto the widget
 * @param target
 */
function ClockViewModel_updateTime(target) {
   var currentTime = new Date ( );
   var currentHours = currentTime.getHours ( );
   var currentMinutes = currentTime.getMinutes ( );
   //var currentSeconds = currentTime.getSeconds ( );

   // Pad the minutes and seconds with leading zeros, if required
   currentMinutes = ( currentMinutes < 10 ? "0" : "" ) + currentMinutes;
   //currentSeconds = ( currentSeconds < 10 ? "0" : "" ) + currentSeconds;

   // Compose the string for display
   var currentTimeString = currentHours + ":" + currentMinutes;// + ":" + currentSeconds ;

   // Update the date and time display
   target.time(currentTimeString);
   target.date(moment().format("LL"));
}
