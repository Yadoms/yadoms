widgetViewModelCtor =

/**
 * Create a colorPicker ViewModel
 * @constructor
 */
function colorPickerViewModel() {
     this.colorpicker = null;
     this.WidgetWidth  = 156;
     this.WidgetHeight = 120;
     this.capacity = "";
     
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
    };

    //
    // TODO : Bug with 2 widgets. Need to have a unique name !
    //
    
    this.createPicker = function (preselectedColor) {
        self.colorpicker = this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'colorpicker-size',
            hexNumberSignPrefix: false,
            sliders: {
                saturation: {
                    maxLeft: 156,
                    maxTop: 120
                },
                hue: {
                    maxTop: 120
                },
                alpha: {
                    maxTop: 120
                }
            },
            color: '#000000',
            format: 'rgb',

            colorSelectors: preselectedColor,
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
               
            if (self.capacity === "colorrgb")
               KeywordManager.sendCommand(self.widget.configuration.device.keywordId, e.color.toHex().toString());
            else if (self.capacity === "colorrgbw")
            {
               var temp = e.color.toHex().toString();
               var red = temp.substring(0, 2);
               var green = temp.substring(2, 4);
               var blue = temp.substring(4, 6);
               rgbw = self.convertRGBtoRGBW(parseInt(red, 16), 
                                            parseInt(green, 16),
                                            parseInt(blue, 16));
               console.log (rgbw);
               var RGBWString = rgbw.red.toString(16)+rgbw.green.toString(16)+rgbw.blue.toString(16)+rgbw.white.toString(16);
               console.log (RGBWString);
               KeywordManager.sendCommand(self.widget.configuration.device.keywordId, RGBWString);
            }
            else
               console.warn("This capacity is not supported !");
        };
    };
    
    //
    // http://219.223.223.150/ldm/images/papers/Advanced_RGBW_OLED_Display_System_with_Novel_RGB-to-RGBW_and_Subpixel_Rendering_Algorithm.pdf
    // Algorithm of Kwon and Kim
    //
    
    this.convertRGBtoRGBW = function (red, green, blue) {
       
       var Q = 255;
       
      console.log ("red:",red);
      console.log ("green",green);
      console.log ("blue",blue);
       
       var max = Math.max(red/Q, green/Q, blue/Q);
       var min = Math.min(red/Q, green/Q, blue/Q);
       
       console.log ("min",min);
       console.log ("max",max);
       
       var W0 = -min*min*min+min*min+min;
       console.log ("W0", W0);
       
       var K = 1+W0/max;
       console.log ("K", K);
       
       var value = {};
       value["white"] = Math.floor (W0*Q);
       value["red"]   = Math.floor (((K*red/Q)-W0)*Q);
       value["green"] = Math.floor (((K*green/Q)-W0)*Q);
       value["blue"]  = Math.floor (((K*blue/Q)-W0)*Q);
       
       return value;
    };
    
    this.configurationChanged = function () {
       var self = this;
       var preselectedColor = null;

       if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
          return;
       
       //we register keyword new acquisition
       self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);       
       
       //we ask the capacity Name
       var deffered2 = self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId);
       deffered2.done(function (keyword) {
          self.capacity = keyword.capacityName;
       });
       
       // destroy the precedent colorPicker if any
       // it's the only solution, to create/delete preselected colors
       if (!isNullOrUndefined(self.colorpicker)) self.colorpicker.colorpicker('destroy');
       
       if (!(isNullOrUndefinedOrEmpty(self.widget.configuration.colorsSelection)))
       {
          if (self.widget.configuration.colorsSelection.length>0) preselectedColor={};
          self.widget.configuration.colorsSelection.forEach(function (item) {
              var newProperties={};
              newProperties[item.content.name] = item.content.color;
              $.extend(preselectedColor, newProperties);
          });
       }
       
       self.createPicker(preselectedColor);
       self.resized();
       
       return deffered2.promise();
    };
    
    this.changeCss = function(width, height) {
      this.widgetApi.find(".colorpicker-saturation").css('height', height+'px');
      this.widgetApi.find(".colorpicker-saturation").css('width', width+'px');
      this.widgetApi.find(".colorpicker-saturation").css('background-size', width+'px '+height+'px');
      this.widgetApi.find(".colorpicker-hue").css('height', height+'px');
      this.widgetApi.find(".colorpicker-hue").css('background-size', '15px '+height+'px');
      this.widgetApi.find(".colorpicker-alpha").css('height', height+'px');
      this.widgetApi.find(".colorpicker-alpha").css('background-size', '14px '+height+'px');
      this.widgetApi.find(".colorpicker-color").css('background-size', '14px '+height+'px');
      this.widgetApi.find(".colorpicker-color div").css('background-size', '14px '+height+'px');
      this.widgetApi.find(".colorpicker-selectors").css('width', width+15+'px');
    };

   this.resized = function() 
   {
      var self = this;
      self.WidgetWidth = this.widget.getWidth()-34;
      
      // If no colors preselected, we use all the space
      if (isNullOrUndefined(self.colorpicker.data('colorpicker').options.colorSelectors))
         self.WidgetHeight = this.widget.getHeight()-57;
      else
         self.WidgetHeight = this.widget.getHeight()-90;
      
      // Update sliders values
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxLeft = self.WidgetWidth;
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxTop = self.WidgetHeight;
      
      self.colorpicker.data('colorpicker').options.sliders.hue.maxTop = self.WidgetHeight;
      self.colorpicker.data('colorpicker').options.sliders.alpha.maxTop = self.WidgetHeight;      
      
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
          
          // unbind the changeColor, otherwise, firea 'changeColor'
          self.colorpicker.unbind('changeColor');
          if (self.capacity === "colorrgb")
             self.colorpicker.colorpicker('setValue', data.value);
          else if (self.capacity === "colorrgbw")
          { // TODO : To be finalize RGBW -> RGB conversion to be write
          }
          else
            console.warn("This capacity is not supported !");          
          self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
        }
    };
};