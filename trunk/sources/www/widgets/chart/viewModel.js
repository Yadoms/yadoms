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
			   type : 'line',
               marginTop: 10
            },
            navigator : {
               adaptToUpdatedData: false,
               enabled : false
            },
			credits: {
				enabled: false
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

            yAxis : { // Default Axis
            },

            plotOptions : {
			  bar: {
                pointPadding: 0.2
              },
			  
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

			if ( this.widget.height() == 100 )
			{
				//Pas d'affichage de titre pour une case de hauteur
				this.chart.setTitle({text: null});
			}
			else
			{
				this.chart.setTitle({text: this.widget.configuration.chartTitle});
			    this.chart.setTitle({y: this.widget.height() - 15});
			}
			
            $(window).trigger("resize");
         }
      };

      this.configurationChanged = function() {

         var self = this;

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         var btns = self.widget.$gridsterWidget.find(".nav-btn");
		 
	     btns.css("position", "relative");
		 btns.css("left", "70px");

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
						
						   if ( (!parseBool(device.content.ownAxis)) )
						   {
					         try{
								 
								 function isOdd(num) { return num % 2;}
								 
								 if (isOdd(index))
									 align = 'left';
								 else
									 align ='right';
								 
								 if (self.chart.get('axis' + self.seriesUuid[index]) == null )
								 {
								   self.chart.addAxis({ // new axis
										id: 'axis' + self.seriesUuid[index], //The same id as the serie with axis at the beginning
										labels: {
											align: align,
											style: {
												color: color
											}
										},
										opposite: isOdd ( index )
									});
								 }
							  }
							  catch (err)
								 {
									 console.log ( err );
								 }
                           }
						
						console.log ( device.content.PlotType );
						
                        //marker of points is enable when there is less than 50 points on the line
                        self.chart.addSeries({id:self.seriesUuid[index],
                                              data:plot, name:"", marker : { enabled : (plot.length < 50), radius : 3, symbol: "circle"}, color: color , yAxis: 'axis' + self.seriesUuid[index], type: device.content.PlotType }
											  , false); // Do not redraw immediately
                        console.log("step 6 " + moment().format("HH:mm:ss'SSS"));
						
                        //we add min and max series
                        if (isSummaryData) {
                           if (parseBool(device.content.rangeArea)) {

                              self.chart.addSeries({id:self.rangeAreasUuid[index], type: 'arearange', lineWidth: 0, fillOpacity: 0.3, zIndex: 0,
                                             data:range, color : color, yAxis: 'axis' + self.seriesUuid[index], type: device.content.PlotType}
											 , false); // Do not redraw immediately

                              //we add attribute to hide it in legend
                              var serie = self.chart.get(self.rangeAreasUuid[index]);
                              serie.hideInLegend = true;
                              console.log("step 7 " + moment().format("HH:mm:ss'SSS"));
                           }
                        }

                        try {
							
							var yAxis = self.chart.get( 'axis' + self.seriesUuid[index]);
							
                           if (parseBool(self.widget.configuration.customYAxisMinMax.checkbox))
						   {
                              //we manage min and max scale y axis
                              var min = parseFloat(self.widget.configuration.customYAxisMinMax.content.minimumValue);
                              var max = parseFloat(self.widget.configuration.customYAxisMinMax.content.maximumValue);
							  yAxis.setExtremes(min, max);
                           }
                           else {
                              //we cancel previous extremes
							  yAxis.setExtremes(null, null);
                           }
                        }
                        catch (err) {
                        }

                        console.log("step 8 " + moment().format("HH:mm:ss'SSS"));

                        self.refreshingData = false;

                        //we get the unit of the keyword
                        KeywordManager.get(device.content.source.keywordId, function(keyword) {
                           var serie = self.chart.get(self.seriesUuid[index]);
						   
                           //we save the unit in the serie and in the yAxis
                           if (serie ) 
						   {
                              serie.units = $.t(keyword.units);
                              serie.name = keyword.friendlyName;
							  
							  if ( (!parseBool(device.content.ownAxis)) )
							  {  
								  yAxis.setTitle({ 
								                  text: keyword.friendlyName,
												  style: {
												       color: color
											      }
								  }, true); // Do not redraw immediately ?? Bizarre
								  
								  // Change the axis Title
								  yAxis.update ({ //Set labels //SetOptions
											labels: {
												format: '{value} ' + serie.units
										   }
								  });
							  }
                           }
                        });

                        console.log("step 9 " + moment().format("HH:mm:ss'SSS"));
                     })
                     .fail(function() {notifyError($.t("chart:errorDuringGettingDeviceData"));});
					 
					 self.chart.redraw ( true );
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
			 
			 try{
				 //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
				 //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
				 
			 var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf( Prefix ).subtract(1, 'seconds'));
			 var dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(nb, range).startOf( Prefix ));
			 

			 $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId +"/"+ Prefix + "/" + dateFrom + "/" + dateTo )
			 .done(function( data ) 
			 {
				 try
				 {
				    self.chart.get(self.seriesUuid[index]).addPoint([DateTimeFormatter.isoDateToDate(data.data.data[0].date)._d.getTime().valueOf(), parseFloat(data.data.data[0].avg)], true, false, true);
				 }
				 catch (err) 
				 {
					 console.error(err.message);
				 }				 
				 
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
		 
		 console.log ( self.chart.yAxis );
		 
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
						  //serie.setVisible (false, true);
						  serie.options.marker.enabled = false;
						  //serie.setVisible (true, true);
						  self.chart.redraw();
					   }
				   }
				   else
				   {
					   if (!(serie.options.marker.enabled))
					   {
						 //serie.setVisible (false, true);
						 serie.options.marker.enabled = true;
						 //serie.setVisible ( true, true );
						 self.chart.redraw();
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