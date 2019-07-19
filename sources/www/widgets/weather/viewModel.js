/**
 * Create a Weather ViewModel
 * @constructor
 */
widgetViewModelCtor = function weatherViewModel() {
   // This variable is needed because of the ko variable
   var environment = this;
   //observable data
   this.data = ko.observable("");
   this.temperature = ko.observable("");
   this.unit = ko.observable("");
   this.condition = ko.observable("");

   this.conditionClass = ko.computed(function () {
      switch (environment.condition().toLowerCase()) {
         case "cloudy":
            return "wi wi-day-cloudy";
         case "snow":
            return "wi wi-day-snow";
         case "chancesnow":
            return "wi wi-day-snow";
         case "partlysunny":
            return "wi wi-day-cloudy";
         case "sunny":
            return "wi wi-day-sunny";
         case "chancerain":
            return "wi wi-day-rain";
         case "rain":
            return "wi wi-day-rain";
         case "sleet":
            return "wi wi-day-sleet";
         case "chancestorm":
            return "wi wi-day-thunderstorm";
         case "storm":
            return "wi wi-day-thunderstorm";
         case "fog":
            return "wi wi-day-fog";
         case "night_clear":
            return "wi wi-night-clear";
         case "night_cloudy":
            return "wi wi-night-cloudy";
         case "night_rain":
            return "wi wi-night-rain";
         case "night_snow":
            return "wi wi-night-snow";
         default:
            return "wi wi-na";
      }
   });

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function () {
      var self = this;
      var d = new $.Deferred();

      self.widgetApi.loadCss("libs/weather-icons/css/weather-icons.min.css").done(function () {
            // Configure the toolbar
            self.widgetApi.toolbar({
               activated: true,
               displayTitle: true
            });
            d.resolve();
         })
         .fail(d.reject);
      return d.promise();
   };

   /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function (keywordId, data) {
      var self = this;
      if (keywordId === parseInt(self.widget.configuration.conditionKeyword.keywordId)) {
         self.condition(data.value);
      } else if (keywordId === parseInt(self.widget.configuration.temperatureSection.content.temperatureKeyword.keywordId)) {
         self.temperature(parseFloat(data.value).toFixed(1));
         self.unit($.t(data.unit));
      }
   };

   this.configurationChanged = function () {
      var self = this;
      //we register keyword new acquisition
      self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.conditionKeyword.keywordId));
      self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.temperatureSection.content.temperatureKeyword.keywordId));
      self.widgetApi.registerAdditionalInformation(["unit"]); // return unit with the getLast Value

      //we register keyword for get last value at web client startup
      self.widgetApi.getLastValue(parseInt(self.widget.configuration.conditionKeyword.keywordId));
      self.widgetApi.getLastValue(parseInt(self.widget.configuration.temperatureSection.content.temperatureKeyword.keywordId));
   }
};