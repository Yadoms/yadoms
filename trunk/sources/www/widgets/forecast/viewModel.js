//TODO : Faire un affichage partiel suivant la taille du module et le nombre d'éléments affichés.
//TODO : Voir quoi faire avec obj.Units.speed

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
   this.period = ko.observableArray();
	 
   this.MaxWindVisible = ko.observable( true );
   this.AveWindVisible = ko.observable( true );
   this.AveHumidityVisible = ko.observable( true );
   this.RainDayVisible = ko.observable( true );

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
		 
		 var res = obj.city.split(",");
		 
		 self.city ( res[0] );
		 self.period.removeAll();
		 
		 $.each(obj.forecast, function (i, object) 
		 {
		    self.period.push({ WeatherCondition: obj.forecast[i].WeatherCondition,
			                   TimeDate: obj.forecast[i].Day + '/' + obj.forecast[i].Month,
			                   TempMax: obj.forecast[i].TempMax + obj.Units.temperature,
							   TempMin: obj.forecast[i].TempMin + obj.Units.temperature,
							   MaxWind: Convertmstokmh(parseFloat(obj.forecast[i].MaxWind,10)) + " km/h", 
							   AveWind: Convertmstokmh(parseFloat(obj.forecast[i].AveWind,10)) + " km/h",
							   AveHumidity: obj.forecast[i].AveHumidity + obj.Units.humidity,
							   RainDay: obj.forecast[i].RainDay + obj.Units.rain,
							   WeatherIcon: "widgets/forecast/images/Icons1/" + obj.forecast[i].WeatherCondition + ".png"
							 });
         }
         );
      }
   };

   this.configurationChanged = function() {
      var self = this;
	  
	 if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
		return;	  
	 
	 try{
	    self.MaxWindVisible ( self.widget.configuration.Information.content.MaxWind );
	    self.AveWindVisible ( self.widget.configuration.Information.content.AveWind );
	    self.AveHumidityVisible ( self.widget.configuration.Information.content.AveHumidity );
   	    self.RainDayVisible ( self.widget.configuration.Information.content.RainDay );
	 }
	 catch(err) {
	    console.debug( err.message );
	 }
   }
 
   this.resized = function() {
   };
 
   this.getDevicesToListen = function() {
      var result = [];

      //Add the keyword Forecast
      result.push(this.widget.configuration.device);

      return result;
   }
};