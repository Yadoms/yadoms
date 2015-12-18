widgetViewModelCtor =

/**
 * Create a Chart ViewModel
 * @constructor
 */
   function ChartViewModel() {

       //widget identifier
       this.widget = null;
       this.refreshingData = false;
       this.seriesUuid = [];

       //Keyword Id List !
       this.devicesList = [];
       this.interval = 0;
       this.keywordInfo = [];
       this.ChartPromise = null;

       /**
        * Initialization method
        * @param widget widget class object
        */
       this.initialize = function (widget) {
           this.widget = widget;
           var self = this;

           // create the chart
           this.$chart = self.widget.$gridWidget.find("div.chartWidgetContainer");

           this.chartOption = {
               chart: {
                   type: 'line',
                   marginTop: 10
               },
               legend: {
		   layout: 'horizontal',
		   align: 'center',
		   verticalAlign: 'bottom',
		   borderWidth: 0,
		   enabled : true
			   },  
               navigator: {
                   adaptToUpdatedData: false,
                   enabled: false
               },
               credits: {
                   enabled: false
               },
               rangeSelector: {
                   enabled: false
               },

               title: {
                   text: null
               },

               scrollbar: {
                   enabled: false
               },

               subtitle: {
                   text: ''
               },

               xAxis: {
                   ordinal: false, //axis is linear
                   events: {},
                   labels: {
                       formatter: function () {
                           if (this.chart.interval) {
                               switch (this.chart.interval) {
                                   default:
                                   case "HOUR":
                                   case "DAY":
                                       return DateTimeFormatter.dateToString(this.value, "LT");
                                       break;
                                   case "WEEK":
                                   case "MONTH":
                                   case "HALF_YEAR":
                                   case "YEAR":
                                       return DateTimeFormatter.dateToString(this.value, "L");
                                       break;
                               }
                           }
                           return DateTimeFormatter.dateToString(this.value);
                       }
                   }
               },

               yAxis: { // Default Axis
               },

               plotOptions: {
                   bar: {
                       pointPadding: 0.2
                   },

                   series: {
                       connectNulls: false
                   }
               },

               tooltip: {
                   useHTML: true,
                   enabled: true,
                   formatter: function () {
                       var s = "<b>" + DateTimeFormatter.dateToString(this.x, "llll") + "</b>";
                       $.each(this.points, function () {
                           if (!this.series.hideInLegend) {
                               if (isNullOrUndefined(this.point.low)) { //Standard serie
                                   s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                      this.series.name + " : " + this.y.toFixed(1) + " " + this.series.units;
                               } else { //Range Serie
                                   s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                      this.series.name + " : " + this.point.low.toFixed(1) + "-" + this.point.high.toFixed(1) + " " + this.series.units;
                               }
                           }
                       });
                       return s;
                   },
                   shared: true
               },

               exporting: {
                   enabled: false
               },

               series: []
           };

           this.$chart.highcharts('StockChart', this.chartOption);
           this.chart = this.$chart.highcharts();

           //we manage toolbar buttons
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"HOUR\"><span data-i18n=\"widgets/chart:navigator.hour\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"DAY\"><span data-i18n=\"widgets/chart:navigator.day\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"WEEK\"><span data-i18n=\"widgets/chart:navigator.week\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"MONTH\"><span data-i18n=\"widgets/chart:navigator.month\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"HALF_YEAR\"><span data-i18n=\"widgets/chart:navigator.half_year\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-button range-btn\" interval=\"YEAR\"><span data-i18n=\"widgets/chart:navigator.year\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-separator\"></div>");
           /*widget.$toolbar.append("<div class=\"widget-toolbar-element\"><span class=\"fa fa-signal\"/></div>");
           widget.$toolbar.append("<div class=\"widget-toolbar-separator\"></div>");*/
           widget.$toolbar.append("<div class=\"widget-toolbar-button export-btn dropdown\">" +
                                      "<span class=\"dropdown-toggle\" id=\"chartExportMenu\"  type=\"button\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"true\">" +
                                          "<span class=\"fa fa-bars\"/>" +
                                      "</span>" +
                                      "<ul class=\"dropdown-menu\" aria-labelledby=\"chartExportMenu\">" +
                                          "<li><span class=\"print-command\" data-i18n=\"widgets/chart:export.print\"></span></li>" +
                                          "<li role=\"separator\" class=\"divider\"></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.png\" mime-type=\"image/png\"></span></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.jpeg\" mime-type=\"image/jpeg\"></span></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.pdf\" mime-type=\"application/pdf\"></span></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.svg\" mime-type=\"image/svg+xml\"></span></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.csv\" mime-type=\"text/csv\"></span></li>" +
                                          "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.xls\" mime-type=\"application/vnd.ms-excel\"></span></li>" +
                                      "</ul>" +
                                  "</div>");

           var $btns = self.widget.$gridWidget.find(".range-btn");
           $btns.unbind("click").bind("click", self.navigatorBtnClick());

           self.widget.$gridWidget.find(".print-command").unbind("click").bind("click", function() {
               self.chart.print();
           });

           self.widget.$gridWidget.find(".export-command").unbind("click").bind("click", function (e) {
               self.chart.exportChartLocal({
                   type: $(e.currentTarget).attr("mime-type"),
                   filename: 'export'
               });
           });
       };

       this.resized = function () {
           var self = this;
           if (!isNullOrUndefined(this.chart)) {
               this.chart.setSize(this.widget.width() - 30, this.widget.height() - 40, false);

               $(window).trigger("resize");
           }
       };

       this.configurationChanged = function () {
           var self = this;

           if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
               return;

		   //Desactivate the old button
           self.widget.$gridWidget.find(".range-btn[interval='" + self.interval + "']").removeClass("widget-toolbar-pressed-button");		   
		   
           self.interval = this.widget.configuration.interval;

		   //Activate the new button
           self.widget.$gridWidget.find(".range-btn[interval='" + self.interval + "']").addClass("widget-toolbar-pressed-button");

           //just update some viewmodel info
           self.seriesUuid = [];
           self.devicesList = self.widget.configuration.devices.slice(0);


           self.ChartPromise = new PromiseCounter(self.devicesList.length, this.refreshData.bind(this), this.widget.configuration.interval);

           //we create an uuid for each serie
           $.each(self.widget.configuration.devices, function (index, device) {
               //we update uuid if they don't exist
               if (isNullOrUndefined(self.seriesUuid[index]))
                   self.seriesUuid[index] = createUUID();

               //we add the keywordId to the listenedList
               self.widget.ListenKeyword(device.content.source.keywordId);

               //we ask the current value
               KeywordManager.get(device.content.source.keywordId, function (keyword) {
                   self.keywordInfo[index] = keyword;

                   self.ChartPromise.resolve(index);
               });
           });
       };

       this.cleanUpChart = function (serie, time, cleanValue) {
           var self = this;

           var ex = false;

           while (!ex) {
               if (!isNullOrUndefined(serie.points)) {
                   if (!isNullOrUndefined(serie.points[0])) {
                       if ((time.valueOf() - serie.points[0].x) > cleanValue)
                           serie.removePoint(0, true); // If false, we never delete the point -> infinite loop
                       else
                           ex = true;
                   } else
                       ex = true;
               } else
                   ex = true;
           }
       };

       this.navigatorBtnClick = function () {
           var self = this;
           return function (e) {
               //we manage activation
               var interval = $(e.currentTarget).attr("interval");

               //we manage button inversion
               self.widget.$gridWidget.find(".range-btn[interval='" + interval + "']").addClass("widget-toolbar-pressed-button");
               self.widget.$gridWidget.find(".range-btn[interval!='" + interval + "']").removeClass("widget-toolbar-pressed-button");

               self.refreshData(interval);
           };
       };

       this.isBoolVariable = function (index) {
           var self = this;
           if (self.keywordInfo[index].type == "Bool") {
               return true;
           }
           else {
               return false;
           }
       }

       this.refreshData = function (interval) {
           var self = this;

           self.interval = interval;

           //we save interval in the chart
           self.chart.interval = interval;

           try {
               if (!self.refreshingData) {
                   self.chart.showLoading($.t("widgets/chart:loadingData"));
                   self.refreshingData = true;
                   //we compute the date from the configuration
                   var dateFrom = "";
                   var dateTo = "";
                   var prefixUri = "";
                   var timeBetweenTwoConsecutiveValues;
                   var isSummaryData;

                   switch (interval) {
                       case "HOUR":

                           //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s.
                           //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.

                           dateTo = DateTimeFormatter.dateToIsoDate(moment());
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'hours').startOf('minute'));
                           //we request all data
                           timeBetweenTwoConsecutiveValues = undefined;
                           isSummaryData = false;
                           break;
                       default:
                       case "DAY":
                           dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('hour').subtract(1, 'seconds'));
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'days').startOf('hour'));
                           //we request hour summary data
                           prefixUri = "/hour";
                           timeBetweenTwoConsecutiveValues = 1000 * 3600;
                           isSummaryData = true;
                           break;
                       case "WEEK":
                           dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('hour').subtract(1, 'seconds'));
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'weeks').startOf('hour'));
                           //we request hour summary data
                           prefixUri = "/hour";
                           timeBetweenTwoConsecutiveValues = 1000 * 3600;
                           isSummaryData = true;
                           break;
                       case "MONTH":
                           dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'months').startOf('day'));
                           //we request day summary data
                           prefixUri = "/day";
                           timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                           isSummaryData = true;
                           break;
                       case "HALF_YEAR":
                           dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(6, 'months').startOf('day'));
                           //we request day summary data
                           prefixUri = "/day";
                           timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                           isSummaryData = true;
                           break;
                       case "YEAR":
                           dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                           dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'years').startOf('day'));
                           //we request day summary data
                           prefixUri = "/day";
                           timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                           isSummaryData = true;
                           break;
                   }

                   //ensure all series and axis are removed (may cause some crash if not done)
                   while (self.chart.series.length > 0)
                       self.chart.series[0].remove(false);

                   while (self.chart.yAxis.length > 0)
                       self.chart.yAxis[0].remove(false);
				   
                   var SeriesPromise = new PromiseCounter(self.widget.configuration.devices.length, this.finalRefresh.bind(this), null);

                   //for each plot in the configuration we request for data
                   $.each(self.widget.configuration.devices, function (index, device) {

                       var DisplayData = true;
                       //If the device is a bool, you have to modify
                       if (self.isBoolVariable(index)) {
                           switch (interval) {
                               case "DAY":
                               case "WEEK":
                               case "MONTH":
                                   dateTo = DateTimeFormatter.dateToIsoDate(moment());
                                   DisplayData = true;
                                   break;
                               case "HALF_YEAR":
                               case "YEAR":
                                   // Display that the range is too large
                                   self.chart.showLoading($.t("widgets/chart:RangeTooBroad"));
                                   DisplayData = false;
                                   self.refreshingData = false;
                                   break;
                           }
                           //we request hour summary data
                           prefixUri = "";
                           isSummaryData = false;
                       }
                       else {
                           DisplayData = true;
                       }

                       if (DisplayData) {

                           $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo)
                              .done(function (data) {
                                  //we parse the json answer
                                  if (data.result != "true") {
                                      notifyError($.t("widgets/chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                                      return;
                                  }

                                  //we make the serie
                                  var plot = [];
                                  var range = [];

                                  var lastDate;
                                  var d;
                                  var IndexDevice = index;

                                  if (!isSummaryData) {
                                      //data comes from acquisition table
                                      $.each(data.data.data, function (index, value) {
                                          lastDate = d;
                                          d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();

                                          var v;
                                          if (!isNullOrUndefined(value.key)) {
                                              v = parseFloat(value.key);
                                          } else {
                                              throw Error("Unable to parse answer");
                                          }

                                          //we manage the missing data
                                          if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                                             (lastDate + timeBetweenTwoConsecutiveValues < d)) {

                                              plot.push([lastDate + 1, null]);
                                          }

                                          plot.push([d, v]);
                                      });
                                  } else {
                                      //it is summarized data so we can get min and max curve
                                      var vMin;
                                      var vMax;

                                      $.each(data.data.data, function (index, value) {
                                          lastDate = d;
                                          d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
                                          var v;
                                          if (!isNullOrUndefined(value.avg)) {
                                              v = parseFloat(value.avg);
                                              vMin = parseFloat(value.min);
                                              vMax = parseFloat(value.max);
                                          } else {
                                              throw Error("Unable to parse answer");
                                          }

                                          //we manage the missing data
                                          if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                                             (lastDate + timeBetweenTwoConsecutiveValues < d)) {

                                              if (device.content.PlotType == "arearange")
                                                  range.push([d, null, null]);

                                              plot.push([d, null]);
                                          }

                                          if (device.content.PlotType == "arearange")
                                              range.push([d, vMin, vMax]);

                                          plot.push([d, v]);
                                      });
                                  }
                                  var color     = "#606060";// default color
				  var colorAxis = "#606060";// default color
                                  try {
									  color = device.content.color;
									  if (!parseBool(self.widget.configuration.oneAxis.checkbox))
                                         colorAxis = device.content.color;
										  
                                  } catch (err) {
                                      console.log(err);
                                  }

                                  //choose the axis id
                                  var yAxisName = 'axis' + self.seriesUuid[index];
								  
                                  if (parseBool(self.widget.configuration.oneAxis.checkbox)) {
                                      yAxisName = 'axis' + self.seriesUuid[0];
                                  }

                                  //create axis if needed
                                  if (isNullOrUndefined(self.chart.get(yAxisName))) {
                                      try {
                                          function isOdd(num) {
                                              return num % 2;
                                          }

                                          if (isOdd(index))
                                              align = 'left';
                                          else
                                              align = 'right';

                                          var unit = $.t(self.keywordInfo[index].units);

                                          if (unit == undefined)
                                              unit = "";

                                          self.chart.addAxis({ // new axis
                                              id: yAxisName, //The same id as the serie with axis at the beginning
                                              title: {
                                                  text: self.keywordInfo[index].friendlyName,
                                                  style: {
                                                      color: colorAxis
                                                  }
                                              },
                                              labels: {
                                                  align: align,
                                                  format: '{value:.1f} ' + unit,
                                                  style: {
                                                      color: colorAxis
                                                  }
                                              },
                                              opposite: isOdd(index)
                                          }, false, false, false);

                                      } catch (err) {
                                          console.log('Fail to create axis (for index = ' + index + ') : ' + err);
                                      }
                                  } else {
                                      console.log('Axis already exists (for index = ' + index + ')');
                                  }

                                  if ((parseBool(self.widget.configuration.oneAxis.checkbox))) {
									  
                                      //Configure the min/max in this case
                                      try {
                                          var yAxis = self.chart.get(yAxisName);
										  
										  // Avec un seul axe, pas de nom
										  yAxis.setTitle({text:""});
										  
										  //console.log (self.$chart);

                                          if (parseBool(self.widget.configuration.oneAxis.content.customYAxisMinMax.checkbox)) {
                                              //we manage min and max scale y axis
                                              var min = parseFloat(self.widget.configuration.oneAxis.content.customYAxisMinMax.content.minimumValue);
                                              var max = parseFloat(self.widget.configuration.oneAxis.content.customYAxisMinMax.content.maximumValue);
                                              yAxis.setExtremes(min, max);
                                          } else {
                                              //we cancel previous extremes
                                              yAxis.setExtremes(null, null);
                                          }
                                      } catch (err) {
                                          console.log(err);
                                      }
                                  }		  
								  
                                  try {
                                      if (device.content.PlotType == "arearange") {

                                          //Add Line
                                          self.chart.addSeries({
                                              id: self.seriesUuid[index],
                                              data: plot,
                                              dataGrouping: {
                                                  enabled: false
                                              },
                                              name: self.keywordInfo[index].friendlyName,
                                              marker: {
                                                  enabled: null,
                                                  radius: 2,
                                                  symbol: "circle"
                                              },
                                              color: color,
                                              yAxis: yAxisName,
                                              lineWidth: 2,
                                              type: 'line'
                                          }, false, false); // Do not redraw immediately

                                          //Add Ranges
                                          if (isSummaryData) {
                                              self.chart.addSeries({
                                                  id: 'range_' + self.seriesUuid[index],
                                                  data: range,
                                                  dataGrouping: {
                                                      enabled: false
                                                  },											  
                                                  name: self.keywordInfo[index].friendlyName + '(Min,Max)',
                                                  linkedTo: self.seriesUuid[index],
                                                  color: color,
                                                  yAxis: yAxisName,
                                                  type: device.content.PlotType,
                                                  lineWidth: 0,
                                                  fillOpacity: 0.3,
                                                  zIndex: 0
                                              }, false, false); // Do not redraw immediately

                                              var serie_range = self.chart.get('range_' + self.seriesUuid[index]);

                                              // Add Units for ranges
                                              if (serie_range)
                                                  serie_range.units = $.t(self.keywordInfo[index].units);
                                          }
                                      } else {

                                          self.chart.addSeries({
                                              id: self.seriesUuid[index],
                                              data: plot,
                                              dataGrouping: {
                                                  enabled: false
                                              },
                                              name: self.keywordInfo[index].friendlyName,
                                              marker: {
                                                  enabled: true,
                                                  radius: 2,
                                                  symbol: "circle"
                                              },											  
                                              color: color,
                                              yAxis: yAxisName,
                                              lineWidth: 2,
                                              step: self.isBoolVariable(index),
                                              type: device.content.PlotType,
                                              animation: false
                                          }, false, false); // Do not redraw immediately
                                      }
                                  } catch (err) {
                                      console.log('Fail to create serie : ' + err);
                                  }

                                  var serie = self.chart.get(self.seriesUuid[index]);

                                  //we save the unit in the serie
                                  if (serie)
								  {
                                      serie.units = $.t(self.keywordInfo[index].units);
									  
				      // If only one axis, we show the legend. In otherwise we destroy it
				      if (parseBool(self.widget.configuration.oneAxis.checkbox)) {
					serie.options.showInLegend = true;
					self.chart.legend.renderItem(serie);
				      }
				      else{
					serie.options.showInLegend = false;
					serie.legendItem = null;
					self.chart.legend.destroyItem(serie);									  
				      }
				      self.chart.legend.render();	

								  }
	  
                                      self.refreshingData = false;
                                      SeriesPromise.resolve(index);
                              })
                              .fail(function () {
                                  notifyError($.t("widgets/chart:errorDuringGettingDeviceData"));
                              });
                       }
                   });
               }
           } catch (err) {
               console.error(err.message);
               self.refreshingData = false;
           }
       };


       this.finalRefresh = function () {
          self = this;

          console.log ( this.chart );
          //console.log ( this.chart.series[0].xData.length );

          var NoAvailableData = true;

          $.each(this.chart.series, function (index, value) {
              if (value.xData.length != 0)
                NoAvailableData = false;
          });
          // TODO : To be finished !!
          // If for all data, length == 0, we display no Data Available
          if (NoAvailableData)
           {
              self.chart.showLoading($.t("widgets/chart:NoAvailableData"));
           }
           else
           {
              self.chart.hideLoading();
           }

           self.chart.redraw(false); //without animation
       };

       this.DisplaySummary = function (index, nb, device, range, Prefix) {
           var self = this;

           try {
               //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
               //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.

               var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(Prefix).subtract(1, 'seconds'));
               var dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(nb, range).startOf(Prefix));

               $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + "/" + Prefix + "/" + dateFrom + "/" + dateTo)
                  .done(function (data) {
                      try {
                          if (data.data.data[0] != undefined) {
                              self.chart.hideLoading(); // If a text was displayed before
                              self.chart.get(self.seriesUuid[index]).addPoint([DateTimeFormatter.isoDateToDate(data.data.data[0].date)._d.getTime().valueOf(), parseFloat(data.data.data[0].avg)], true, false, true);

                              //Add also for ranges if any
                              var serie = self.chart.get('range_' + self.seriesUuid[index]);
                              if (serie)
                                  serie.addPoint([DateTimeFormatter.isoDateToDate(data.data.data[0].date)._d.getTime().valueOf(), parseFloat(data.data.data[0].min), parseFloat(data.data.data[0].max)], true, false, true);
                          }
                      } catch (err) {
                          console.error(err.message);
                      }

                  });
           } catch (err) {
               console.error(err.message);
           }
       };

       /**
       * New acquisition handler
       * @param keywordId keywordId on which new acquisition was received
       * @param data Acquisition data
       */
       this.onNewAcquisition = function (keywordId, data) {
           var self = this;
           var bShift = false;

           if (self.seriesUuid.length == 0)
               return;

           try {
               $.each(self.widget.configuration.devices, function (index, device) {
                   if (keywordId == device.content.source.keywordId) {
                       //we've found the device
                       var cleanValue;
                       // Cleaning ranges switch
                       switch (self.interval) {
                           case "HOUR":
                               cleanValue = 3600000;
                               break;
                           case "DAY":
                               cleanValue = 3600000 * 24;
                               break;
                           case "WEEK":
                               cleanValue = 3600000 * 24 * 7;
                               break;
                           case "MONTH":
                               cleanValue = 3600000 * 24 * 30;
                               break;
                           case "HALF_YEAR":
                               cleanValue = 3600000 * 24 * 182;
                               break;
                           case "YEAR":
                               cleanValue = 3600000 * 24 * 365;
                               break;
                           default:
                               cleanValue = 3600000;
                               break;
                       }

                       var serie = self.chart.get(self.seriesUuid[index]);
                       var serie_range = self.chart.get('range_' + self.seriesUuid[index]);

                       // If a serie is available
                       if (!isNullOrUndefined(serie)) {
                           // Clean points > cleanValue for serie
                           self.cleanUpChart(serie, data.date, cleanValue);

                           // Clean points > cleanValue for ranges, if any
                           if (!isNullOrUndefined(serie_range))
                               self.cleanUpChart(serie_range, data.date, cleanValue);

                           // Add new point depending of the interval
                           switch (self.interval) {
                               case "HOUR":
                                   console.log(serie);

                                   if (!isNullOrUndefined(serie))
                                   {
                                       self.chart.hideLoading(); // If a text was displayed before
                                       serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
                                   }
                                   break;
                               case "DAY":

                                   if ((data.date.valueOf() - serie.points[serie.points.length - 1].x) > 3600000 * 2)
                                       self.DisplaySummary(index, 1, device, "hours", "hour");
                                   break;

                               case "WEEK":

                                   if ((data.date.valueOf() - serie.points[serie.points.length - 1].x) > 3600000 * 2)
                                       self.DisplaySummary(index, 1, device, "weeks", "hour");

                                   break;
                               case "MONTH":

                                   if ((data.date.valueOf() - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2)
                                       self.DisplaySummary(index, 1, device, "months", "day");

                                   break;
                               case "HALF_YEAR":

                                   if ((data.date.valueOf() - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2)
                                       self.DisplaySummary(index, 6, device, "months", "day");

                                   break;
                               case "YEAR":

                                   if ((data.date.valueOf() - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2)
                                       self.DisplaySummary(index, 1, device, "years", "day");

                                   break;
                               default:
                                   break;
                           }
                       }
                   }
               });
           } catch (err) {
               console.log(err);
           }
       };
   };
