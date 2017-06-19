widgetViewModelCtor =

   /**
    * Create a teleInfoForecast ViewModel
    * @constructor
    */
      function teleInfoForecastViewModel() {

          // default size
          this.WidgetHeight = 70;
          this.WidgetWidth  = 95;          
          
          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
             self = this;
             
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

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              // registration of the keyword
              if  (!isNullOrUndefined(this.widget.configuration.device)) {
                self.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);                 
              }
          };

this.refresh = function(color)
   {
	   var self = this;

      var element = self.widgetApi.find(".teleinfo-forecast-canvas");

		//get a reference to the canvas
      var ctx = element.get(0).getContext("2d");
		
		//Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		element.attr('width', self.WidgetWidth);
		element.attr('height', self.WidgetHeight);
		element.css('width', self.WidgetWidth);
		element.css('height', self.WidgetHeight);
      
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, element.width, element.height);      
		  
      var centerX = self.WidgetWidth / 2;
      var centerY = self.WidgetHeight / 2;
      var radius = 30;
      
      ctx.beginPath();
      ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI, true);
      ctx.closePath();
      ctx.lineWidth = 1;
      ctx.fillStyle = color;
      ctx.fill();
      ctx.strokeStyle = 'black';
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
              
                if (keywordId === this.widget.configuration.device.keywordId) {
                    //data.value
                    if (data.value === "RED")
                    {
                       self.refresh('red');
                    }else if (data.value === "BLUE")
                    {
                       self.refresh('blue');
                    }else if (data.value === "WHITE")
                    {
                       self.refresh('white');
                    }else
                    {
                       self.refresh('grey');
                    }
                }
           }
        }
    };
};