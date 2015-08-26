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
      this.rangeAreasUuid = [];

	  this.interval = 0;
	  
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

         var btns = self.widget.$gridsterWidget.find(".nav-btn");

         $.each(btns, function (index, btn) {
            $(btn).unbind("click").bind("click", self.navigatorBtnClick($(btn).attr("level")));

            //we ensure that the configured interval is selected
            if ($(btn).attr("level") == self.widget.configuration.interval) {
               $(btn).addClass("btn-primary");
            }
         });

         //we create an uuid for each serie
         $.each(self.widget.configuration.devices, function (index, device) {
            //we update uuid if they don't exist
            if (isNullOrUndefined(self.seriesUuid[index]))
               self.seriesUuid = createUUID();
            if (isNullOrUndefined(self.rangeAreasUuid[index]))
               self.rangeAreasUuid = createUUID();
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
		
		 self.interval = interval;
		
         console.log("step 1 " + moment().format("HH:mm:ss'SSS"));

         try {
            if (!self.refreshingData) {
               this.chart.showLoading($.t("chart:loadingData"));
               self.refreshingData = true;
               //we compute the date from the configuration
               var dateFrom = "";
               var dateTo = ""; 
			   console.log (dateTo);
               var prefixUri = "";
               var timeBetweenTwoConsecutiveValues;
               var isSummaryData;

               switch (interval) {
                  case "HOUR" :
				  
					 //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
					 //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
				 
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('minute').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'hours').startOf('minute'));
                     //we request all data
                     timeBetweenTwoConsecutiveValues = undefined;
                     isSummaryData = false;
                     break;
                  default:
                  case "DAY" :
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('hour').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'days').startOf('hour'));
                     //we request hour summary data
                     prefixUri = "/hour";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600;
                     isSummaryData = true;
                     break;
                  case "WEEK" :
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('hour').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'weeks').startOf('hour'));
                     //we request hour summary data
                     prefixUri = "/hour";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600;
                     isSummaryData = true;
                     break;
                  case "MONTH" :
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'months').startOf('day'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
                  case "HALF_YEAR" :
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(6, 'months').startOf('day'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
                  case "YEAR" :
				     dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf('day').subtract(1, 'seconds'));
                     dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(1, 'years').startOf('day'));
                     //we request day summary data
                     prefixUri = "/day";
                     timeBetweenTwoConsecutiveValues = 1000 * 3600 * 24;
                     isSummaryData = true;
                     break;
               }
               console.log("step 2 " + moment().format("HH:mm:ss'SSS"));

               var curvesToLoad = self.widget.configuration.devices.length;
               //for each plot in the configuration we request for data
               $.each(self.widget.configuration.devices, function (index, device) {

                  $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo)
                     .done(function( data ) {
                        //we parse the json answer
                        if (data.result != "true")
                        {
                           notifyError($.t("chart:errorDuringGettingDeviceData"), JSON.stringify(data));
                           return;
                        }
                        console.log("step 3 " + moment().format("HH:mm:ss'SSS"));

                        //we hide the loading for the last item
                        curvesToLoad--;
                        if (curvesToLoad == 0)
                           self.chart.hideLoading();

                        //we save interval in the chart
                        self.chart.interval = interval;

                        //we remove last series
                        var serie = self.chart.get(self.seriesUuid[index]);
                        if (!isNullOrUndefined(serie))
                           serie.remove();

                        if (parseBool(device.content.rangeArea)) {
                           var serie = self.chart.get(self.rangeAreasUuid[index]);
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
                        var color = "black";
                        try {
                           color = device.content.color;
                        }
                        catch(err) {}

                        //marker of points is enable when there is less than 50 points on the line
                        self.chart.addSeries({id:self.seriesUuid[index],
                                              data:plot, name:"", marker : { enabled : (plot.length < 50), radius : 3, symbol: "circle"}, color: color});
                        console.log("step 6 " + moment().format("HH:mm:ss'SSS"));
                        //we add min and max series
                        if (isSummaryData) {
                           if (parseBool(device.content.rangeArea)) {

                              self.chart.addSeries({id:self.rangeAreasUuid[index], type: 'arearange', lineWidth: 0, fillOpacity: 0.3, zIndex: 0,
                                             data:range, color : color});

                              //we add attribute to hide it in legend
                              var serie = self.chart.get(self.rangeAreasUuid[index]);
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

                        self.refreshingData = false;

                        //we get the unit of the keyword
                        KeywordManager.get(device.content.source.keywordId, function(keyword) {
                           var serie = self.chart.get(self.seriesUuid[index]);
                           //we save the unit in the serie
                           if (serie) {
                              serie.units = $.t(keyword.units);
                              serie.name = keyword.friendlyName;
                           }
                        });

                        console.log("step 9 " + moment().format("HH:mm:ss'SSS"));
                     })
                     .fail(function() {notifyError($.t("chart:errorDuringGettingDeviceData"));});
               });
            }
         }
         catch (err) {
            console.error(err.message);
            self.refreshingData = false;
         }
      };

	   this.DisplaySummary = function (index, nb, device, range, Prefix) {
		     self = this;
		     console.log ("Display Summary");
			 
			 try{
				 //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
				 //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
				 
			 var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf( Prefix ).subtract(1, 'seconds'));
			 var dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(nb, range).startOf( Prefix ));
			 
			 $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId +"/"+ Prefix + "/" + dateFrom + "/" + dateTo )
			 .done(function( data ) 
			 {
				 console.log ("Nouvelle valeur moyenne");
				 self.chart.get(self.seriesUuid[index]).addPoint([DateTimeFormatter.isoDateToDate(data.data.data[0].date)._d.getTime().valueOf(), parseFloat(data.data.data[0].avg)], true, false, true);
				 console.log ("---");
			 });
			 }
             catch (err) 
			 {
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
		 
         try {
            $.each(self.widget.configuration.devices, function (index, device) {
               if (searchedDevice == device.content.source) {
                  //we've found the device
				  
				   // Cleaning ranges switch
				   switch ( self.interval ) 
				   {
					  case "HOUR" :
                         cleanValue = 3600000;
						 break;
					  case "DAY" :
					     cleanValue = 3600000 * 24;
						 break;
					  case "WEEK" :
					     cleanValue = 3600000 * 24 * 7;
						 break;
					  case "MONTH" :
					     cleanValue = 3600000 * 24 * 30;
						 break;
					  case "HALF_YEAR" :
					     cleanValue = 3600000 * 24 * 182;
						 break;
					  case "YEAR" :
					     cleanValue = 3600000 * 24 * 365;
						 break;
					  default:
					     cleanValue = 3600000;
						 break;					 
				   }
				  
				  var serie = self.chart.get(self.seriesUuid[index]);
				  
				  console.log ( serie );
				  
				  // Clean points > cleanValue
				  if (!isNullOrUndefined( serie.points[0] ))
				  {
					  while ( (data.date.valueOf() - serie.points[0].x) > cleanValue )
					  {
						  serie.removePoint ( 0 );
					  }
				  }
				  
				  // Add new point depending of the interval
                   switch ( self.interval ) 
				   {
					  case "HOUR" :
					     self.chart.get(self.seriesUuid[index]).addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
						 break;
					  case "DAY" : 
					  
					     console.log ( data.date.valueOf() - serie.points[serie.points.length-1].x );
					  
				         if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 2 )
						 {
                            self.DisplaySummary ( index, 1, device, "hours", "hour" );
						 }					  
					     break;
					  case "WEEK" :

					     console.log ( data.date.valueOf() - serie.points[serie.points.length-1].x );
					  
				         if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 2 )
						 {
							 self.DisplaySummary ( index, 1, device, "weeks", "hour" );
						 }
						 break;
					  case "MONTH" :
					  
					  	 console.log ( data.date.valueOf() - serie.points[serie.points.length-1].x );
					  
				         if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
						 {
							 self.DisplaySummary ( index, 1, device, "months", "day" );
						 }
						 break;
					  case "HALF_YEAR" :
					  
					  	 console.log ( data.date.valueOf() - serie.points[serie.points.length-1].x );
					  
				         if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
						 {
							 self.DisplaySummary ( index, 6, device, "months", "day" );
						 }					  
						 break;
					  case "YEAR" :
					  	 console.log ( data.date.valueOf() - serie.points[serie.points.length-1].x );
					  
				         if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
						 {
							 self.DisplaySummary ( index, 1, device, "years", "day" );
						 }					  
						 break;
					  default:
						 break;					 
				   }
				   
				   //Enabling/disabling marker
				   if (serie.points.length > 50)
				   {
					   if (serie.options.marker.enabled)
					   {
						  serie.options.marker.enabled = false;
						  serie.setVisible (true, true);
					   }
				   }
				   else
				   {
					   if (!(serie.options.marker.enabled))
					   {
						 serie.options.marker.enabled = true;
						 serie.setVisible ( true, true );
					   }
				   }
               }
            });
         }
         catch (err) {
         }
      };

      this.getDevicesForAcquisitions = function() {
         var self = this;
         var result = [];

         try {
            $.each(self.widget.configuration.devices, function (index, device) {
               try {
                  result.push(device.content.source);
               }
               catch (err) {
               }
            });
         }
         catch (err) {
         }

         return result;
      };

   };
