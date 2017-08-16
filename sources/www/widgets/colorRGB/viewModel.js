widgetViewModelCtor =

/**
 * Create a colorRGB ViewModel
 * @constructor
 */
function colorRGBViewModel() {
    //observable data
     
     this.colorpicker = null;
     this.WidgetWidth  = 156;
     this.WidgetHeight = 140;
     
    /**
     * Initialization method
     * @param widget widget class object
     */     
    this.initialize = function () {
        self = this;
       
        //TODO : Register predefines colors -> In the widget configuration ?
       
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: false
        });
        
        // Display the color picker
        self.colorpicker = this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'colorpicker-size',
            hexNumberSignPrefix: false,
            sliders: {
                saturation: {
                    maxLeft: 156,
                    maxTop: 140
                },
                hue: {
                    maxTop: 140
                },
                alpha: {
                    maxTop: 140
                }
            },
            color: '#000000',
            format: 'rgb',
            colorSelectors: {      // TODO : to be modified
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
            // remove the dropdown-menu class to disable the border
            template : "<div class='colorpicker'><div class='colorpicker-saturation'><i><b></b></i></div><div class='colorpicker-hue'><i></i></div><div class='colorpicker-alpha'><i></i></div><div class='colorpicker-color'><div /></div><div class='colorpicker-selectors'></div></div>",
            container: true,
            inline: true
        });
        
        // capture the event changeColor
        self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
    };

    // function called when the color changed
    this.changeColorButtonClick = function () {
           var self = this;
           return function (e) {
            console.log ("color changed ! :", e.color.toHex());
            KeywordManager.sendCommand(self.widget.configuration.device.keywordId, e.color.toHex()/*.slice(1)*/.toString());      
        };
    };
    
    this.configurationChanged = function () {
        var self = this;

        // Nothing to do
    };
    
    this.changeCss = function(width, height) {
      $('.widget-colorRGB .colorpicker-saturation').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-saturation').css('width', width+'px');
      $('.widget-colorRGB .colorpicker-saturation').css('background-size', width+'px '+height+'px');
      $('.widget-colorRGB .colorpicker-hue').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-hue').css('background-size', '15px '+height+'px');
      $('.widget-colorRGB .colorpicker-alpha').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-alpha').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-color').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-color div').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-selectors').css('width', width+15+'px');
    };

   this.resized = function() 
   {
      var self = this;
      
      console.log ("resized !");
      
      self.WidgetWidth = this.widget.getWidth()-34;
      self.WidgetHeight = this.widget.getHeight()-70;
      
      // Update sliders values
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxLeft = self.WidgetWidth;
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxTop = self.WidgetHeight;
      
      self.colorpicker.data('colorpicker').options.sliders.hue.maxTop = self.WidgetHeight;
      self.colorpicker.data('colorpicker').options.sliders.alpha.maxTop = self.WidgetHeight;      
      
      console.log (self.colorpicker.data('colorpicker'));
      
      self.changeCss(self.WidgetWidth, self.WidgetHeight);
      
      self.colorpicker.colorpicker('update');
   };	
    
    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === self.widget.configuration.device.keywordId) {
          console.log(data);
          self.colorpicker.setColor(data.data);
        }
    };
};