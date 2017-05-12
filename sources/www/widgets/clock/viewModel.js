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
    this.date = ko.observable("");


    /**
     * Update the time onto the widget
     * @param target
     */
    function updateTime_(target) {
        serverTime = target.widgetApi.askServerLocalTime();
        target.time(moment(serverTime).format("LT"));
        target.date(moment(serverTime).format("LL"));
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

        setInterval(function () {
            updateTime_(self);
        }, 10000);
        updateTime_(self);

    };
};
