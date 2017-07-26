widgetViewModelCtor =

/**
 * Create a colorRGB ViewModel
 * @constructor
 */
function colorRGBViewModel() {
    //observable data
    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: false
        });
        
        // Display the color picker
        this.widgetApi.find(".picker-canvas").colorpicker({
            color: '#ffaa00',
            container: true,
            inline: true
        });
    };

    this.configurationChanged = function () {
        var self = this;

        // TODO
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === self.widget.configuration.device.keywordId) {

          // TODO

        }
    };
};