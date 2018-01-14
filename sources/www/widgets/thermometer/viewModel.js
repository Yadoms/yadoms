widgetViewModelCtor =

/**
 * Create a thermometer ViewModel
 * @constructor
 */
function thermometerViewModel(){  
   this.data = "-";
   
   // default size
   this.WidgetHeight = 175;
   this.WidgetWidth  = 95;
   this.tempMax = 50;
   this.tempMin = -40;
   var isSmall = true;

   /**
    * Initialization method
    * @param widget widget class object
    */
	
   this.initialize = function(){
       //we configure the toolbar
       this.widgetApi.toolbar({
           activated: true,
           displayTitle: true,
           batteryItem: true
       });
   };
   
    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(keywordId, data){
      var self = this;	  
	  
      if (keywordId === self.widget.configuration.device.keywordId){
		  if ( data.value !== self.data ){ // refresh only when it's necessary.
			 //it is the good device
          if (data.value !== "")
			    self.data = parseFloat(data.value).toFixed(1).toString();
          else 
             self.data = "-";
			 
			 //refresh the canvas
			 self.refresh();
		  }		  
      }
   };
   
   this.refresh = function(){
	   var self = this;

      var element = self.widgetApi.find(".thermometer-canvas");

		//get a reference to the canvas
      var ctx = element.get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, element.width, element.height);
		
		//Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		element.attr('width', self.WidgetWidth);
		element.attr('height', self.WidgetHeight);
		element.css('width', self.WidgetWidth);
		element.css('height', self.WidgetHeight);
		
		//x  : initial x shouldn't change
		//y  : position y for the height of the column. 0 is the top of the frame
		//dx : the thickness of the column. should not change
		//dy : the height of the column. The final value y + dy should equal to 74 (in the colour ball).
		// min -> 0 of height
		// max -> 60 of height
      
      // Value for the physical representation
      var posYMin = 10 * self.WidgetHeight / 99;   // the highest position
      var posYMax = 80 * self.WidgetHeight / 99;	// the lowest position	
      var posYBall = (90 - (self.WidgetWidth*7/(100-1))) * (self.WidgetHeight/99);
      var posCenterBall = (91 - (self.WidgetWidth*7/(100-1))) * (self.WidgetHeight/99);
      
      //draw a circle
      ctx.beginPath();
               // position x         position y             diameter
      ctx.arc(self.WidgetWidth / 2, posCenterBall , 8 * self.WidgetWidth / 100, 0, Math.PI*2, true); 
      ctx.closePath();

      var initialPositionY = posYMax;
      
      // We draw into the thermeter only if a value is present
      if (self.data !== "-"){
         ctx.fillStyle = "rgb(" + Math.round(255-(self.tempMax-self.data)*255/90) + ",0," + Math.round(255-(self.data-self.tempMin)*255/90)+")";
         ctx.fill();
         
         var lenghtColumn = posYBall - initialPositionY;		
         
         if ((self.data>self.tempMin) && (self.data<self.tempMax) && ((self.tempMax - self.tempMin)!=0)){
            initialPositionY = posYMin + (self.tempMax-self.data)*(posYMax - posYMin)/(self.tempMax - self.tempMin);
            lenghtColumn = posYBall - initialPositionY;
         }
         else
         {
            if ( self.data <= self.tempMin ){
               initialPositionY = posYMax;
               lenghtColumn = posYBall - initialPositionY;
            }
            if (self.data > self.tempMax ){
               initialPositionY = posYMin;
               lenghtColumn = posYBall - initialPositionY;
            }			
         }
         
         ctx.fillRect( (self.WidgetWidth / 2) - (3 * self.WidgetWidth / 100), initialPositionY, 6 * self.WidgetWidth / 100, lenghtColumn );
      }
      else{
         ctx.fillStyle = "rgb(128,0,128)";
         ctx.fill();
         initialPositionY = 50;
      }
      
		if (self.WidgetWidth <= 100)
		   ctx.font="12px Georgia";
		else
		   ctx.font="20px Georgia";
   
		//write the text at the same position as the height of the column
		ctx.fillText(self.data + "°",self.WidgetWidth / 2 + 15*self.WidgetWidth / 100, initialPositionY );
		  
      // Draw the thermometer
		//draw a circle
		ctx.beginPath();
		
		//black color
      ctx.fillStyle = "rgb(0,0,0)";
		ctx.lineWidth = 3;		
		
		//bubble
		         // position x        position y             diameter
		ctx.arc(self.WidgetWidth / 2, posCenterBall, 12 * self.WidgetWidth / 100, Math.PI*1.3, Math.PI*1.7, true);
		
		//top
		ctx.arc(self.WidgetWidth / 2, 10 * self.WidgetHeight / 100, 7 * self.WidgetWidth / 100, Math.PI*2, Math.PI*1, true);
		
		ctx.closePath();
		ctx.stroke();
   };
   
   this.configurationChanged = function() {
       var self = this;

       //we register keyword new acquisition
       self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);
	   
      //we fill the deviceId of the battery indicator
      self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
      
      if (parseBool(self.widget.configuration.customYAxisMinMax.checkbox)){
         self.tempMax = parseFloat(self.widget.configuration.customYAxisMinMax.content.maximumValue);
         self.tempMin = parseFloat(self.widget.configuration.customYAxisMinMax.content.minimumValue);
      } else {
         self.tempMax = 50;
         self.tempMin = -40;
      }
      self.refresh();
   };

   this.resized = function(){
      var self = this;
	   
      if ( this.widget.getHeight() == 200 && this.widget.getWidth() == 200 ){
		   self.WidgetWidth  = 190;
		   self.WidgetHeight = 170;
		   isSmall = false;
	   }
      else if ( this.widget.getHeight() == 200 && this.widget.getWidth() == 100 ){
		   self.WidgetWidth  = 95;
		   self.WidgetHeight = 170;

	   }	   
      else if ( this.widget.getHeight() == 300 && this.widget.getWidth() == 100 ){
		   self.WidgetWidth  = 95;
		   self.WidgetHeight = 272;
	   }  
      else if ( this.widget.getHeight() == 300 && this.widget.getWidth() == 200 ){
		   self.WidgetWidth  = 190;
		   self.WidgetHeight = 272;
	   }
      else if (this.widget.getHeight() == 400 && this.widget.getWidth() == 200 ){
		   self.WidgetWidth  = 190;
		   self.WidgetHeight = 368;
	   }	   
	   else{
		   self.WidgetWidth  = 95;
		   self.WidgetHeight = 368;
		   isSmall = true;
	   }
      
      //To be painted only one time
      self.refresh();
   };
};