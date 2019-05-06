widgetViewModelCtor =

   /**
    * Create a teleInfoForecast ViewModel
    * @constructor
    */
      function teleInfoForecastViewModel() {
		  
          //observable data
          this.bg_color = ko.observable("");

          // default size
          this.WidgetHeight = 95;
          this.WidgetWidth  = 95;          
          
          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
             var self = this;
             
             //we configure the toolbar
             self.widgetApi.toolbar({
                 activated: true,
                 displayTitle: true,
                 batteryItem: true
             });
          };

          /**
           * ConfigurationChanged method
           * @param none
           */          
          this.configurationChanged = function () {
              var self = this;
              var readOnlyMode=false;
              var arrayOfDeffered = [];

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)) || isNullOrUndefined(this.widget.configuration.device)) )
                  return;

              //we register keyword new acquisition
              self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
      
              //we register keyword for get last value at web client startup 
              self.widgetApi.getLastValue(self.widget.configuration.device.keywordId);
          };

         this.refresh = function()
         {
            var self = this;

            var element = self.widgetApi.find(".teleinfo-forecast-canvas");

            //get a reference to the canvas
            var ctx = element.get(0).getContext("2d");
            
            // Refresh the canvas, clear all existing information
            ctx.clearRect(0, 0, element.width, element.height);
            
            //Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
            element.attr('width', self.WidgetWidth);
            element.attr('height', self.WidgetHeight);
            element.css('width', self.WidgetWidth);
            element.css('height', self.WidgetHeight);
              
              // Draw the thermometer
            //draw a circle
            ctx.beginPath();
            
            //black color
            ctx.fillStyle = "rgb(0,0,0)";
            ctx.lineWidth = 3;		
            
            //bubble
                     // position x        position y             diameter
            ctx.arc(self.WidgetWidth / 2, self.WidgetHeight / 2, 12 * self.WidgetWidth / 2, Math.PI*1.3, Math.PI*1.7, true);
            
            //top
            ctx.arc(self.WidgetWidth / 2, 10 * self.WidgetHeight / 2, 7 * self.WidgetWidth / 100, Math.PI*2, Math.PI*1, true);
            
            ctx.closePath();
            ctx.stroke();
         }
          
          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;
              
              if (!isNullOrUndefined(this.widget.configuration)){

                 //Check if first device
                 if (!isNullOrUndefined(this.widget.configuration.device)) {
                    
                      if (keywordId === parseInt(this.widget.configuration.device.keywordId)) {
                         
                          //data.value
                          if (data.value === "RED")
                          {
                             self.bg_color("red");
                          }else if (data.value === "BLUE")
                          {
                             self.bg_color("blue");
                          }else if (data.value === "WHITE")
                          {
                             self.bg_color("white");
                          }else
                          {
                             self.bg_color("grey");
                          }
                      }
                 }
              }
          };
      };