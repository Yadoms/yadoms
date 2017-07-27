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
        console.log (this.widgetApi.find(".picker-canvas"));
        this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'colorpicker-2x',
            sliders: {
                saturation: {
                    maxLeft: 150,
                    maxTop: 150
                },
                hue: {
                    maxTop: 150
                },
                alpha: {
                    maxTop: 150
                }
            },            
            color: '#ffaa00',
            container: true,
            inline: true
        });
        
        // capture the event changeColor
        this.widgetApi.find(".picker-canvas").colorpicker().on('changeColor', function(e) {
           
            console.log ("color changed !");
           
            //$('body')[0].style.backgroundColor = e.color.toString(
            //    'rgba');
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

/*
<input id="cp9" type="text" class="form-control" value="pink" />
<style>
    .colorpicker-2x .colorpicker-saturation {
        width: 200px;
        height: 200px;
    }

    .colorpicker-2x .colorpicker-hue,
    .colorpicker-2x .colorpicker-alpha {
        width: 30px;
        height: 200px;
    }

    .colorpicker-2x .colorpicker-color,
    .colorpicker-2x .colorpicker-color div {
        height: 30px;
    }
</style>
<script>
    $(function() {
        $('#cp9').colorpicker({
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
            }
        });
    });
</script>
*/