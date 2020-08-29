function Convertmstokmh(value) {
   return value * 3.6;
}

function round(value, precision) {
   if (typeof value == "string") {
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
      this.temperatureMinKw = temperatureKws.find(function (kw) { return kw.name.toLowerCase().includes("min"); });
      this.temperatureMaxKw = temperatureKws.find(function (kw) { return kw.name.toLowerCase().includes("max"); });
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
         || (!isNullOrUndefined(this.forecastDatetimeKw) && keywordId == this.forecastDatetimeKw.id)
         || (!isNullOrUndefined(this.temperatureKw) && keywordId == this.temperatureKw.id)
         || (!isNullOrUndefined(this.temperatureMinKw) && keywordId == this.temperatureMinKw.id)
         || (!isNullOrUndefined(this.temperatureMaxKw) && keywordId == this.temperatureMaxKw.id)
         || (!isNullOrUndefined(this.humidityKw) && keywordId == this.humidityKw.id)
         || (!isNullOrUndefined(this.pressureKw) && keywordId == this.pressureKw.id)
         || (!isNullOrUndefined(this.rainKw) && keywordId == this.rainKw.id)
         || (!isNullOrUndefined(this.snowKw) && keywordId == this.snowKw.id)
         || (!isNullOrUndefined(this.uvKw) && keywordId == this.uvKw.id)
         || (!isNullOrUndefined(this.visibilityKw) && keywordId == this.visibilityKw.id)
         || (!isNullOrUndefined(this.windDirectionKw) && keywordId == this.windDirectionKw.id)
         || (!isNullOrUndefined(this.windspeedKw) && keywordId == this.windspeedKw.id))
         return this;
   }

   this.values = new Map();
   this.setValue = function (keywordId, data) {
      this.values.set(parseInt(keywordId), data);
   }

   this.getForecastDateTime = function (format) {
      if (isNullOrUndefined(this.forecastDatetimeKw))
         return $.t("widgets.weather-multi:LiveConditions");
      const date = moment(new Date(this.values.get(this.forecastDatetimeKw.id)));
      switch (format) {
         case "shortDate":
            return date.format('dddd D');
         case "longDate":
            return date.format('LL');
         case "hour":
            return date.format('LT');
         default:
            console.warn("Invalid format for date " + format);
            return "-";
      }
   }

   this.getCondition = function () { return this.values.get(this.conditionKw.id); }
   this.getWeatherIconPath = function (iconset) {
      var path = "widgets/weather-multi/images/conditions/" + iconset + "/";
      switch (iconset) {
         case "material":
            switch (this.getCondition()) {
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

   this.getTemperature = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.temperatureKw.id)) ? "" : (round(this.values.get(this.temperatureKw.id), 1) + this.temperatureUnity);
   }

   this.hasTemperatureMinMax = function () {
      return !isNullOrUndefinedOrEmpty(this.values.get(this.temperatureMinKw.id)) && !isNullOrUndefined(this.values.get(this.temperatureMaxKw.id));
   }
   this.getTemperatureMin = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.temperatureMinKw.id)) ? "" : (round(this.values.get(this.temperatureMinKw.id), 1) + this.temperatureUnity);
   }
   this.getTemperatureMax = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.temperatureMaxKw.id)) ? "" : (round(this.values.get(this.temperatureMaxKw.id), 1) + this.temperatureUnity);
   }

   this.hasTemperatureAvg = function () {
      return !isNullOrUndefinedOrEmpty(this.values.get(this.temperatureKw.id));
   }
   this.getTemperatureAvg = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.temperatureKw.id)) ? "" : (round(this.values.get(this.temperatureKw.id), 1) + this.temperatureUnity);
   }

   this.hasRainOrSnow = function () {
      return !isNullOrUndefined(this.isSnow());
   }
   this.getRainMm = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.rainKw.id)) ? "" : (round(this.values.get(this.rainKw.id), 1) +  " mm");
   }
   this.getSnowCm = function () {
      // Note : 1 mm of precipitation gives approximatively 1 cm of snow
      return isNullOrUndefinedOrEmpty(this.values.get(this.snowKw.id)) ? "" : (round(this.values.get(this.snowKw.id), 0) + " cm");
   }
   this.isSnow = function () {
      if (isNullOrUndefinedOrEmpty(this.values.get(this.rainKw.id)) && isNullOrUndefinedOrEmpty(this.values.get(this.snowKw.id))) {
         return undefined;
      }
      if (isNullOrUndefinedOrEmpty(this.values.get(this.rainKw.id))) {
         return true;
      }
      if (isNullOrUndefinedOrEmpty(this.values.get(this.snowKw.id))) {
         return false;
      }
      return this.values.get(this.snowKw.id) > this.values.get(this.rainKw.id);
   }

   this.hasWind = function () {
      return !isNullOrUndefinedOrEmpty(this.values.get(this.windspeedKw.id));
   }
   this.getWindSpeed = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.windspeedKw.id)) ? "" : ((round(Convertmstokmh(parseFloat(this.values.get(this.windspeedKw.id), 10)), 0)) + " km/h");
   }
   this.hasWindDirection = function () {
      return !isNullOrUndefinedOrEmpty(this.values.get(this.windDirectionKw.id));
   }
   this.getWindDirection = function () {
      return isNullOrUndefinedOrEmpty(this.values.get(this.windDirectionKw.id)) ? "" : this.values.get(this.windDirectionKw.id);
   }
}

widgetViewModelCtor =

   function forecastViewModel() {

      //Default value - This value is overwrite after
      this.period = ko.observableArray();

      //Definition of the temporary array
      this.tempPeriod = new Array();

      this.devices = [];

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

      this.onNewAcquisition = function (keywordId, data) {

         const controlIndex = this.findControlIndexFromKeywordId(keywordId);

         if (isNullOrUndefined(controlIndex))
            return;

         this.findDeviceFromKeywordId(keywordId).setValue(keywordId, data);

         refresh();
      }

      this.refresh = function () {
         self = this;

         self.tempPeriod = new Array();

         $.each(self.devices, function (index, device) {
            self.tempPeriod.push({
               TimeDate: device.getForecastDateTime(self.widget.configuration.DateFormat),

               WeatherIcon: device.getWeatherIconPath(self.widget.configuration.Iconset),

               DisplayTemperatureMinMax: device.hasTemperatureMinMax() ? "display: block" : "display: none",
               TempMax: device.getTemperatureMax(),
               TempMin: device.getTemperatureMin(),

               DisplayTemperatureAvg: (!device.hasTemperatureMinMax() && device.hasTemperatureAvg()) ? "display: block" : "display: none",
               TempAvg: device.getTemperatureAvg(),

               DisplayRainOrSnow: device.hasRainOrSnow() ? "display: block" : "display: none",
               Rain: device.isSnow() ? device.getSnowCm() : device.getRainMm(),
               RainOrSnowImage: "widgets/weather-multi/images/" + (device.isSnow() ? "snow.png" : "rain.png"),

               DisplayWind: device.hasWind() ? "display: block" : "display: none",
               Wind: device.getWindSpeed(),
               DisplayWindDirection: device.hasWindDirection() ? "display: inline" : "display: none",
               RotateWind: "transform:rotate(" + device.getWindDirection() + "deg);"
            });

         });

         //Resize the widget and display the elements automatically
         self.resized();
      }

      this.configurationChanged = function () {
         var self = this;
         
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
               console.error("Fail to configure widget : " + error);
               self.widgetApi.setState(widgetStateEnum.InvalidConfiguration);
            });
      }

      this.resized = function () {
         this.period.removeAll();
         this.period(this.tempPeriod.slice());
      };
   };