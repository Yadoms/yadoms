widgetViewModelCtor =

/**
 * Create a thermometer ViewModel
 * @constructor
 */
function ThermometerViewModel() 
{  
   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   
   //simple data
   this.WidgetHeight = 100;
   this.WidgetWidth  = 100;
   
   var isSmall = true;
	 
   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
	
   this.initialize = function(widget) 
   {
      this.widget = widget;
	  
	  var elementID = "widget-thermometer-" + this.widget.id; // Unique ID
	  
	  // Initialisation of a unique canvas associated to this widget
	   	$('<canvas />').attr({
		id: elementID
	    }).appendTo( "#widget-" + this.widget.id );
   };
   
   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) 
   {
      var self = this;	  
	  
      if (device == self.widget.configuration.device) 
	  {
		  if ( data.value != self.data() ) // refresh only when it's necessary.
		  {
			 //it is the good device
			 self.data(data.value);
			 
			 //refresh the canvas
			 self.refresh();
		  }
      }
   };
   
   this.refresh = function()
   {
	   var self = this;
	
	    var elementID = "widget-thermometer-" + this.widget.id; // Unique ID
	
		//get a reference to the canvas
		var ctx = $( "#" + elementID ).get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, self.WidgetWidth, self.WidgetHeight );
		
		//Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		$("#" + elementID ).attr('width' , self.WidgetWidth  );
        $("#" + elementID ).attr('height', self.WidgetHeight );

		//x  : initial x shouldn't change
		//y  : position y for the height of the column. 0 is the top of the frame
		//dx : the thickness of the column. should not change
		//dy : the height of the column. The final value y + dy should equal to 74 (in the colour ball).
		// -40° -> 10 of height
		// 50°  -> 60 of height
		
		// Value 
		var TEMP_MAX = self.widget.configuration.customYAxisMinMax.content.maximumValue;
		var TEMP_MIN = self.widget.configuration.customYAxisMinMax.content.minimumValue;
		
		// Value for the physical representation
		var POS_Y_MIN = 10 * self.WidgetHeight / 100;
		var POS_Y_MAX = 60 * self.WidgetHeight / 100;		
		var POS_Y_BALL = 84 * self.WidgetHeight / 100;
		var POS_CENTER_BALL = 86 * self.WidgetHeight / 100;
		
        ctx.fillStyle = "rgb(" + Math.round(255 - ( TEMP_MAX - self.data())* 255/90) + ",0," + Math.round( 255 - ( self.data() - TEMP_MIN ) * 255/90 ) + ")"
		 
		//draw a circle
		ctx.beginPath();
		         // position x         position y             diameter
		ctx.arc(self.WidgetWidth / 2, POS_CENTER_BALL , 8 * self.WidgetWidth / 100, 0, Math.PI*2, true); 
		ctx.closePath();
		ctx.fill();
		
		var initial_position_y = POS_Y_MAX;
		var lenght_column = POS_Y_BALL - initial_position_y;		
		
		if (self.data() > TEMP_MIN && self.data() < TEMP_MAX)
		{
			initial_position_y = POS_Y_MIN + ( TEMP_MAX - self.data() )*( POS_Y_MAX - POS_Y_MIN )/ ( TEMP_MAX - TEMP_MIN) ;
			lenght_column = POS_Y_BALL - initial_position_y;
		}
		else
		{
			if ( self.data() <= TEMP_MIN )
			{
				initial_position_y = POS_Y_MAX;
				lenght_column = POS_Y_BALL - initial_position_y;
			}
			if (self.data() > TEMP_MAX )
			{
				initial_position_y = POS_Y_MIN;
				lenght_column = POS_Y_BALL - initial_position_y;
			}			
		}
		
		ctx.fillRect( (self.WidgetWidth / 2) - (3 * self.WidgetWidth / 100), initial_position_y, 6 * self.WidgetWidth / 100, lenght_column );

		if (self.WidgetWidth == 100)
		{
		   ctx.font="12px Georgia";
		}
		if (self.WidgetWidth == 200)
		{
		   ctx.font="20px Georgia";
		}
   
		//write the text at the same position as the height of the column
		ctx.fillText(self.data() + "°",self.WidgetWidth / 2 + 15*self.WidgetWidth / 100, initial_position_y );
		  
        // Draw the thermometer
		//draw a circle
		ctx.beginPath();
		
		//black color
        ctx.fillStyle = "rgb(0,0,0)";
		ctx.lineWidth = 3;		
		
		//bubble
		         // position x        position y             diameter
		ctx.arc(self.WidgetWidth / 2, POS_CENTER_BALL, 12 * self.WidgetWidth / 100, Math.PI*1.3, Math.PI*1.7, true);
		
		//top
		ctx.arc(self.WidgetWidth / 2, 10 * self.WidgetWidth / 100, 7 * self.WidgetWidth / 100, Math.PI*2, Math.PI*1, true);
		
		ctx.closePath();
		ctx.stroke();
   }
   
   this.configurationChanged = function() 
   {
      var self = this;
   };

   this.resized = function() 
   {
       var self = this;
	   
	   //The size is x2 only when the widget is square
	   if (this.widget.height() <= 250 && this.widget.height() >= 150 && this.widget.width() <= 250 && this.widget.width() >= 150 )
	   {
		   self.WidgetWidth  = 200;
		   self.WidgetHeight = 200;
		   
		   self.refresh();
		   isSmall = false;
	   }
	   else if (this.widget.height() <= 250 && this.widget.height() >= 150 && this.widget.width() >= 80 && this.widget.width() <= 110 )
	   {
		   self.WidgetWidth  = 100;
		   self.WidgetHeight = 200;
		   
		   //To be painted only one time
		   self.refresh();
	   }	   
	   else if (this.widget.height() <= 350 && this.widget.height() >= 250 && this.widget.width() >= 80 && this.widget.width() <= 110 )
	   {
		   self.WidgetWidth  = 100;
		   self.WidgetHeight = 300;
		   
		   //To be painted only one time
		   self.refresh();
	   }  
	   else if (this.widget.height() <= 350 && this.widget.height() >= 250 && this.widget.width() >= 180 && this.widget.width() <= 210 )
	   {
		   self.WidgetWidth  = 200;
		   self.WidgetHeight = 300;
		   
		   //To be painted only one time
		   self.refresh();
	   }	   
	   else
	   {
		   self.WidgetWidth  = 100;
		   self.WidgetHeight = 100;
		   
		   self.refresh();
		   isSmall = true;
	   }
   };	   
  
   this.getDevicesForAcquisitions = function() 
   {
      var result = [];

      //Add the keyword
      result.push(this.widget.configuration.device);
	  
	  return result;
   }
};