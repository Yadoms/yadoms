widgetViewModelCtor =

/**
 * Create a Clock ViewModel
 * @constructor
 */
function clockViewModel() {
    /**
     * Observable data
     */
    this.time = ko.observable("00:00");

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

        var self = this;

        //we set the date into toolbar
        WidgetApi.toolbar.appendCustom(self.widget, "<span class=\"date-field\"></span>");

        this.$dateField = self.widget.$gridWidget.find(".date-field");

        setInterval(function () {
            updateTime_(self);
        }, 1000);
        updateTime_(self);
    };

    /**
    * Callback for a resized event
    */
    this.resized = function () {
    };

    /**
     * Update the time onto the widget
     * @param target
     */
    function updateTime_(target) {
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

};
