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
   this.initialize = function(widget) {
      this.widget = widget;
      var self = this;

      // create the chart
      this.$chart = self.widget.$gridWidget.find("div.chartWidgetContainer");

      this.chartOption = {
         chart: {
            type: 'line',
            marginTop: 10
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
            text: ''
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
               formatter: function() {
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
            formatter: function() {
               var s = "<b>" + DateTimeFormatter.dateToString(this.x, "llll") + "</b>";
               $.each(this.points, function() {
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

         series: []
      };
      
      
      this.$chart.highcharts('StockChart', this.chartOption);

      this.chart = this.$chart.highcharts();
   };

   this.resized = function() {
      var self = this;

      if (!isNullOrUndefined(this.chart)) {
         this.chart.setSize(this.widget.width(), this.widget.height() - 10, false);

         /* No Title at all
         if ( this.widget.height() == 100 )
         {
         	//No display of the title when height = 1 case
         	this.chart.setTitle({text: null});
         }
         else
         {
         	this.chart.setTitle({text: this.widget.configuration.chartTitle});
             this.chart.setTitle({y: this.widget.height() - 15});
         }
         */

         var btns = self.widget.$gridWidget.find(".nav-btn");

         if (!isNullOrUndefined(btns)) {
            btns.css("position", "relative");
            btns.css("left", "100px");
            btns.css("top", self.widget.height() - 35);
         }

         $(window).trigger("resize");
      }
   };

   this.configurationChanged = function() {

      var self = this;

      if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
         return;

      self.interval = this.widget.configuration.interval;

      var btns = self.widget.$gridWidget.find(".nav-btn");

      $.each(btns, function(index, btn) {
         $(btn).unbind("click").bind("click", self.navigatorBtnClick($(btn).attr("level")));

         switch (index) {
            case 0:
               $(btn).css("data-i18n", "navigator.hour");
               break;
            case 1:
               $(btn).css("data-i18n", "navigator.day");
               break;
            case 2:
               $(btn).css("data-i18n", "navigator.week");
               break;
            case 3:
               $(btn).css("data-i18n", "navigator.month");
               break;
            case 4:
               $(btn).css("data-i18n", "navigator.half_month");
               break;
            case 5:
               $(btn).css("data-i18n", "navigator.year");
               break;
            default:
               break;
         }

         //we ensure that the configured interval is selected
         if ($(btn).attr("level") == self.widget.configuration.interval) {
            $(btn).addClass("btn-primary");
         }
      });

      //just update some viewmodel info
      self.seriesUuid = [];
      self.devicesList = self.widget.configuration.devices.slice(0);
      
      
	  self.ChartPromise = new PromiseCounter (self.devicesList.length, this.refreshData.bind(this), this.widget.configuration.interval);

      //we create an uuid for each serie
      $.each(self.widget.configuration.devices, function(index, device) {
         //we update uuid if they don't exist
         if (isNullOrUndefined(self.seriesUuid[index]))
            self.seriesUuid[index] = createUUID();

         KeywordManager.get(device.content.source.keywordId, function(keyword) {
            self.keywordInfo[index] = keyword;

            self.ChartPromise.resolve(index);
         });
      });
   };

   this.cleanUpChart = function(serie, time, cleanValue) {
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

   this.navigatorBtnClick = function(interval) {
      var self = this;
      return function(e) {
         //we manage activation
         self.widget.$gridWidget.find(".nav-btn[level!='" + interval + "']").addClass("btn-default").removeClass("btn-primary");
         self.widget.$gridWidget.find(".nav-btn[level='" + interval + "']").addClass("btn-primary").removeClass("btn-default");

         self.refreshData(interval);
      };
   };

	this.isBoolVariable = function( index )  {
		var self = this;
	   //console.log ("Evaluation bool");
	   if (self.keywordInfo[index].type == "Bool")
	   {
		   //console.log ("bool=true");
		  return true;
	   }
	  else
	  {
		  //console.log ("bool=false");							  
		  return false;
	  }
	}      
   
   this.refreshData = function(interval) {
      var self = this;

      self.interval = interval;

      //we save interval in the chart
      self.chart.interval = interval;

      
      console.log("step 1 " + moment().format("HH:mm:ss'SSS"));

      try {
         if (!self.refreshingData) {
            self.chart.showLoading($.t("chart:loadingData"));
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
            console.log("step 2 " + moment().format("HH:mm:ss'SSS"));

            //ensure all series and axis are removed (may cause some crash if not done)
            while (self.chart.series.length > 0)
              self.chart.series[0].remove(false);

            while (self.chart.yAxis.length > 0)
              self.chart.yAxis[0].remove(false);
           
            var SeriesPromise = new PromiseCounter (self.widget.configuration.devices.length, this.finalRefresh.bind(this), null);
			
            //for each plot in the configuration we request for data
            $.each(self.widget.configuration.devices, function(index, device) {

			     var DisplayData = true;
			     //If the device is a bool, you have to modify
				 if  (self.isBoolVariable(index))
				 {
					 switch (interval) {
                        case "DAY" :
                        case "WEEK" :
                        case "MONTH" :						
					      dateTo = DateTimeFormatter.dateToIsoDate(moment());
						  DisplayData = true;
						  break;
                        case "HALF_YEAR" :
                        case "YEAR" :
						  // Display that the range is too large
                          self.chart.showLoading($.t("chart:RangeTooBroad"));
                          DisplayData = false;
                          self.refreshingData = false;						  
						  break;
					 }
					 //we request hour summary data
					 prefixUri = "";
					 isSummaryData = false;						 
				 }
				 else{
					 DisplayData = true;
				 }			
			
			   if (DisplayData) {
			
               $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo)
                  .done(function(data) {
                     console.log("done :", "rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
                     //we parse the json answer
                     if (data.result != "true") {
                        notifyError($.t("chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                        return;
                     }
                     console.log("step 3 " + moment().format("HH:mm:ss'SSS"));

                     //we make the serie
                     var plot = [];
                     var range = [];

                     var lastDate;
                     var d;
                     var IndexDevice = index;

                     if (!isSummaryData) {
                        console.log("step 4 " + moment().format("HH:mm:ss'SSS"));
                        //data comes from acquisition table
                        $.each(data.data.data, function(index, value) {
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

                        $.each(data.data.data, function(index, value) {
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
                     console.log("step 5 " + moment().format("HH:mm:ss'SSS"));
                     var color = "black";
                     try {
                        color = device.content.color;
                     } catch (err) {
                        console.log(err);
                     }

                     //choose the axis id
                     var yAxisName = 'axis' + self.seriesUuid[index];
                     if( parseBool(self.widget.configuration.oneAxis) ) {
                        yAxisName = 'axis' + self.seriesUuid[0];
                     }
                     
                     //create axis if needed
                     if (isNullOrUndefined(self.chart.get(yAxisName))) {
                        console.log('Axis do not exist, create it (for index = ' + index + ')');
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
                                    color: color
                                 }
                              },
                              labels: {
                                 align: align,
                                 format: '{value} ' + unit,
                                 style: {
                                    color: color
                                 }
                              },
                              opposite: isOdd(index)
                           }, false, false, false);
                           
                           if (isNullOrUndefined(self.chart.get(yAxisName))) {
                              console.error("Impossible de créer un axe !!!!");
                           }
                        } catch (err) {
                           console.log('Fail to create axis (for index = ' + index + ') : ' + err);
                        }                        
                     } else {
                        console.log('Axis already exists (for index = ' + index + ')');
                     }
                     
                     if ((parseBool(self.widget.configuration.oneAxis))) {
                        //Configure the min/max in this case
                        try {
                           var yAxis = self.chart.get(yAxisName);

                           if (parseBool(self.widget.configuration.customYAxisMinMax.checkbox)) {
                              //we manage min and max scale y axis
                              var min = parseFloat(self.widget.configuration.customYAxisMinMax.content.minimumValue);
                              var max = parseFloat(self.widget.configuration.customYAxisMinMax.content.maximumValue);
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
                           console.log(yAxisName);

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
                           console.log(yAxisName);   
						   
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
                        serie.units = $.t(self.keywordInfo[index].units);

                     self.refreshingData = false;
                     SeriesPromise.resolve(index);
                  })
                  .fail(function() {
                     notifyError($.t("chart:errorDuringGettingDeviceData"));
                  });
			   }
            });
         }
      } catch (err) {
         console.error(err.message);
         self.refreshingData = false;
      }
   };


   this.finalRefresh = function() {
      this.chart.hideLoading();
      this.chart.redraw(false); //without animation
   };

   this.DisplaySummary = function(index, nb, device, range, Prefix) {
      var self = this;

      try {
         //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
         //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.

         var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(Prefix).subtract(1, 'seconds'));
         var dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(nb, range).startOf(Prefix));

         $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + "/" + Prefix + "/" + dateFrom + "/" + dateTo)
            .done(function(data) {
               try {
                  if(data.data.data[0] != undefined) {
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
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(searchedDevice, data) {
      var self = this;
      var bShift = false;

      if (self.seriesUuid.length == 0)
         return;

      try {
         $.each(self.widget.configuration.devices, function(index, device) {
            if (searchedDevice == device.content.source) {
               //we've found the device

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

                  //console.log ( parseFloat(data.value) );
                  console.log(serie);

                  // Add new point depending of the interval
                  switch (self.interval) {
                     case "HOUR":
                        console.log(serie);

                        if (!isNullOrUndefined(serie))
                           serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
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

   this.getDevicesForAcquisitions = function() {
      var self = this;
      var result = [];

      try {
         $.each(self.widget.configuration.devices, function(index, device) {
            try {
               result.push(device.content.source);
            } catch (err) {
               console.log(err);
            }
         });
      } catch (err) {
         console.log(err);
      }
      return result;
   };
};