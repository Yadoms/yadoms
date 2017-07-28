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
        self = this;
       
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: false
        });
        
        // Display the color picker
        console.log (this.widgetApi.find(".picker-canvas"));
        this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'colorpicker-2x',
            sliders: {
                saturation: {
                    maxLeft: 200,
                    maxTop: 200
                },
                hue: {
                    maxTop: 200
                },
                alpha: {
                    maxTop: 200
                }
            },
            color: '#000000',
            format: 'rgb',
            colorSelectors: {
                'black': '#000000',
                'white': '#ffffff',
                'red': '#FF0000',
                'default': '#777777',
                'primary': '#337ab7',
                'success': '#5cb85c',
                'info': '#5bc0de',
                'warning': '#f0ad4e',
                'danger': '#d9534f'
            },
            container: true,
            inline: true
        });
        
        // capture the event changeColor
        this.widgetApi.find(".picker-canvas").colorpicker().unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
    };

    // function called when the color changed
    this.changeColorButtonClick = function () {
           var self = this;
           return function (e) {
            console.log ("color changed ! :", e.color.toHex());
            KeywordManager.sendCommand(self.widget.configuration.device.keywordId, e.color.toHex().slice(1).toString());
            
        };
    };
    
    this.configurationChanged = function () {
        var self = this;

        // TODO
    };

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