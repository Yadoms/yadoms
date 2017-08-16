widgetViewModelCtor =

/**
 * Create a colorRGB ViewModel
 * @constructor
 */
function colorRGBViewModel() {
    //observable data
     
     this.colorpicker = null;
     this.WidgetWidth  = 140;
     this.WidgetHeight = 140;
     
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
        
        // TODO : display colorSelector.
        // TODO : Change the place of the top/bot to hide the border
        
        // Display the color picker
        self.colorpicker = this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'cssClass', // TODO : Change the name
            sliders: { // TODO : Check how to modify theses values after the initialization
                saturation: {
                    maxLeft: 140,
                    maxTop: 140
                },
                hue: {
                    maxTop: 140
                },
                alpha: {
                    maxTop: 140
                }
            },
            slidersHorz: {
               // TODO : To be defined
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
        self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
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

        // Nothing to do
        
        //self.colorpicker.colorpicker('reposition');
    };
    
    this.changeCss = function(width, height) {
      $('.cssClass .colorpicker-saturation').css('height', height+'px');
      $('.cssClass .colorpicker-saturation').css('width', width+'px');
      $('.cssClass .colorpicker-saturation').css('background-size', width+'px '+height+'px');
      $('.cssClass .colorpicker-hue').css('height', height+'px');
      $('.cssClass .colorpicker-hue').css('background-size', '15px '+height+'px');
      $('.cssClass .colorpicker-alpha').css('height', height+'px');
      $('.cssClass .colorpicker-alpha').css('background-size', '15px '+height+'px');
      $('.cssClass .colorpicker-color').css('background-size', '15px '+height+'px');
      $('.cssClass .colorpicker-color div').css('background-size', '15px '+height+'px');
    };

   this.resized = function() 
   {
      var self = this;
      
      self.WidgetWidth = this.widget.getWidth()-60;
      self.WidgetHeight = this.widget.getHeight()-70;
      
      self.changeCss(self.WidgetWidth, self.WidgetHeight);
      
      //this.widgetApi.find(".colorpicker-selectors-visible").css('enable', 'true');
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