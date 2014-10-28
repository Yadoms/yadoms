widgetViewModelCtor =

   /**
    * Create a Chart ViewModel
    * @constructor
    */
      function ChartViewModel() {

      //widget identifier
      this.widget = null;

      this.refreshingData = false;

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
            chart: {
               marginTop: 10
            },
            navigator : {
               adaptToUpdatedData: false,
               enabled : false
            },

            rangeSelector : {
               enabled : false
            },

            title: {
               text: ''
            },

            scrollbar : {
               enabled : false
            },

            subtitle: {
               text: ''
            },

            xAxis : {
               ordinal: false, //axis is linear
               events : {
               },
               labels : {
                  formatter : function () {
                     if (this.chart.interval) {
                        switch (this.chart.interval) {
                           default:
                           case "HOUR" :
                           case "DAY" :
                              return DateTimeFormatter.dateToString(this.value, "LT");
                              break;
                           case "WEEK" :
                           case "MONTH" :
                           case "HALF_YEAR" :
                           case "YEAR" :
                              return DateTimeFormatter.dateToString(this.value, "L");
                              break;
                        }
                     }
                     return DateTimeFormatter.dateToString(this.value);
                  }
               }
            },

            yAxis : {
               //minRange: 1
            },

            plotOptions : {
              series : {
                 connectNulls: false
              }
            },

            tooltip : {
               useHTML: true,
               enabled : true,
               formatter : function () {
                  var s = "<b>" + DateTimeFormatter.dateToString(this.x, "llll") + "</b>";
                  $.each(this.points, function () {
                     if (!this.series.hideInLegend) {
                        s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                              this.series.name + " : " + this.y + " " + this.series.units;
                     }
                  });
                  return s;
               }
            },

            series : []
         });

         this.chart = this.$chart.highcharts();
      };

      this.resized = function() {

         if (!isNullOrUndefined(this.chart)) {
            this.chart.setSize(this.widget.width(), this.widget.height(), false);
            $(window).trigger("resize");
         }
      };

      this.configurationChanged = function() {

         var self = this;

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         //we update chart title
         try {
            this.chart.setTitle({text: this.widget.configuration.chartTitle});
         }
         catch(err) {}

         var curve2Active = false;

         try {
            curve2Active = parseBool(this.widget.configuration.device2.checkbox);
         }
         catch(err) {}

         //we create the array of colors
         /*var colorArray = [];

         try {
            colorArray.push(this.widget.configuration.device1.content.color)
         }
         catch(err) {}

         if (curve2Active) {
            try {
               colorArray.push(this.widget.configuration.device2.content.color)
            }
            catch(err) {}
         }

         this.chart.options.colors = colorArray;
*/
         var btns = self.widget.$gridsterWidget.find(".nav-btn");

         $.each(btns, function (index, btn) {
            $(btn).unbind("click").bind("click", self.navigatorBtnClick($(btn).attr("level")));

            //we ensure that the configured interval is selected
            if ($(btn).attr("level") == self.widget.configuration.interval) {
               $(btn).addClass("btn-primary");
            }
         });

         //we ask for device information
         this.refreshData(this.widget.configuration.interval, moment());
      };

      this.navigatorBtnClick = function(interval) {
         var self = this;
         return function (e) {
            //we manage activation
            self.widget.$gridsterWidget.find(".nav-btn[level!='" + interval + "']").addClass("btn-default").removeClass("btn-primary");
            self.widget.$gridsterWidget.find(".nav-btn[level='" + interval + "']").addClass("btn-primary").removeClass("btn-default");

            self.refreshData(interval);
         };
      };

      this.refreshData = function(interval) {
         var self = this;

         console.log("step 1 " + moment().format("HH:mm:ss'SSS"));

         try {
            if (!self.refreshingData) {
               this.chart.showLoading($.t("chart:loadingData"));
               self.refreshingData = true;
               //we compute the date from the configuration
               var dateFrom = "";
               var dateTo = DateTimeFormatter.dateToIsoDate(moment());
               var prefixUri = "";
               var timeBetweenTwoConsecutiveValues;
               var isSummaryData;

               switch (interval) {
                  case "HOUR" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'hours'));
                     //we request all data
                     timeBetweenTwoConsecutiveValues = undefined;
                     isSummaryData = false;
                     break;
                  default:
                  case "DAY" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'days'));
                     //we request hour summary data
                     prefixUri = "/hour";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600;
                     isSummaryData = true;
                     break;
                  case "WEEK" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'weeks'));
                     //we request hour summary data
                     prefixUri = "/hour";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600;
                     isSummaryData = true;
                     break;
                  case "MONTH" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'months'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
                  case "HALF_YEAR" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(6, 'months'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
                  case "YEAR" :
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'years'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
               }
               console.log("step 2 " + moment().format("HH:mm:ss'SSS"));
               $.getJSON("rest/acquisition/keyword/" + this.widget.configuration.device1.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo)
                  .done(function( data ) {
                     //we parse the json answer
                     if (data.result != "true")
                     {
                        notifyError($.t("chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                        return;
                     }
                     console.log("step 3 " + moment().format("HH:mm:ss'SSS"));
                     self.chart.hideLoading();

                     //we save interval in the chart
                     self.chart.interval = interval;

                     //we remove last series
                     if (self.serie1UUID) {
                        var serie = self.chart.get(self.serie1UUID);
                        if (!isNullOrUndefined(serie))
                           serie.remove();
                     }

                     if (self.rangeSerie1UUID) {
                        var serie = self.chart.get(self.rangeSerie1UUID);
                        if (!isNullOrUndefined(serie))
                           serie.remove();
                     }

                     //we make the serie
                     var plot = [];

                     var lastDate;
                     var d;

                     if (!isSummaryData) {
                        console.log("step 4 " + moment().format("HH:mm:ss'SSS"));
                        //data comes from acquisition table
                        $.each(data.data.data, function(index, value) {
                           lastDate = d;
                           d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
                           var v;
                           if (!isNullOrUndefined(value.key)) {
                              v = parseFloat(value.key);
                           }
                           else {
                              throw Error("Unable to parse answer");
                           }

                           //we manage the missing data
                           if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                              (lastDate + timeBetweenTwoConsecutiveValues < d)) {
                              plot.push([lastDate + 1, null]);
                           }

                           plot.push([d, v]);
                        });
                     }
                     else {
                        //it is summarized data so we can get min and max curve
                        var range = [];
                        var vMin;
                        var vMax;

                        $.each(data.data.data, function(index, value) {
                           lastDate = d;
                           d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
                           var v;
                           if (!isNullOrUndefined(value.avg)) {
                              v = parseFloat(value.avg);
                              vMin = parseFloat(value.min);
                              vMax = parseFloat(value.max);
                           }
                           else {
                              throw Error("Unable to parse answer");
                           }

                           //we manage the missing data
                           if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                              (lastDate + timeBetweenTwoConsecutiveValues < d)) {
                              plot.push([lastDate + 1, null]);
                              //range.push([lastDate + 1, null, null]);
                           }

                           plot.push([d, v]);
                           range.push([d, vMin, vMax]);
                        });
                     }
                     console.log("step 5 " + moment().format("HH:mm:ss'SSS"));
                     var color;
                     try {
                        color = self.widget.configuration.device1.content.color;
                     }
                     catch(err) {}

                     self.serie1UUID = createUUID();

                     //marker of points is enable when there is less than 50 points on the line
                     self.chart.addSeries({id:self.serie1UUID,
                                           data:plot, name:"First Device", marker : { enabled : (plot.length < 50), radius : 3, symbol: "circle"}, color: color});
                     console.log("step 6 " + moment().format("HH:mm:ss'SSS"));
                     //we add min and max series
                     if (isSummaryData) {
                        //TODO gerer la conf
                        if (true) {

                           self.rangeSerie1UUID = createUUID();
                           self.chart.addSeries({id:self.rangeSerie1UUID, type: 'arearange', lineWidth: 0, fillOpacity: 0.3, zIndex: 0,
                                          data:range, color : color});

                           //we add attribute to hide it in legend
                           var serie = self.chart.get(self.rangeSerie1UUID);
                           serie.hideInLegend = true;
                           console.log("step 7 " + moment().format("HH:mm:ss'SSS"));
                        }
                     }

                     try {
                        if (parseBool(self.widget.configuration.customYAxisMinMax.checkbox)) {
                           //we manage min and max scale y axis
                           var min = parseFloat(self.widget.configuration.customYAxisMinMax.content.minimumValue);
                           var max = parseFloat(self.widget.configuration.customYAxisMinMax.content.maximumValue);
                           self.chart.yAxis[0].setExtremes(min, max);
                        }
                        else {
                           //we cancel previous extremes
                           self.chart.yAxis[0].setExtremes(null, null);
                        }
                     }
                     catch (err) {
                     }

                     console.log("step 8 " + moment().format("HH:mm:ss'SSS"));
                     //self.chart.zoomOut();

                     self.refreshingData = false;

                     //we get the unit of the keyword
                     KeywordManager.get(self.widget.configuration.device1.content.source.keywordId, function(keyword) {
                        var serie = self.chart.get(self.serie1UUID);
                        //we save the unit in the serie
                        if (serie) {
                           serie.units = keyword.units;
                           serie.name = keyword.friendlyName;
                        }
                     });

                     console.log("step 9 " + moment().format("HH:mm:ss'SSS"));
                  })
                  .fail(function() {notifyError($.t("chart:errorDuringGettingDeviceData"));});
            }
         }
         catch (err) {
            console.error(err.message);
            self.refreshingData = false;
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
         try {
            if (device == this.widget.configuration.device1.content.source) {
               //it is the good device
               self.chart.get(self.serie1UUID).addPoint([data.date.valueOf(), parseFloat(data.value)]);
            }
         }
         catch (err) {
         }
      };

      this.getDevicesToListen = function() {
         var result = [];

         try {
            result.push(this.widget.configuration.device1.content.source);
         }
         catch (err) {
         }

         return result;
      };

   };
