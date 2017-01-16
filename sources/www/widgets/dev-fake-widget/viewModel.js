/**
 * Create a Smiley ViewModel
 * @constructor
 */

widgetViewModelCtor = function () {
    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;

        self.widgetApi.toolbar({
            activated: false
        });
    };
};
