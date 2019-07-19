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
         case "sunny":
            return "wi wi-day-sunny";
         case "cloudy":
            return "wi wi-cloudy";
         case "cloudygusts":
            return "wi wi-cloudy-gusts";
         case "cloudywindy":
            return "wi wi-cloudy-windy";
         case "cloudyhigh":
            return "wi wi-day-cloudy-high";
         case "partlycloudy":
            return "wi wi-day-cloudy";
         case "fog":
            return "wi wi-fog";
         case "hail":
            return "wi wi-hail";
         case "haze":
            return "wi wi-day-haze";
         case "drizzle":
            return "wi wi-day-fog";
         case "lightning":
            return "wi wi-lightning";
         case "rain":
            return "wi wi-rain";
         case "rainmix":
            return "wi wi-rain-mix";
         case "rainwind":
            return "wi wi-rain-wind";
         case "showers":
            return "wi wi-showers";
         case "sleet":
            return "wi wi-sleet";
         case "sleetstorm":
            return "wi wi-sleet";
         case "snow":
            return "wi wi-snow";
         case "snowthunderstorm":
            return "wi wi-snow";
         case "snowwind":
            return "wi wi-snow-wind";
         case "sprinkle":
            return "wi wi-sprinkle";
         case "stormshowers":
            return "wi wi-storm-showers";
         case "thunderstorm":
            return "wi wi-thunderstorm";
         case "windy":
            return "wi wi-windy";
         case "lightwind":
            return "wi wi-windy";
         case "nightclear":
            return "wi wi-night-clear";
         case "nightcloudy":
            return "wi wi-night-cloudy";
         case "nightcloudygusts":
            return "wi wi-night-cloudy-gusts";
         case "nightcloudywindy":
            return "wi wi-night-cloudy-windy";
         case "nightcloudyhigh":
            return "wi wi-night-cloudy-high";
         case "nightpartlycloudy":
            return "wi wi-night-partly-cloudy";
         case "nightfog":
            return "wi wi-night-fog";
         case "nighthail":
            return "wi wi-night-hail";
         case "nighthaze":
            return "wi wi-night-fog";
         case "nightlightning":
            return "wi wi-night-";
         case "nightrain":
            return "wi wi-night-lightning";
         case "nightrainmix":
            return "wi wi-night-rain-mix";
         case "nightrainwind":
            return "wi wi-night-rain-wind";
         case "nightshowers":
            return "wi wi-night-showers";
         case "nightsleet":
            return "wi wi-night-sleet";
         case "nightsleetstorm":
            return "wi wi-night-sleet-storm";
         case "nightsnow":
            return "wi wi-night-snow";
         case "nightsnowthunderstorm":
            return "wi wi-night-snow-thunderstorm";
         case "nightsnowwind":
            return "wi wi-night-snow-wind";
         case "nightsprinkle":
            return "wi wi-night-sprinkle";
         case "nightstormshowers":
            return "wi wi-night-storm-showers";
         case "nightthunderstorm":
            return "wi wi-night-thunderstorm";
         case "nightwindy":
            return "wi wi-night-windy";
         case "nightlightwind":
            return "wi wi-night-windy";
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