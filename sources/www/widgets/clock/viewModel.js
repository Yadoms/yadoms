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
     * Update the time onto the widget
     * @param target
     */
    function updateTime_(target) {
        target.time(moment().format("LT"));
        //target.$dateField.text(moment().format("LL"));
    }

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;

        self.widgetApi.toolbar({
            activated: false
        });

        //self.$dateField = self.widgetApi.find(".date-field");

        setInterval(function () {
            updateTime_(self);
        }, 1000);
        updateTime_(self);
    };
};
