function SimpleClockViewModel() {
   //observable data
   this.data;

   //page identifier
   this.parentPage;

   //widget identifier
   this.widget;

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
   }

   this.initialize = function(page, widget) {
      this.parentPage = page;
      this.widget = widget;

      var target = this;
      setInterval( function() {updateTime(target)}, 1000);
      updateTime(target);
   }

   function updateTime(target) {
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
}

widgetViewModel = new SimpleClockViewModel();
widgetViewModel.data = ko.observable();
