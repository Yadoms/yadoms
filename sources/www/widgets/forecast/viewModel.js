function Convertmstokmh( value ){
return value*3.6;
}

widgetViewModelCtor =

/**
 * Create a Forecast ViewModel
 * @constructor
 */
function ForecastViewModel() {

   //observable data
   this.data = ko.observable("");
   this.city = ko.observable("");
   
   //Default value - This value is overwrite after
   this.period     = ko.observableArray();
   
   //Definition of the temporary array
   this.TempPeriod = new Array();

   this.MaxTempVisible = ko.observable    ( true );
   this.MinTempVisible = ko.observable    ( true );
   this.MaxWindVisible = ko.observable    ( true );
   this.AveWindVisible = ko.observable    ( true );
   this.RainDayVisible = ko.observable    ( true );
   
   //Nbre of day to be displayed
   this.DayNbre = ko.observable ( 10 );
   
   //Height of the widget.
   this.height = 0; 
      
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
   };
   
   /**
    * Draw in circle the speed and the direction of the wind
    * @data device identifier which make the values
    * @WindPosition Direction from the Wind
    * @WindSpeed    Speed of the Wind
    */   
   
	this.canvasload = function( data, WindPosition, WindSpeed ) 
	{		
		var self = this;
		
		//get a reference to the canvas
		var ctx = $( "#" + data ).get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, 40, 40 );
		
		ctx.fillStyle = "rgb(0,0,0)"; // black
		 
		//draw a circle
		ctx.beginPath();
				 
		ctx.arc(20, 20, 17, 0, Math.PI*2, true); 		
		ctx.closePath();
		
		ctx.lineWidth = 1;
		ctx.strokeStyle = '#000000';
		ctx.stroke();		
		
		ctx.font="14px Georgia";
		
		//write the text at the same position as the height of the column
		ctx.fillText( WindSpeed ,20 - ( 7 * String(WindSpeed).match(/\d/g).length ) /2 , 23);
		ctx.stroke();
		
		//triangle of the direction
		ctx.beginPath();
		ctx.moveTo(20 - 12 * Math.sin ( Math.PI/180 * parseInt(WindPosition) )     , 20 - 12 * Math.cos ( Math.PI/180 * parseInt(WindPosition) ));
		ctx.lineTo(20 - 20 * Math.sin ( Math.PI/180 * (parseInt(WindPosition)+10) ), 20 - 20 * Math.cos ( Math.PI/180 * (parseInt(WindPosition)+10) ));
		ctx.lineTo(20 - 20 * Math.sin ( Math.PI/180 * (parseInt(WindPosition)-10) ), 20 - 20 * Math.cos ( Math.PI/180 * (parseInt(WindPosition)-10) ));
		ctx.closePath();
		ctx.fill();
		ctx.stroke();
	};
   
   this.RainCanvasLoad = function( data, RainValue )
	{
		var self = this;
			
		//get a reference to the canvas
		var ctx = $( "#" + data ).get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, 40, 40 );
	
		ctx.fillStyle = "rgb(116,208,241)"; // blue Azur clair
		
		if ( RainValue < 40 )
			position_y = 40 - parseInt( RainValue );
		else
			position_y = 0;
		
		ctx.fillRect(0, position_y , 40, 40);
		ctx.stroke();

		ctx.fillStyle = "rgb(0,0,255)"; // black
		
		//TODO : Voir pour récupérer la valeur de la classe font-family et font size tout simplement !
		ctx.font="14px Georgia";
		
		//write the text at the same position as the height of the column
		ctx.fillText( RainValue ,20 - ( 7 * String(RainValue).match(/\d/g).length ) /2 , 23);
		ctx.stroke();
	};
   
   /**
    * New acquisition handler
    * @param device Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) {
      var self = this;
	  
      if (device == self.widget.configuration.device) 
      {  
		 var obj = jQuery.parseJSON( data.value );
		 
		 //We only keep the city name
		 var res = obj.city.split(",");
		 self.city ( res[0] );
		 
		 //We delete all information already keep in.
		 while( self.TempPeriod.length > 0 ) 
		 {
            self.TempPeriod.pop();
         }
		 
		 self.DayNbre ( obj.forecast.length );
		 
		 //Copy of all object into the temporary array
		 $.each(obj.forecast, function (i, object)
		 {
			    // create the name for each div where wind canvas will be attached
			    var elementID = 'widget-' + self.widget.id + '-column-' + i;
				
				// create the name for each div where rain canvas will be attached
				var RainElementID = 'widget-' + self.widget.id + '-rain-' + i;

				// create the name for each div where rain canvas will be attached
				var SnowElementID = 'widget-' + self.widget.id + '-snow-' + i;
				
				self.TempPeriod.push({ WeatherCondition: obj.forecast[i].WeatherCondition,
									   TimeDate: moment( obj.forecast[i].Day + "-" + obj.forecast[i].Month, "DD-MM").format('LL') ,//obj.forecast[i].Day + '/' + obj.forecast[i].Month,
									   TempMax: obj.forecast[i].TempMax + "\u00B0", // Un caractère parasite est généré par boost::write_json sous Ubuntu \u00B0 = ° obj.Units.temperature.substring(0, obj.Units.temperature.length - 1),
									   TempMin: obj.forecast[i].TempMin + "\u00B0", //obj.Units.temperature.substring(0, obj.Units.temperature.length - 1),
									   MaxWind: Convertmstokmh(parseFloat(obj.forecast[i].MaxWind,10)), 
									   AveWind: Convertmstokmh(parseFloat(obj.forecast[i].AveWind,10)),
									   AveWindDegrees: obj.forecast[i].AveWindDegrees,
									   WindCanvasId: elementID,
									   //AveHumidity: obj.forecast[i].AveHumidity + obj.Units.humidity,
									   RainCanvasId: RainElementID,
									   //SnowCanvasId: SnowElementID,
									   RainDay: obj.forecast[i].RainDay, 
									   //SnowDay: obj.forecast[i].SnowDay, 
									   WeatherIcon: "widgets/forecast/images/Icons1/" + obj.forecast[i].WeatherCondition + ".png"
									 });
         } 
         );

		 //Resize the widget and display the elements automatically
		 self.resized ();
      }
   };

   this.configurationChanged = function() {
      var self = this;
	  
	 if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
		return;	  
	 
	 try
	 {
        self.MaxTempVisible     ( true );
        self.MinTempVisible     ( true );			 
	    self.MaxWindVisible     ( parseBool( self.widget.configuration.Information.content.MaxWind ));
	    self.AveWindVisible     ( parseBool( self.widget.configuration.Information.content.AveWind ));
   	    self.RainDayVisible     ( parseBool( self.widget.configuration.Information.content.RainDay ));	
	 }
	 catch(err) 
	 {
	    console.debug( err.message );
	 }
   }
 
   this.resized = function() 
   {
       var self = this;
   
	   if (self.widget.height() <= 150) 
	   {
	         //In one case : we keep only the icon
	         self.MaxTempVisible     ( false );
			 self.MinTempVisible     ( false );
			 self.RainDayVisible     ( false );
			 self.AveWindVisible     ( false );
			 self.MaxWindVisible     ( false );
	   }
	   else if ( (self.widget.height() <= 220) && (self.widget.height() > 190) )
	   {   
		     self.height = 20; // Location 
		     self.height = self.height + 20; // Size of the day
		     self.height = self.height + 40; //Size of the icon of the day
			 
	         //In two cases : we enable automatically temperatures. All others information are read from the configuration
	         self.MaxTempVisible     ( true );
			 self.height = self.height + 20; //Size of the maximum temperature
			 self.MinTempVisible     ( true );
			 self.height = self.height + 20; //Size of the minimum temperature

			 self.AveWindVisible     ( parseBool (self.widget.configuration.Information.content.AveWind ));
			 if (self.AveWindVisible() )
			    self.height = self.height + 40; //Size of the Wind Canvas
			 
			 self.MaxWindVisible     ( parseBool (self.widget.configuration.Information.content.MaxWind ));
			 if (self.MaxWindVisible() )
			    self.height = self.height + 20; //Size of the Maximum Wind
			 
			 if ( ((self.height + 40) <= 200) && parseBool (self.widget.configuration.Information.content.RainDay ) ) 
			 {
			    self.RainDayVisible     ( true );
			    self.height = self.height + 40; //Size of the rain canvas
			 }
			 else
			    self.RainDayVisible ( false );
	   }
	   else if ( (self.SizePainted != 3) && (self.widget.height() > 220) )
	   {
	         //In three cases : we enable automatically temperatures. All others information are read from the configuration
			self.MaxTempVisible     ( true );
			self.MinTempVisible     ( true );			 
			self.MaxWindVisible     ( parseBool( self.widget.configuration.Information.content.MaxWind ));
			self.AveWindVisible     ( parseBool( self.widget.configuration.Information.content.AveWind ));
			self.RainDayVisible     ( parseBool( self.widget.configuration.Information.content.RainDay ));
	   }
	   
	   // if length = 2 cases -> 2 days
	   if (self.widget.width() <= 200)
	   {
	      self.DayNbre ( 1 );
	   }
	   else if (self.widget.width() <= 300) // if length = 3 cases -> 3 days
	   {
	      self.DayNbre ( 3 );
	   }
	   else if (self.widget.width() <= 400) // if length = 4 cases -> 5 days
	   {
	      self.DayNbre ( 5 );
	   }
	   else if (self.widget.width() <= 500) // if length = 5 cases -> 6 days
	   {
	      self.DayNbre ( 6 );
	   }
	   else if (self.widget.width() <= 600) // if length = 6 cases -> 8 days
	   {
	      self.DayNbre ( 8 );
	   }		   
	   else if (self.DayNbre() != 10)  // Otherwise 10 days
	   {
	      self.DayNbre ( 10 );
	   }
	   
	  self.period.removeAll();
	  self.period ( self.TempPeriod.slice ( 0, self.DayNbre() ));
   };
 
   this.getDevicesForAcquisitions = function() {
      var result = [];

      //Add the keyword Forecast
      result.push(this.widget.configuration.device);

      return result;
   }
};