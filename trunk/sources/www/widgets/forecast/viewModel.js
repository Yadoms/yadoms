
widgetViewModelCtor =

/**
 * Create a Forecast ViewModel
 * @constructor
 */
function ForecastViewModel() {

   //observable data
   this.data = ko.observable("");
   
   //Default value - This value is overwrite after
   this.period = ko.observableArray();

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
         self.data(data.value);
		 var obj = jQuery.parseJSON( data.value );
		 
		 console.debug( "objet reçu %o",obj );
		 
		 while(self.period.length > 0) 
            self.period.pop();
		 
		 $.each(obj.forecast, function (i, object) 
		 {
		    self.period.push({ WeatherCondition: obj.forecast[i].WeatherCondition, 
			                   TempMax: obj.forecast[i].TempMax + obj.Units.temperature,
							   TempMin: obj.forecast[i].TempMin + obj.Units.temperature,
							   MaxWind: parseFloat(obj.forecast[i].MaxWind,10).toFixed(2) + obj.Units.speed,
							   AveWind: parseFloat(obj.forecast[i].AveWind,10).toFixed(2) + obj.Units.speed,
							   AveHumidity: obj.forecast[i].AveHumidity + obj.Units.humidity,
							   RainDay: obj.forecast[i].RainDay + obj.Units.rain,
							   WeatherIcon: "widgets/forecast/images/Icons1/" + obj.forecast[i].WeatherCondition + ".gif"
							 });
         }
         );
      }
   };

   //this.configurationChanged = function() {
   //   this.device(this.widget.configuration.device);
   //   var self = this;
   //}
   
   this.getDevicesToListen = function() {
      var result = [];

      //Add the keyword Forecast
      result.push(this.widget.configuration.device);

      return result;
   }

};
