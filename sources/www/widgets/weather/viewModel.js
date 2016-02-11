function Convertmstokmh( value ){
return value*3.6;
}

widgetViewModelCtor =

/**
 * Create a Weather ViewModel
 * @constructor
 */
function WeatherViewModel() {

   //observable data
   this.data = ko.observable("");
   this.city = ko.observable("");
   this.WeatherIcon = ko.observable("");
   this.Temp = ko.observable("");
   this.WindCanvasId = ko.observable("");
   
   this.dateformat = "";
   this.lastUpdate = "";
   
   //Height of the widget.
   this.height = 0; 
   
   this.WidgetWidth  = 180;
   this.WidgetHeight = 62;
      
   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
     this.widget = widget;
	  
	 var self = this;
	 
     self.WindCanvasId ( "widget-weather-" + this.widget.id );	 
	 
	  var elementID = "widget-weather-" + this.widget.id; // Unique ID
	  
	  // Initialisation of a unique canvas associated to this widget
	   	$('<canvas />').attr({
		id: elementID
	    }).appendTo( "#widget-" + this.widget.id );	 
		
		 $("#" + self.WindCanvasId() ).attr('width' , self.WidgetWidth  );
         $("#" + self.WindCanvasId() ).attr('height', self.WidgetHeight );
		 $("#" + self.WindCanvasId() ).css('overflow', 'hidden' );
   }; 
 
   /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(keywordId, data) {
      var self = this;
	  
      if (keywordId == self.widget.configuration.device.keywordId) 
      {  
		 var obj = jQuery.parseJSON( data.value );
		 
		 console.log ( obj );
		 
		 //We only keep the city name
		 var res = obj.city.split(",");
		 self.city ( res[0] );
		 
		 self.WeatherIcon( "widgets/weather/images/Icons1/" + obj.Conditions.WeatherCondition + ".png");
		 self.Temp ( obj.Conditions.Temp + $.t(obj.Units.temperature) ); 		
		 
		 self.paint();

		 //Send update information to the HMI, on if the date is new
    	 if (self.lastUpdate != data.date._i)
		 {
		    self.lastUpdate = data.date._i;
		 }
      }
   };
   
   this.paint = function ()
   {
	   var self = this;
	   
		 //get a reference to the canvas
		 var ctx = $( "#" + self.WindCanvasId() ).get(0).getContext("2d");
		 
		 // Refresh the canvas, clear all existing information
		 ctx.clearRect(0, 0, self.WidgetWidth, self.WidgetHeight );
		
		 //Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		 $("#" + self.WindCanvasId() ).attr('width' , self.WidgetWidth  );
         $("#" + self.WindCanvasId() ).attr('height', self.WidgetHeight );
		
		//Draw the image
		var imageObj = new Image();
		imageObj.src = self.WeatherIcon();
		console.log ( imageObj ); //self.WeatherIcon()
		
		imageObj.onload = function () {
             ctx.drawImage ( imageObj, 0, 0, self.WidgetHeight, self.WidgetHeight );
        }
  
		//write the text at the same position as the height of the column
		ctx.font="bold 20px Georgia";
		ctx.fillText(self.Temp(),6*self.WidgetWidth / 10, self.WidgetHeight / 2 );
		
		//write the text at the same position as the height of the column
		ctx.font="18px Georgia";
		ctx.fillText(self.city(),(self.WidgetWidth - 50) / 2, 9*self.WidgetHeight / 10 );		
   };

   this.configurationChanged = function() {
      var self = this;
	  
	 if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
		return;	  
	 
     //we register keyword new acquisition
	 WidgetApi.keyword.registerKeywordAcquisitions(self.widget, self.widget.configuration.device.keywordId);

	 try
	 {
		//Read the date format
		self.dateformat = self.widget.configuration.DateFormat;
	 }
	 catch(err) 
	 {
	    console.debug( err.message );
	 }
   }
 
   this.resized = function() 
   {
       var self = this;
   
	   if (self.widget.height() <= 120) 
	   {
           self.WidgetWidth  = 180;
		   self.WidgetHeight = 62;
	   }
	   else if ( (self.widget.height() <= 220) && (self.widget.height() > 190) )
	   { 
           self.WidgetWidth  = 280;
		   self.WidgetHeight = 165;
	   }
	   else 
	   {
           self.WidgetWidth  = 180;
		   self.WidgetHeight = 62;	       
	   }	   
	   
		self.paint();
   };
};