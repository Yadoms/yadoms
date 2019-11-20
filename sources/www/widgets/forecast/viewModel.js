function Convertmstokmh(value) {
   return value * 3.6;
}

function round(value, precision) {
   if (typeof value == "string"){
      value = parseFloat(value);
   }
   var multiplier = Math.pow(10, precision || 0);
   return Math.round(value * multiplier) / multiplier;
}

function ConditionsDevice(keywords) {
   assert(!isNullOrUndefined(keywords), "keywords array must be defined");
   assert(!isNullOrUndefined(keywords.length) && keywords.length > 0, "keywords array is empty");

   this.deviceId = keywords[0].deviceId;

   this.conditionKw = keywords.find(function (kw) { return kw.capacityName == "weathercondition"; });
   assert(!isNullOrUndefined(this.conditionKw), "weathercondition must be provided");

   this.forecastDatetimeKw = keywords.find(function (kw) { return kw.capacityName == "datetime"; });

   var temperatureKws = keywords.filter(kw => kw.capacityName == "temperature");
   if (temperatureKws.length == 1) {
      this.temperatureKw = temperatureKws[0];
   }
   else if (temperatureKws.length > 1) {
      this.temperatureKw = temperatureKws.find(function (kw) { return kw.name.toLowerCase() == "temperature"; });
      this.temperatureMinKw = temperatureKws.find(function (kw) { return kw.name.toLowerCase() == "temperature min"; });
      this.temperatureMaxKw = temperatureKws.find(function (kw) { return kw.name.toLowerCase() == "temperature max"; });
   }
   this.temperatureUnity = $.t(keywords.find(function (kw) { return kw.capacityName == "temperature"; }).units);

   this.humidityKw = keywords.find(function (kw) { return kw.capacityName == "humidity"; });

   this.pressureKw = keywords.find(function (kw) { return kw.capacityName == "pressure"; });

   var rainKws = keywords.filter(kw => kw.capacityName == "rain");
   if (rainKws.length == 1) {
      this.rainKw = rainKws[0];
   }
   else if (rainKws.length > 1) {
      this.rainKw = rainKws.find(function (kw) { return kw.name.toLowerCase().includes("rain"); });
      this.snowKw = rainKws.find(function (kw) { return kw.name.toLowerCase().includes("snow"); });
   }

   this.uvKw = keywords.find(function (kw) { return kw.capacityName == "uv"; });

   this.visibilityKw = keywords.find(function (kw) { return kw.capacityName == "distance"; });

   this.windDirectionKw = keywords.find(function (kw) { return kw.capacityName == "direction"; });

   this.windspeedKw = keywords.find(function (kw) { return kw.capacityName == "speed"; });

   this.hasKeywordId = function (keywordId) {
      if (keywordId == this.conditionKw.id
         || keywordId == this.forecastDatetimeKw.id
         || keywordId == this.temperatureKw.id
         || keywordId == this.temperatureMinKw.id
         || keywordId == this.temperatureMaxKw.id
         || keywordId == this.humidityKw.id
         || keywordId == this.pressureKw.id
         || keywordId == this.rainKw.id
         || keywordId == this.snowKw.id
         || keywordId == this.uvKw.id
         || keywordId == this.visibilityKw.id
         || keywordId == this.windDirectionKw.id
         || keywordId == this.windspeedKw.id)
         return this;
   }

   this.values = new Map();
   this.setValue = function (keywordId, data) {
      this.values.set(parseInt(keywordId), data);
   }
   this.getCondition = function () { return this.values.get(this.conditionKw.id); }
   this.getForecastDateTime = function (format) {
      return moment(new Date(this.values.get(this.forecastDatetimeKw.id))).format(format === "short" ? 'dddd D' : 'LL');
   }
   this.getTemperature = function () {
      return isNullOrUndefined(this.values.get(this.temperatureKw.id)) ? "" : (round(this.values.get(this.temperatureKw.id), 1) + this.temperatureUnity);
   }
   this.getTemperatureMin = function () {
      return isNullOrUndefined(this.values.get(this.temperatureMinKw.id)) ? "" : (round(this.values.get(this.temperatureMinKw.id), 1) + this.temperatureUnity);
   }
   this.getTemperatureMax = function () {
      return isNullOrUndefined(this.values.get(this.temperatureMaxKw.id)) ? "" : (round(this.values.get(this.temperatureMaxKw.id), 1) + this.temperatureUnity);
   }
   this.getWindSpeed = function () {
      return isNullOrUndefined(this.values.get(this.windspeedKw.id)) ? "" : (round(Convertmstokmh(parseFloat(this.values.get(this.windspeedKw.id), 10)), 0));
   }
   this.getWindDirection = function () {
      return isNullOrUndefined(this.values.get(this.windDirectionKw.id)) ? "" : this.values.get(this.windDirectionKw.id);
   }
   this.getRain = function () {
      return isNullOrUndefined(this.values.get(this.rainKw.id)) ? "" : round(this.values.get(this.rainKw.id), 1);
   }
   this.getWeatherIconPath = function (iconset) {
      var path = "widgets/forecast/images/conditions/" + iconset + "/";
      switch (iconset) {
         case "material":
            switch (this.values.get(this.conditionKw.id)) {
               case "Sunny": return path + "Sunny.png";
               case "Cloudy": return path + "Cloudy.png";
               case "CloudyGusts": return path + "Cloudy.png";
               case "CloudyWindy": return path + "Cloudy.png";
               case "CloudyHigh": return path + "Cloudy.png";
               case "PartlyCloudy": return path + "PartlySunny.png";
               case "Fog": return path + "Fog.png";
               case "Hail": return path + "Sleet.png";
               case "Haze": return path + "Fog.png";
               case "Drizzle": return path + "Rain.png";
               case "Lightning": return path + "Storm.png";
               case "Rain": return path + "Rain.png";
               case "RainMix": return path + "Rain.png";
               case "RainWind": return path + "Rain.png";
               case "Showers": return path + "Rain.png";
               case "Sleet": return path + "Sleet.png";
               case "SleetStorm": return path + "Sleet.png";
               case "Snow": return path + "Snow.png";
               case "SnowThunderstorm": return path + "Snow.png";
               case "SnowWind": return path + "Snow.png";
               case "Sprinkle": return path + "Snow.png";
               case "StormShowers": return path + "Rain.png";
               case "Thunderstorm": return path + "Storm.png";
               case "Windy": return path + "Sunny.png";
               case "LightWind": return path + "Sunny.png";
               case "NightClear": return path + "Night_Clear.png";
               case "NightCloudy": return path + "Night_Cloudy.png";
               case "NightCloudyGusts": return path + "Night_Cloudy.png";
               case "NightCloudyWindy": return path + "Night_Cloudy.png";
               case "NightCloudyHigh": return path + "Night_Cloudy.png";
               case "NightPartlyCloudy": return path + "Night_Cloudy.png";
               case "NightFog": return path + "Night_Cloudy.png";
               case "NightHail": return path + "Night_Rain.png";
               case "NightHaze": return path + "Night_Cloudy.png";
               case "NightLightning": return path + "Storm.png";
               case "NightRain": return path + "Night_Rain.png";
               case "NightRainMix": return path + "Night_Rain.png";
               case "NightRainWind": return path + "Night_Rain.png";
               case "NightShowers": return path + "Night_Rain.png";
               case "NightSleet": return path + "Night_Rain.png";
               case "NightSleetStorm": return path + "Night_Rain.png";
               case "NightSnow": return path + "Night_Snow.png";
               case "NightSnowThunderstorm": return path + "Night_Snow.png";
               case "NightSnowWind": return path + "Night_Snow.png";
               case "NightSprinkle": return path + "Night_Snow.png";
               case "NightStormShowers": return path + "Night_Rain.png";
               case "NightThunderstorm": return path + "Night_Rain.png";
               case "NightWindy": return path + "Night_Clear.png";
               case "NightLightWind": return path + "Night_Clear.png";
            }
            break;
         case "color":
            switch (this.values.get(this.conditionKw.id)) {
               case "Sunny": return path + "sunny.png";
               case "Cloudy": return path + "cloudy.png";
               case "CloudyGusts": return path + "wind.png";
               case "CloudyWindy": return path + "wind.png";
               case "CloudyHigh": return path + "cloudy.png";
               case "PartlyCloudy": return path + "partly-cloudy.png";
               case "Fog": return path + "fog.png";
               case "Hail": return path + "freezing-rain.png";
               case "Haze": return path + "hazy.png";
               case "Drizzle": return path + "drizzle.png";
               case "Lightning": return path + "thunder-storm.png";
               case "Rain": return path + "rainy.png";
               case "RainMix": return path + "rainy-snow.png";
               case "RainWind": return path + "rainy.png";
               case "Showers": return path + "showers.png";
               case "Sleet": return path + "sleet.png";
               case "SleetStorm": return path + "sleet.png";
               case "Snow": return path + "snow.png";
               case "SnowThunderstorm": return path + "blowing-snow.png";
               case "SnowWind": return path + "snow.png";
               case "Sprinkle": return path + "snow.png";
               case "StormShowers": return path + "rainy.png";
               case "Thunderstorm": return path + "thunder-storm.png";
               case "Windy": return path + "wind.png";
               case "LightWind": return path + "wind.png";
               case "NightClear": return path + "moon.png";
               case "NightCloudy": return path + "m-cloudy-night.png";
               case "NightCloudyGusts": return path + "m-cloudy-night.png";
               case "NightCloudyWindy": return path + "m-cloudy-night.png";
               case "NightCloudyHigh": return path + "m-cloudy-night.png";
               case "NightPartlyCloudy": return path + "p-c-night.png";
               case "NightFog": return path + "m-cloudy-night.png";
               case "NightHail": return path + "m-c-night-rain.png";
               case "NightHaze": return path + "m-cloudy-night.png";
               case "NightLightning": return path + "m-c-night-rain.png";
               case "NightRain": return path + "m-c-night-rain.png";
               case "NightRainMix": return path + "m-c-night-rain.png";
               case "NightRainWind": return path + "m-c-night-rain.png";
               case "NightShowers": return path + "m-c-night-rain.png";
               case "NightSleet": return path + "m-c-night-rain.png";
               case "NightSleetStorm": return path + "m-c-night-rain.png";
               case "NightSnow": return path + "m-c-night-snow.png";
               case "NightSnowThunderstorm": return path + "m-c-night-snow.png";
               case "NightSnowWind": return path + "m-c-night-snow.png";
               case "NightSprinkle": return path + "m-c-night-snow.png";
               case "NightStormShowers": return path + "m-c-night-rain.png";
               case "NightThunderstorm": return path + "m-c-night-rain.png";
               case "NightWindy": return path + "moon.png";
               case "NightLightWind": return path + "moon.png";
            }
            break;
      }
      console.warn("Unknwon condition value. iconset = " + iconset + ", conditionValue = ", this.values.get(this.conditionKw.id))
      return "";
   }
}

