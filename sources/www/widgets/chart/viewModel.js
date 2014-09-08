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
               text: 'Data Acquisition'
            },

            subtitle: {
               text: 'Subtitle'
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
         this.chart.showLoading("Loading data ...");
      };

      this.resized = function() {
         if (!isNullOrUndefined(this.chart))
            $(window).trigger("resize");
      };

      this.configurationChanged = function() {
         //we update the kind observable property

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

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

            console.info(Date());

            $.getJSON("rest/acquisition/highcharts/keyword/" + this.widget.configuration.device1.content.source.keywordId + "/" + dateFrom)
               .done(function( data ) {
                  //we parse the json answer
                  if (data.result != "true")
                  {
                     notifyError($.t("chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                     return;
                  }

                  //debugger;
                  console.info(Date());

                  var acq = JSON.parse(data.data);

                  self.chart.hideLoading();
                  self.chart.addSeries({name:'Device', data:acq});
                  console.info(Date());
               })
               .fail(function() {notifyError($.t("switch:errorDuringGettingDeviceInformation"));});
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
         if (device == this.widget.configuration.device) {
               //it is the good device
               //self.command(data.value);
         }
      };

      this.getDevicesToListen = function() {
         var result = [];

         if (!isNullOrUndefined(this.widget.configuration.device1))
            result.push(this.widget.configuration.device1.content.source);

         return result;
      };

   };
