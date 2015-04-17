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
   this.AveHumidityVisible = ko.observable( true );
   this.RainDayVisible = ko.observable    ( true );

   //Nbre of day to be displayed
   this.DayNbre = ko.observable ( 10 );
   
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
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
    * @param deviceId
    */
   this.dispatch = function(device, data) {
      var self = this;

      if (device == self.widget.configuration.device) 
      {  
		 var obj = jQuery.parseJSON( data.value );
		 
		 console.debug( "objet reçu %o",obj );
		 
		 //We only keep the city name
		 var res = obj.city.split(",");
		 self.city ( res[0] );
		 
		 //We delete all information already keep in.
		 self.period.removeAll();
		 
		 self.DayNbre ( obj.forecast.length );
		 
		 //Copy of all object into the temporary array
		 $.each(obj.forecast, function (i, object)
		 {
				self.TempPeriod.push({ WeatherCondition: obj.forecast[i].WeatherCondition,
									   TimeDate: obj.forecast[i].Day + '/' + obj.forecast[i].Month,
									   TempMax: obj.forecast[i].TempMax + "\u00B0", // Un caractère parasite est généré par boost::write_json sous Ubuntu \u00B0 = ° obj.Units.temperature.substring(0, obj.Units.temperature.length - 1),
									   TempMin: obj.forecast[i].TempMin + "\u00B0", //obj.Units.temperature.substring(0, obj.Units.temperature.length - 1),
									   MaxWind: Convertmstokmh(parseFloat(obj.forecast[i].MaxWind,10)) + "km/h", 
									   AveWind: Convertmstokmh(parseFloat(obj.forecast[i].AveWind,10)) + "km/h",
									   AveHumidity: obj.forecast[i].AveHumidity + obj.Units.humidity,
									   RainDay: obj.forecast[i].RainDay + obj.Units.rain,
									   WeatherIcon: "widgets/forecast/images/Icons1/" + obj.forecast[i].WeatherCondition + ".png"
									 });
         }	 
         );
		 
		 //Resize the widget and display the elements automatically
		 self.resized ();
      }
	  
	  this.configurationChanged ();
   };

   this.configurationChanged = function() {
      var self = this;
	  
	 if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
		return;	  
	 
	 try
	 {
	    self.MaxWindVisible     ( parseBool( self.widget.configuration.Information.content.MaxWind ));
	    self.AveWindVisible     ( parseBool( self.widget.configuration.Information.content.AveWind ));
	    self.AveHumidityVisible ( parseBool( self.widget.configuration.Information.content.AveHumidity ));
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
   
	   if (this.widget.height() <= 200) 
	   {
	         //In one case : we keep only the icon
	         self.MaxTempVisible     ( false );
			 self.MinTempVisible     ( false );
			 self.RainDayVisible     ( false );
			 self.AveHumidityVisible ( false );
			 self.AveWindVisible     ( false );
			 self.MaxWindVisible     ( false );
	   }
	   else
	   {
	         //In two cases : we enable automatically temperatures. All others information are read from the configuration
	         self.MaxTempVisible     ( true );
			 self.MinTempVisible     ( true );	   
			 self.RainDayVisible     ( parseBool (self.widget.configuration.Information.content.RainDay ));
			 self.AveHumidityVisible ( parseBool (self.widget.configuration.Information.content.AveHumidity ));
			 self.AveWindVisible     ( parseBool (self.widget.configuration.Information.content.AveWind ));
			 self.MaxWindVisible     ( parseBool (self.widget.configuration.Information.content.MaxWind ));
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
	   else  // Otherwise 10 days
	   {
	      self.DayNbre ( 10 );   
	   }
	   
	  self.period.removeAll();
	  self.period ( self.TempPeriod.slice ( 0, self.DayNbre() ));		   
   };
 
   this.getDevicesToListen = function() {
      var result = [];

      //Add the keyword Forecast
      result.push(this.widget.configuration.device);

      return result;
   }
};
