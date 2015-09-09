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
						 if (isNullOrUndefined(this.point.low))
						 { //Standard serie
                             s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                this.series.name + " : " + this.y.toFixed(1) + " " + this.series.units;
						 }
						 else
						 { //Range Serie
                             s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                this.series.name + " : " + this.point.low.toFixed(1) + "-" + this.point.high.toFixed(1) + " " + this.series.units;							 
						 }
                     }
                  });
                  return s;
               },
			   shared : true
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
				//No display of the title when height = 1 case
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
		 btns.css("left", "100px"); // TODO : Déplacer au milieu  lorsque le graphique est plus grand
		 
         $.each(btns, function (index, btn) {
            $(btn).unbind("click").bind("click", self.navigatorBtnClick($(btn).attr("level")));
			
			switch (index)
			{
				case 0:
				   $(btn)[0].innerText = self.widget.package.navigator.hour;
				   break;
				case 1:
				   $(btn)[0].innerText = self.widget.package.navigator.day;
				   break;
				case 2:
				   $(btn)[0].innerText = self.widget.package.navigator.week;
				   break;
				case 3:
				   $(btn)[0].innerText = self.widget.package.navigator.month;
				   break;
				case 4:
				   $(btn)[0].innerText = self.widget.package.navigator.half_month;
				   break;
				case 5:
				   $(btn)[0].innerText = self.widget.package.navigator.year;
				   break;
				default:
				   break;				   
			}

            //we ensure that the configured interval is selected
            if ($(btn).attr("level") == self.widget.configuration.interval) {
               $(btn).addClass("btn-primary");
            }
         });
		 
		  if (self.devicesList.length == 0)
		  {
		     self.devicesList = self.widget.configuration.devices.slice(0);
		  }
		  else
		  {
			  var tempDevicesList = self.widget.configuration.devices.slice(0); 
			  
				$.each(self.devicesList, function (index, device) 
				{
					var isFound = false;
					
					$.each(tempDevicesList, function (index_temp, device_temp) 
					{	
						//Si already registered, we remove
						if ( device.content.source.keywordId == device_temp.content.source.keywordId )
						  isFound = true;
					});
					
					if (!isFound)
					{		
							//we remove last series
							var serie = self.chart.get(self.seriesUuid[index]);
							console.log ( serie );
							
							if (!isNullOrUndefined(serie))
							   serie.remove( false );
						   
						   //we remove last ranges if any
						    serie = self.chart.get('range_' + self.seriesUuid[index]);

							if (!isNullOrUndefined(serie))
							   serie.remove( false );

							  //Delete the axis
							var yAxis = self.chart.get( 'axis' + self.seriesUuid[index]);							
							
							//If Unique Axis, we don't delete it !
							if (!isNullOrUndefined(yAxis))
							{
							    if  (
								    (!(parseBool(self.widget.configuration.oneAxis))) ||
								    ((index !=0) && (parseBool(self.widget.configuration.oneAxis)))
									)
								{	
									yAxis.remove ( false );
								}
							}
							
							// Delele the assign id for serie if defined !
							if (!isNullOrUndefined(self.seriesUuid[index]) && (index >-1))
								self.seriesUuid.splice(index,1);
					}
				});			  
			  
			  //We copy the new list
			  self.devicesList = tempDevicesList.slice(0);
		  }
		  
         //we create an uuid for each serie
         $.each(self.widget.configuration.devices, function (index, device) {
            //we update uuid if they don't exist
            if (isNullOrUndefined(self.seriesUuid[index]))
               self.seriesUuid[index] = createUUID();
         });		  
		 
         //we ask for device information
         this.refreshData(this.widget.configuration.interval, moment());
      }; 

	  this.cleanUpChart = function(serie, time, cleanValue) { 
	     var self = this;
		 
		 var b_sortie = false;
		 
		  while ( !b_sortie )
		  {
			  if (!isNullOrUndefined( serie.points ))
			  {
				  if (!isNullOrUndefined( serie.points[0] ))
				  {  
					  if ((time.valueOf() - serie.points[0].x) > cleanValue)
						 serie.removePoint ( 0, true ); // If false, we never delete the point -> infinite loop
					  else
						  b_sortie = true;
				  }
				  else
					  b_sortie = true;
			  }
			  else
				  b_sortie = true;
		  }
	  }
	  
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
				 
				     dateTo = DateTimeFormatter.dateToIsoDate(moment());
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

			      console.log ( "rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo );
			   
                  $.getJSON("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo)
                     .done(function( data ) {
						 console.log ("done :", "rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
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
                           serie.remove( false );
					   
					    //we remove last axis
					    var yAxis = self.chart.get( 'axis' + self.seriesUuid[index]);							
							
						//If Unique Axis, we don't delete it !
						if (!isNullOrUndefined(yAxis))
						   yAxis.remove ( false );
					   
					    //we remove ranges if any
                        var serie_range = self.chart.get('range_' + self.seriesUuid[index]);
                        if (!isNullOrUndefined(serie_range))
                           serie_range.remove( false );

                        //we make the serie
                        var plot = [];
                        var range = [];						

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
                        }
                        catch(err) {
							console.log ( err );
						}
						
						   // We create axis only if each serie have is own axis
						   if ( !(parseBool(self.widget.configuration.oneAxis) ) || 
						        ((index == 0) && ( parseBool(self.widget.configuration.oneAxis) ))
							  )
						   {
					         try{
								 
								 function isOdd(num) { return num % 2;}
								 
								 if (isOdd(index))
									 align = 'left';
								 else
									 align ='right';
								 
								 if (self.chart.get('axis' + self.seriesUuid[index]) == null )
								 {
									 console.log ("Add Axis", 'axis' + self.seriesUuid[index] );
									 console.log ( self.seriesUuid );
									 console.log ( index );
									 
								   self.chart.addAxis({ // new axis
										id: 'axis' + self.seriesUuid[index], //The same id as the serie with axis at the beginning
										labels: {
											align: align,
											style: {
												color: color
											}
										},
										opposite: isOdd ( index )
									}, false, false, false);
								 }
							  }
							  catch (err)
								 {
									 console.log ( err );
								 }
                           }
						
						var yAxisName = "";
						
                           if ( (parseBool(self.widget.configuration.oneAxis) ))
						   {	
                               yAxisName = 	'axis' + self.seriesUuid[0];
							   
							   //Configure the min/max in this case
								try {
									var yAxis = self.chart.get( yAxisName );
									
								   if (parseBool( self.widget.configuration.customYAxisMinMax.checkbox) )
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
							   
						   }
						   else{
							   yAxisName = 	'axis' + self.seriesUuid[index];
						   }
						
                        if (device.content.PlotType == "arearange")
						{
							console.log ("Search Axis", yAxisName );
							console.log ( self.chart );
							console.log ( plot );							
							
							//Add Line
							self.chart.addSeries({id:self.seriesUuid[index],
												  data:plot, 
												  name:"", 
												  marker : { enabled : null, radius : 3, symbol: "circle"}, 
												  color: color , 
												  yAxis: yAxisName, 
												  type: 'line',
												  }
												  , false, false); // Do not redraw immediately
							
							//Add Ranges
							if (isSummaryData) {
								self.chart.addSeries({id:'range_' + self.seriesUuid[index],
													  data:range, 
													  name:"",
													  linkedTo: self.seriesUuid[index],
													  color: color , 
													  yAxis: yAxisName, 
													  type: device.content.PlotType,
													  lineWidth: 0,
													  fillOpacity: 0.3,
													  zIndex: 0													  
													  }
													  , false, false); // Do not redraw immediately
							}
							
							console.log ( self.chart );
							console.log ("addSeries");							
						}						  
						else
						{
							console.log ("Search Axis", yAxisName );
							console.log ( self.chart.yAxis );
							console.log ( plot );
							
							self.chart.addSeries({id:self.seriesUuid[index],
												  data:plot, 
												  name: "", 
												  marker : { enabled : true, radius : 3, symbol: "circle"}, 
												  color: color , 
												  yAxis: yAxisName, 
												  type: device.content.PlotType,
												  animation : false
												  }
												  , false, false); // Do not redraw immediately
												  
							console.log ("addSeries");
						}
                        console.log("step 6 " + moment().format("HH:mm:ss'SSS"));

                        self.refreshingData = false;

                        //we get the unit of the keyword
                        KeywordManager.get(device.content.source.keywordId, function(keyword) {
							
                           var serie = self.chart.get(self.seriesUuid[index]);
						   var yAxis = self.chart.get( 'axis' + self.seriesUuid[index]);
						   
                           //we save the unit in the serie and in the yAxis
                           if (serie) 
						   {
                              serie.units = $.t(keyword.units);
                              serie.name = keyword.friendlyName;
							  
							  if ( (!parseBool(self.widget.configuration.oneAxis)))
							  {  
								  yAxis.setTitle({ 
								                  text: keyword.friendlyName,
												  style: {
												       color: color
											      }
								  }, false);
								  
								  // Change the axis Title
								  yAxis.update ({ //Set labels
											labels: {
												format: '{value} ' + serie.units
										   }
								  }, false); 
							  }
 
                              //unique Axis
						      if ((index == 0) && ( parseBool(self.widget.configuration.oneAxis) ))
							  {
								  yAxis.setTitle({ 
								                  text: serie.units										      
												  }, false);
								  
								  // Change the axis Title
								  yAxis.update ({ //Set labels
											labels: {
												format: '{value} ' + serie.units
										   }
								  }, false); 								  
							  }
                           }
						   
						  //Ranges Names
						  var serie_range = self.chart.get('range_' + self.seriesUuid[index]);
						  
						   if (serie_range) 
						   {
							  serie_range.units = $.t(keyword.units);
							  serie_range.name = keyword.friendlyName + '(Min,Max)';
						   }
						   
						   console.log ("redraw");
						   self.chart.redraw ( false ); //sans animation
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
					
					//Add also for ranges if any
					var serie = self.chart.get('range_' + self.seriesUuid[index]);
					if (serie)
						serie.addPoint([DateTimeFormatter.isoDateToDate(data.data.data[0].date)._d.getTime().valueOf(), parseFloat(data.data.data[0].min), parseFloat(data.data.data[0].max)], true, false, true);
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
				  var serie_range = self.chart.get('range_' + self.seriesUuid[index]);
				  
				  // If a serie is available
				  if (!isNullOrUndefined( serie ))
				  {  
					  // Clean points > cleanValue for serie
					  self.cleanUpChart ( serie, data.date, cleanValue );
					  
					  // Clean points > cleanValue for ranges, if any
					  if (!isNullOrUndefined( serie_range ))							  
						 self.cleanUpChart ( serie_range, data.date, cleanValue );
				  
					  // Add new point depending of the interval
					   switch ( self.interval ) 
					   {
						  case "HOUR" :
							   serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
                               //No point for the range
							 break;
						  case "DAY" : 
						  
							 if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 2 )
								self.DisplaySummary ( index, 1, device, "hours", "hour" );
							 break;
							 
						  case "WEEK" :
						  
							 if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 2 )
								 self.DisplaySummary ( index, 1, device, "weeks", "hour" );
							 
							 break;
						  case "MONTH" :
						  
							 if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
								 self.DisplaySummary ( index, 1, device, "months", "day" );

							 break;
						  case "HALF_YEAR" :
						  
							 if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
								 self.DisplaySummary ( index, 6, device, "months", "day" );

							 break;
						  case "YEAR" :
						  
							 if ( (data.date.valueOf() - serie.points[serie.points.length-1].x) > 3600000 * 24 * 2 )
								 self.DisplaySummary ( index, 1, device, "years", "day" );

							 break;
						  default:
							 break;					 
					   }
				   }
               }
            });
         }
         catch (err) {
			 console.log ( err );
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
				   console.log ( err );
               }
            });
         }
         catch (err) {
			 console.log ( err );
         }
         return result;
      };
   };