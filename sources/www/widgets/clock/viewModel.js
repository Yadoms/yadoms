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
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;

        //we set the date into toolbar
        self.widgetApi.toolbar.appendCustom("<span class=\"date-field\"></span>");

        self.$dateField = self.widgetApi.find(".date-field");

        setInterval(function () {
            updateTime_(self);
        }, 1000);
        updateTime_(self);
    };

    /**
     * Update the time onto the widget
     * @param target
     */
    function updateTime_(target) {
        target.time(moment().format("LT"));
        target.$dateField.text(moment().format("LL"));
    }

};
