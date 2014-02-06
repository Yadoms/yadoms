function SimpleClockViewModel() {
    //observable data
    this.data;
    
    //widget DOM object (div)
    this.$widgetNode;

    //gridster widget node object (li)
    this.$grisdterWidgetNode;

    //configuration
    this.configuration;

    this.resized = function() {
    if (this.$widgetNode.width() <= 150) {
            this.$widgetNode.find(".simpleClockWidget").css("font-size", "1.0em");
        }
        else if (this.$widgetNode.width() <= 400)
            this.$widgetNode.find(".simpleClockWidget").css("font-size", "2.0em");
        else
            this.$widgetNode.find(".simpleClockWidget").css("font-size", "4.0em");
    }
    
    this.initialize = function($widgetNode, $gridsterWidgetNode, configuration) {
        this.$widgetNode = $widgetNode
        this.$grisdterWidgetNode = $gridsterWidgetNode;
        this.configuration = configuration;

        var target = this;
        /*
        setInterval( function() {
        var seconds = new Date().getSeconds();
        var sdegree = seconds * 6;
        var srotate = "rotate(" + sdegree + "deg)";
        target.$widget.find(".sec").css({"-moz-transform" : srotate, "-webkit-transform" : srotate});

        }, 1000 );

        setInterval( function() {
        var hours = new Date().getHours();
        var mins = new Date().getMinutes();
        var hdegree = hours * 30 + (mins / 2);
        var hrotate = "rotate(" + hdegree + "deg)";

        target.$widget.find(".hour").css({"-moz-transform" : hrotate, "-webkit-transform" : hrotate});

        }, 1000 );

        setInterval( function() {
        var mins = new Date().getMinutes();
        var mdegree = mins * 6;
        var mrotate = "rotate(" + mdegree + "deg)";

        target.$widget.find(".min").css({"-moz-transform" : mrotate, "-webkit-transform" : mrotate});

        }, 1000 );
        */

        setInterval( function() {updateTime(target)}, 1000);
        updateTime(target);
    }
    
   function updateTime(viewModel) {
     var currentTime = new Date ( );
     var currentHours = currentTime.getHours ( );
     var currentMinutes = currentTime.getMinutes ( );
     var currentSeconds = currentTime.getSeconds ( );

     // Pad the minutes and seconds with leading zeros, if required
     currentMinutes = ( currentMinutes < 10 ? "0" : "" ) + currentMinutes;
     currentSeconds = ( currentSeconds < 10 ? "0" : "" ) + currentSeconds;

     // Choose either "AM" or "PM" as appropriate
     var timeOfDay = ( currentHours < 12 ) ? "AM" : "PM";

     // Convert the hours component to 12-hour format if needed
     currentHours = ( currentHours > 12 ) ? currentHours - 12 : currentHours;

     // Convert an hours component of "0" to "12"
     currentHours = ( currentHours == 0 ) ? 12 : currentHours;

     // Compose the string for display
     var currentTimeString = currentHours + ":" + currentMinutes + ":" + currentSeconds + " " + timeOfDay;

     // Update the time display
      viewModel.$widgetNode.find(".simpleClockWidget").text(currentTimeString);
   }
}

widgetViewModel = new SimpleClockViewModel();
widgetViewModel.data = ko.observable();
