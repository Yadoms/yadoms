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
        target.time(moment().format("LT"));
        target.date(moment().format("LL"));
    }

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;
        var d = new $.Deferred();

        self.widgetApi.loadCss("widgets/clock/widget.css")
        .done(function () {
            self.widgetApi.toolbar({
                activated: false
            });

            setInterval(function () {
                updateTime_(self);
            }, 1000);
            updateTime_(self);
            d.resolve();
        });

        return d.promise();
    };
};
