widgetViewModelCtor =

/**
 * Create a Clock ViewModel
 * @constructor
 */
function ClockViewModel() {
    /**
     * Observable data
     */
    var self = this;
    this.time = ko.observable("00:00");

    this.timeFontSize = ko.observable("1.5em");
    this.timeFontSizeCSS = ko.computed(function () {
        return { "font-size": self.timeFontSize() };
    }, this);

    /**
     * Widget identifier
     */
    this.widget = null;

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function (widget) {
        this.widget = widget;

        var target = this;

        //we set the date into toolbar
        ToolbarApi.appendCustomIcon(this.widget, "<span class=\"widget-clock-date-field\"></span>");

        this.$dateField = this.widget.$gridWidget.find(".widget-clock-date-field");

        setInterval(function () {
            ClockViewModel_updateTime(target);
        }, 1000);
        ClockViewModel_updateTime(target);
    };

    /**
    * Callback for a resized event
    */
    this.resized = function () {
        if (this.widget.width() <= 200) {
            this.timeFontSize("2.7em");
        }
        else if (this.widget.width() <= 300) {
            this.timeFontSize("3.3em");
        }
        else {
            this.timeFontSize("6.0em");
        }
    };

};

/**
 * Update the time onto the widget
 * @param target
 */
function ClockViewModel_updateTime(target) {
    var currentTime = new Date();
    var currentHours = currentTime.getHours();
    var currentMinutes = currentTime.getMinutes();
    //var currentSeconds = currentTime.getSeconds ( );

    // Pad the minutes and seconds with leading zeros, if required
    currentMinutes = (currentMinutes < 10 ? "0" : "") + currentMinutes;
    //currentSeconds = ( currentSeconds < 10 ? "0" : "" ) + currentSeconds;

    // Compose the string for display
    var currentTimeString = currentHours + ":" + currentMinutes;// + ":" + currentSeconds ;

    // Update the date and time display
    target.time(currentTimeString);
    target.$dateField.text(moment().format("LL"));
}