widgetViewModelCtor =
   /**
    * Create a Forecast ViewModel
    * @constructor
    */
   function forecastViewModel() {

      //observable data
      this.data = ko.observable(""); //TODO conserver ?

      //Default value - This value is overwrite after
      this.period = ko.observableArray(); //TODO conserver ?

      //Definition of the temporary array
      this.TempPeriod = new Array(); //TODO conserver ?

      this.devices = [];

      //Number of day to be displayed
      this.ControlNumber = ko.observable(10); //TODO conserver ?

      //Height of the widget.
      this.height = 0; //TODO conserver ?

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function () {
         var self = this;
         self.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
         });
      };

      this.RainCanvasLoad = function (canvasId, rainValue) {
         //get a reference to the canvas
         var ctx = this.widgetApi.find("#" + canvasId).get(0).getContext("2d");

         // Refresh the canvas, clear all existing information
         ctx.clearRect(0, 0, 40, 45);

         ctx.fillStyle = "rgb(116,208,241)"; // blue Azur clair

         var rain = parseInt(rainValue, 10);
         if (isNaN(rain)) rain = 0;

         var positionY = 0;
         if (rainValue < 40)
            positionY = 40 - rain;

         ctx.fillRect(0, positionY, 40, rain);
         ctx.stroke();

         ctx.strokeStyle = "rgb(116,208,241)"; // blue Azur clair

         ctx.beginPath();
         ctx.moveTo(40, 0);
         ctx.lineTo(40, 40);
         ctx.closePath();

         ctx.fillStyle = "rgb(0,0,255)"; // black

         // We use the same font as in the css file
         ctx.font = $(".cases").css("font-size") + " " + $(".cases").css("font-family");

         //write the text at the same position as the height of the column
         ctx.fillText(rainValue, 20 - (7 * String(rainValue).length) / 2, 23);
         ctx.stroke();
      };

      this.findControlIndexFromKeywordId = function (keywordId) {
         return this.devices.findIndex(function (device) {
            return device.hasKeywordId(keywordId);
         });
      }

      this.findDeviceFromKeywordId = function (keywordId) {
         return this.devices.find(function (device) {
            return device.hasKeywordId(keywordId);
         });
      }

      this.onFirstAcquisition = function (keywordId, data) {
         this.findDeviceFromKeywordId(keywordId).setValue(keywordId, data);
      }

      /**
       * New acquisition handler
       * @param device Device on which new acquisition was received
       * @param data Acquisition data
       */
      this.onNewAcquisition = function (keywordId, data) {

         const controlIndex = this.findControlIndexFromKeywordId(keywordId);

         if (isNullOrUndefined(controlIndex))
            return;

         this.findDeviceFromKeywordId(keywordId).setValue(keywordId, data);

         refresh();
      }

      this.refresh = function () {
         self = this;

         self.TempPeriod = new Array();

         //TODO virer ? self.ControlNumber(obj.forecast.length);

         // Redraw
         $.each(self.devices, function (index, device) {
            // create the name for each div where wind canvas will be attached
            var windElementId = 'widget-wind-' + index;

            // create the name for each div where rain canvas will be attached
            var rainElementId = 'widget-rain-' + index;

            self.TempPeriod.push({
               WeatherCondition: device.getCondition(),
               TimeDate: device.getForecastDateTime(self.widget.configuration.DateFormat),
               TempMax: device.getTemperatureMax(),
               TempMin: device.getTemperatureMin(),
               AveWind: device.getWindSpeed() + " km/h",
               RotateWind: "transform:rotate(" + device.getWindDirection() + "deg);",
               WindCanvasId: windElementId,
               RainCanvasId: rainElementId,
               Rain: device.getRain(),
               WeatherIcon: device.getWeatherIconPath(self.widget.configuration.Iconset)
            }); //TODO exploiter les valeurs non encore exploitées(snow...)

         });

         //Resize the widget and display the elements automatically
         self.resized();
      }

      this.configurationChanged = function () {
         var self = this;
         //TODO le top serait de ne devoir sélectionner que le device (les keywords étant récupérés dynamiquement). Il faut sélectionner tout device contenant au moins le KW conditions (le seul requis)
         if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;

         self.devicesIdPerPeriod = self.widget.configuration.devices.map(x => x.content.source.deviceId);

         var arrayOfDeffered = [];
         var listenKeywordIds = [];
         self.devices = [];
         self.devicesIdPerPeriod.forEach(function (deviceId) {
            var d = $.Deferred();
            DeviceManager.getKeywordsByDeviceId(deviceId)
               .done(function (keywords) {
                  self.devices.push(new ConditionsDevice(keywords));
                  Array.prototype.push.apply(listenKeywordIds, keywords.map(kw => kw.id));
                  d.resolve();
               })
               .fail(d.reject);
            arrayOfDeffered.push(d.promise());
         });

         $.when.apply($, arrayOfDeffered)
            .done(function () {
               self.widgetApi.registerKeywordForNewAcquisitions(listenKeywordIds);

               // Cannot use self.widgetApi.getLastValue here, as keywords list is not known at configurationChanged end.
               // So directy get last keywords values and call onNewAcquisition
               KeywordManager.getLastValues(listenKeywordIds)
                  .done(function (data) {
                     for (let [keywordId, keywordLastValue] of Object.entries(data)) {
                        self.onFirstAcquisition(keywordId, keywordLastValue.lastValue);
                     }
                     self.refresh();
                  });
            })
            .fail(function (error) {
               throw "Fail to configure widget : " + error;
            });
      }

      this.resized = function () {
         var self = this;

         if (self.widget.getWidth() <= 100)
            self.ControlNumber(1); //TODO revoir le nombre
         else if (self.widget.getWidth() <= 200) // if length = 2 cases -> 2 days
            self.ControlNumber(3); //TODO revoir le nombre
         else if (self.widget.getWidth() <= 300) // if length = 3 cases -> 3 days
            self.ControlNumber(4); //TODO revoir le nombre
         else if (self.widget.getWidth() <= 400) // if length = 4 cases -> 5 days
            self.ControlNumber(6); //TODO revoir le nombre
         else if (self.widget.getWidth() <= 500) // if length = 5 cases -> 6 days
            self.ControlNumber(6); //TODO revoir le nombre
         else if (self.widget.getWidth() <= 600) // if length = 6 cases -> 8 days
            self.ControlNumber(8); //TODO revoir le nombre
         else if (self.ControlNumber() !== 10)  // Otherwise 10 days
            self.ControlNumber(10); //TODO revoir le nombre

         self.period.removeAll();
         self.period(self.TempPeriod.slice(0, self.ControlNumber()));
      };
   };