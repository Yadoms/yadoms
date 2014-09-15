widgetViewModelCtor =

   /**
    * Create a Chart ViewModel
    * @constructor
    */
      function ChartViewModel() {

      //widget identifier
      this.widget = null;

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function(widget) {
         this.widget = widget;

         var self = this;
         // create the chart
         this.$chart = self.widget.$gridsterWidget.find("div.chartWidgetContainer");
         this.$chart.highcharts('StockChart', {

            navigator : {
               adaptToUpdatedData: false
            },

            title: {
               text: ''
            },

            subtitle: {
               text: ''
            },

            rangeSelector : {
               buttons: [{
                  type: 'hour',
                  count: 1,
                  text: '1h'
               }, {
                  type: 'day',
                  count: 1,
                  text: '1d'
               }, {
                  type: 'month',
                  count: 1,
                  text: '1m'
               }, {
                  type: 'year',
                  count: 1,
                  text: '1y'
               }, {
                  type: 'all',
                  text: 'All'
               }],
               inputEnabled: false, // it supports only days
               selected : 4 // all
            },

            navigator : {enabled:false},

            xAxis : {
               ordinal: false,
               events : {
                  //afterSetExtremes : afterSetExtremes
               },
               minRange: 3600 * 1000 // one hour
            },

            series : []
         });

         this.chart = this.$chart.highcharts();

         this.chart.showLoading($.t("chart:loadingData"));
      };

      this.resized = function() {
         if (!isNullOrUndefined(this.chart))
            $(window).trigger("resize");
      };

      this.configurationChanged = function() {
         //we update the kind observable property

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         //we update chart title
         debugger;
         this.chart.setTitle({text: this.widget.configuration.chartTitle});

         var curve2Active = parseBool(this.widget.configuration.device2.checkbox, false);

         //we create the array of colors
         var colorArray = [];
         if ((!isNullOrUndefined(this.widget.configuration.device1)) && (!isNullOrUndefined(this.widget.configuration.device1.content)) && (!isNullOrUndefined(this.widget.configuration.device1.content.color))) {
            colorArray.push(this.widget.configuration.device1.content.color)
         }

         if ((!isNullOrUndefined(this.widget.configuration.device2)) && (curve2Active) && (!isNullOrUndefined(this.widget.configuration.device2.content)) && (!isNullOrUndefined(this.widget.configuration.device2.content.color))) {
            colorArray.push(this.widget.configuration.device2.content.color)
         }

         this.chart.options.colors = colorArray;

         //todo manage min and max

         //we ask for device information
         if (!isNullOrUndefined(this.widget.configuration.device1)) {
            var self = this;
            //we compute the date from the configuration
            var dateFrom = "";

            switch (this.widget.configuration.interval) {
               case "HOUR" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'hours'));
                  break;
               default:
               case "DAY" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'days'));
                  break;
               case "WEEK" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'weeks'));
                  break;
               case "MONTH" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'months'));
                  break;
               case "HALF_YEAR" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(6, 'months'));
                  break;
               case "YEAR" :
                  dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'years'));
                  break;
            }

            if ((!isNullOrUndefined(self.widget.configuration.device1)) && (!isNullOrUndefined(self.widget.configuration.device1.content))) {

               $.getJSON("rest/acquisition/highcharts/keyword/" + this.widget.configuration.device1.content.source.keywordId + "/" + dateFrom)
                  .done(function( data ) {
                     //we parse the json answer
                     if (data.result != "true")
                     {
                        notifyError($.t("chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                        return;
                     }

                     var acq = JSON.parse(data.data);

                     self.chart.hideLoading();
                     var serie = self.chart.get("Device1");
                     if (!isNullOrUndefined(serie))
                        serie.remove();

                     self.chart.addSeries({id:'Device1', data:acq, name:'Fisrt device'});

                     //we ask for device information
                     DeviceManager.get(self.widget.configuration.device1.content.source.deviceId, function (device) {
                        var serie = self.chart.get("Device1");
                        if (!isNullOrUndefined(serie))
                           self.chart.get("Device1").name = device.friendlyName;
                     });
                  })
                  .fail(function() {notifyError($.t("chart:errorDuringGettingDeviceData"));});
            }
         }
      };

      /**
       * Dispatch the data to the viewModel
       * @deviceId device identifier which make the values
       * @param data data to dispatch
       * @param device
       */
      this.dispatch = function(device, data) {
         var self = this;
         if (!isNullOrUndefined(this.widget.configuration.device1)) {
            if (device == this.widget.configuration.device1.content.source) {
                  //it is the good device
               var serie = self.chart.get("Device1");
               if (!isNullOrUndefined(serie))
                  this.chart.get("Device1").addPoint([data.date.valueOf(), parseFloat(data.value)]);
            }
         }
      };

      this.getDevicesToListen = function() {
         var result = [];

         if (!isNullOrUndefined(this.widget.configuration.device1))
            result.push(this.widget.configuration.device1.content.source);

         return result;
      };

   };
