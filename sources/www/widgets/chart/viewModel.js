widgetViewModelCtor =

/**
 * Create a Chart ViewModel
 * @constructor
 */
   function chartViewModel() {

       this.refreshingData = false;
       this.seriesUuid = [];

       //Keyword Id List !
       this.devicesList = [];
       this.interval = 0;
       this.deviceInfo = [];
       this.keywordInfo = [];
       this.cleaningTask = null;
       this.differentialDisplay = [];
       this.incompatibility = false;
       
       //This variable is used in differential display
       this.chartLastValue = [];
       
       //this variable is for the selection of the computed value used for the chart (min, avg, max)
       this.periodValueType = [];

       /**
        * Initialization method
        */
       this.initialize = function () {

           var self = this;
           var d = new $.Deferred();

           // create the chart
           self.$chart = self.widgetApi.find("div.container");

           self.widgetApi.loadLibrary([
               "libs/highstock/js/highstock.js",
               "libs/highstock/js/highcharts-more.js",
               "libs/highstock/js/modules/exporting.js",
               "libs/highcharts-export-clientside/js/highcharts-export-clientside.min.js"
           ]).done(function () {

               self.chartOption = {
                   chart: {
                       type: 'line',
                       marginTop: 10
                   },
                   legend: {
                       layout: 'horizontal',
                       align: 'center',
                       verticalAlign: 'bottom',
                       borderWidth: 0,
                       enabled: true
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
                               if (self.chart.interval) {
                                   switch (self.chart.interval) {
                                       default:
                                       case "HOUR":
                                       case "DAY":
                                           return DateTimeFormatter.dateToString(this.value, "LT");

                                       case "WEEK":
                                       case "MONTH":
                                       case "HALF_YEAR":
                                       case "YEAR":
                                           return DateTimeFormatter.dateToString(this.value, "L");
                                   }
                               }
                               return DateTimeFormatter.dateToString(this.value);
                           }
                       }
                   },

                   yAxis: {
                       // Default Axis
                   },

                   plotOptions: {
                       bar: {
                           pointPadding: 0.2
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

               self.$chart.highcharts('StockChart', self.chartOption);
               self.chart = self.$chart.highcharts();
               
               self.widgetApi.toolbar({
                   activated: true,
                   displayTitle: true,
                   items: [
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HOUR\"><span data-i18n=\"widgets/chart:navigator.hour\"/></div>" },
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"DAY\"><span data-i18n=\"widgets/chart:navigator.day\"/></div>"},
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"WEEK\"><span data-i18n=\"widgets/chart:navigator.week\"/></div>"},
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"MONTH\"><span data-i18n=\"widgets/chart:navigator.month\"/></div>"},
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HALF_YEAR\"><span data-i18n=\"widgets/chart:navigator.half_year\"/></div>"},
                   { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"YEAR\"><span data-i18n=\"widgets/chart:navigator.year\"/></div>" },
                   { separator: ""},
                   { custom: "<div class=\"widget-toolbar-button export-btn dropdown\">" +
                                "<a id=\"chartExportMenu" + self.widget.id + "\" data-target=\"#\" class=\"widget-toolbar-button export-btn dropdown\" role=\"button\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\">" +
                                    "<span class=\"fa fa-bars\"/>" +
                                "</a>" +
                                "<ul class=\"dropdown-menu\" aria-labelledby=\"chartExportMenu" + self.widget.id + "\">" +
                                    "<li><span class=\"print-command\" data-i18n=\"widgets/chart:export.print\"></span></li>" +
                                    "<li role=\"separator\" class=\"divider\"></li>" +
                                    "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.png\" mime-type=\"image/png\"></span></li>" +
                                    "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.jpeg\" mime-type=\"image/jpeg\"></span></li>" +
                                    "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.svg\" mime-type=\"image/svg+xml\"></span></li>" +
                                    "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.csv\" mime-type=\"text/csv\"></span></li>" +
                                    "<li><span class=\"export-command\" data-i18n=\"widgets/chart:export.xls\" mime-type=\"application/vnd.ms-excel\"></span></li>" +
                                "</ul>" +
                             "</div>"
                       }
                   ]
               });

               //we manage toolbar buttons

               var $btns = self.widgetApi.find(".range-btn");
               $btns.unbind("click").bind("click", self.navigatorBtnClick());

               self.widgetApi.find(".print-command").unbind("click").bind("click", function () {
                   self.chart.print();
               });

               self.widgetApi.find(".export-command").unbind("click").bind("click", function (e) {
                   self.chart.exportChartLocal({
                       type: $(e.currentTarget).attr("mime-type"),
                       filename: 'export'
                   });
               });
               
               // TODO : Request the server for its current time
               self.widgetApi.askServerLocalTime(function (serverLocalTime) {
                  // ...
               });
               
               d.resolve();
           })
           .fail(function (error) {
               //TODO : To be tested
               notifyError($.t("widgets/chart:errorInitialization"), error);
               throw $.t("widgets/chart:errorInitialization");
           });
           return d.promise();
       };

       this.resized = function () {
           var self = this;
           if (!isNullOrUndefined(self.chart)) {
               this.chart.setSize(self.widget.getWidth() - 30, self.widget.getHeight() - 40, false);

               $(window).trigger("resize");
           }
       };

       this.configurationChanged = function () {
           var self = this;
           
           // Reset of some values
           self.periodValueType = [];
           self.seriesUuid = [];

           if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
               return;

           //Desactivate the old button
           self.widgetApi.find(".range-btn[interval='" + self.interval + "']").removeClass("widget-toolbar-pressed-button");

           self.interval = self.widget.configuration.interval;

           //Activate the new button
           self.widgetApi.find(".range-btn[interval='" + self.interval + "']").addClass("widget-toolbar-pressed-button");

           //just update some viewmodel info
           self.devicesList = self.widget.configuration.devices.slice(0);        
           
           var arrayOfDeffered = [];

           //we create an uuid for each serie
           $.each(self.widget.configuration.devices, function (index, device) {

               //we update uuid if they don't exist
               if (isNullOrUndefined(self.seriesUuid[index]))
                   self.seriesUuid[index] = createUUID();

               //we register keyword new acquisition
               self.widgetApi.registerKeywordAcquisitions(device.content.source.keywordId);
               
               // We ask the current device name
               var deffered = DeviceManager.get(device.content.source.deviceId);
               arrayOfDeffered.push(deffered);
               deffered.done(function (data) {
                   self.deviceInfo[index] = data;
               });

               //we ask the current value
               var deffered2 = KeywordManager.get(device.content.source.keywordId);
               arrayOfDeffered.push(deffered2);
               deffered2.done(function (keyword) {
                   self.keywordInfo[index] = keyword;
                   try{
                      if (parseBool(device.content.advancedConfiguration.checkbox)){
                         
                         // read the differential display variable
                         if (device.content.advancedConfiguration.content.differentialDisplay ==="relative")
                            self.differentialDisplay[index] = true;
                         else
                            self.differentialDisplay[index] = false;
                        
                         // read the period value we need
                         self.periodValueType[index] = device.content.advancedConfiguration.content.periodtype;

                      }else{ // automatic managment : the managment of the information is done from the measure type of the keyword                   
                        if (keyword.measure === "Cumulative"){
                           self.differentialDisplay[index] = true;
                           self.periodValueType[index] = "max";
                        }
                        else{ // Default values and Absolute value
                           self.differentialDisplay[index] = false;
                           self.periodValueType[index] = "avg";
                        }
                     }
                  }
                  catch(error)
                  {
                     console.warn('exception detecting : automatic management');
                     if (keyword.measure === "Cumulative"){
                        self.differentialDisplay[index] = true;
                        self.periodValueType[index] = "max";
                     }
                     else{ // Default values and Absolute value
                        self.differentialDisplay[index] = false;
                        self.periodValueType[index] = "avg";
                     }
                  }
                  
                  if (self.differentialDisplay[index] && device.content.PlotType === "arearange")
                  {
                     notifyError($.t("widgets/chart:incompatibilityDifferential"), "error");
                     self.incompatibility = true;
                     return;
                  }
                  else
                     self.incompatibility = false;
                   
               });
           });

           $.whenAll(arrayOfDeffered).done(function () {
               //we use the periodic task of cleaning
               clearInterval(self.cleaningTask);
               self.cleaningTask = setInterval(self.periodicCleaningTask.bind(self), 60000); // Cleaning every minute
               self.refreshData(self.widget.configuration.interval);
           });
       };

       this.navigatorBtnClick = function () {
           var self = this;
           return function (e) {
               //we manage activation
               var interval = $(e.currentTarget).attr("interval");

               //we manage button inversion
               self.widgetApi.find(".range-btn[interval='" + interval + "']").addClass("widget-toolbar-pressed-button");
               self.widgetApi.find(".range-btn[interval!='" + interval + "']").removeClass("widget-toolbar-pressed-button");

               self.refreshData(interval);
           };
       };

       this.isBoolVariable = function (index) {
           var self = this;
           if ((self.keywordInfo[index]) && (self.keywordInfo[index].type === "Bool")) {
               return true;
           }
           else {
               return false;
           }
       }
       
       this.isEnumVariable = function (index) {
           var self = this;
           if ((self.keywordInfo[index]) && (self.keywordInfo[index].type === "enum")) {
               return true;
           }
           else {
               return false;
           }
       }       

       this.refreshData = function (interval) {
           var self = this;

           if (self.chart) {
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
                       var deviceIsSummary = [];
					   var ChartIndex = 0;

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

                       var arrayOfDeffered = [];
                       self.chartLastValue = [];
                       
                       //for each plot in the configuration we request for data
                       $.each(self.widget.configuration.devices, function (index, device) {

                           var displayData = true;

                           //If the device is a bool, you have to modify
                           if (self.isBoolVariable(index)) {
                               switch (interval) {
                                   case "DAY":
                                   case "WEEK":
                                   case "MONTH":
                                       dateTo = DateTimeFormatter.dateToIsoDate(moment());
                                       displayData = true;
                                       break;
                                   case "HALF_YEAR":
                                   case "YEAR":
                                       // Display that the range is too large
                                       self.chart.showLoading($.t("widgets/chart:RangeTooBroad"));
                                       displayData = false;
                                       self.refreshingData = false;
                                       break;
                               }
                               //we request hour summary data
                               prefixUri = "";
                               isSummaryData = false;
                               deviceIsSummary[index] = false; // We change the summary for the boolean device.
                           } else {
                               displayData = true;
                               deviceIsSummary[index] = isSummaryData; // By default, it's the summary define above.
                           }

                           if (displayData) {

                               var deffered = RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
                               arrayOfDeffered.push(deffered);
                               deffered.done(function (data) {

                                   //we make the serie
                                   var plot = [];
                                   var range = [];

                                   var lastDate;
                                   var d;
                                   var legendText = "";

                                   if (!(deviceIsSummary[index])) {
                                       //data comes from acquisition table
                                       $.each(data.data, function (index2,value) {
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
                                           
                                           // The differential display is disabled if the type of the data is enum or boolean
                                           if (self.differentialDisplay[index] && !self.isBoolVariable(index) && !self.isEnumVariable(index))
                                           {
                                              if (!isNullOrUndefined(self.chartLastValue[index]))
                                                 plot.push([d, v-self.chartLastValue[index]]);

                                              self.chartLastValue[index] = v;
                                           }
                                           else // standard display
                                              plot.push([d, v]);
                                       });
                                   } else {
                                      
                                       //it is summarized data so we can get min and max curve
                                       var vMin;
                                       var vMax;

                                       $.each(data.data, function (index2, value) {
                                           lastDate = d;
                                           d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
                                           var vplot;
                                           
                                           if (!isNullOrUndefined(value[self.periodValueType[index]])) {
                                               // read the computed desired value (avg/min/max)
                                               vplot = parseFloat(value[self.periodValueType[index]]);
                                               vMin = parseFloat(value.min);
                                               vMax = parseFloat(value.max);
                                           } else {
                                               throw Error("Unable to parse answer");
                                           }

                                           //we manage the missing data
                                           if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                                           (lastDate + timeBetweenTwoConsecutiveValues < d)) {

                                               if (device.content.PlotType === "arearange")
                                                   range.push([d, null, null]);

                                               plot.push([d, null]);
                                           }

                                           // The differential display is disabled if the type of the data is enum or boolean
                                           if (self.differentialDisplay[index] && !self.isBoolVariable(index) && !self.isEnumVariable(index))                                              
                                           {  
                                              if (!isNullOrUndefined(self.chartLastValue[index]))
                                                 plot.push([d, vplot-self.chartLastValue[index]]);
                                              
                                              self.chartLastValue[index] = vplot;
                                           }
                                           else{
                                              if (device.content.PlotType === "arearange")
                                                  range.push([d, vMin, vMax]);

                                              plot.push([d, vplot]);                                                   
                                           }
                                       });
                                   }
                                   var color = "#606060"; // default color
                                   var colorAxis = "#606060"; // default color
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
                                   
                                   // axes and series names
                                   try {
                                      if (self.widget.configuration.legendLabels ==="Device")
                                         legendText = self.deviceInfo[index].friendlyName;
                                      else if (self.widget.configuration.legendLabels ==="Keyword")
                                         legendText = self.keywordInfo[index].friendlyName;                                       
                                      else
                                         legendText = self.deviceInfo[index].friendlyName + "/" + self.keywordInfo[index].friendlyName;
                                   }catch(error){ // If the configuration doesn't exist (migration, ...) -> default value
                                      legendText = self.deviceInfo[index].friendlyName + "/" + self.keywordInfo[index].friendlyName;
                                   }                                   
                                   
                                   //create axis if needed
                                   if (isNullOrUndefined(self.chart.get(yAxisName))) {
                                       try {
                                           function isOdd(num) {
                                               return num % 2;
                                           }

                                           var align = 'right';
                                           if (isOdd(index))
                                               align = 'left';

                                           var unit = $.t(self.keywordInfo[index].units);

                                           if (unit == undefined)
                                               unit = "";

                                           self.chart.addAxis({
                                               // new axis
                                               id: yAxisName, //The same id as the serie with axis at the beginning
                                               reversedStacks: false,
                                               title: {
                                                   text: legendText,
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

                                       } catch (error) {
                                           console.log('Fail to create axis (for index = ' + index + ') : ' + error);
                                       }
                                   } else {
                                       console.log('Axis already exists (for index = ' + index + ')');
                                   }

                                   if ((parseBool(self.widget.configuration.oneAxis.checkbox))) {

                                       //Configure the min/max in this case
                                       try {
                                           var yAxis = self.chart.get(yAxisName);

                                           // Avec un seul axe, pas de nom
                                           yAxis.setTitle({ text: "" });

                                           if (parseBool(self.widget.configuration.oneAxis.content.customYAxisMinMax.checkbox)) {
                                               //we manage min and max scale y axis
                                               var min = parseFloat(self.widget.configuration.oneAxis.content.customYAxisMinMax.content.minimumValue);
                                               var max = parseFloat(self.widget.configuration.oneAxis.content.customYAxisMinMax.content.maximumValue);
                                               yAxis.setExtremes(min, max);
                                           } else {
                                               //we cancel previous extremes
                                               yAxis.setExtremes(null, null);
                                           }
                                       } catch (error2) {
                                           console.log(error2);
                                       }
                                   }

                                   try {
                                      
                                       // Standard options
                                       var serieOption = {
                                          id: self.seriesUuid[index],
                                          index: ChartIndex,
                                          data: plot,
                                          dataGrouping: {
                                             enabled: false
                                          },
                                          name: legendText,
                                          connectNulls: self.isBoolVariable(index),
                                          marker: {
                                             enabled: null,
                                             radius: 2,
                                             symbol: "circle"
                                          },
                                          color: color,
                                          yAxis: yAxisName,
                                          lineWidth: 2,
                                          animation: false
                                       };
                                      
                                       if (device.content.PlotType === "arearange") { // arearange
                                           serieOption.type = 'line';
                                       }else {                                        // default option
                                           serieOption.step = self.isBoolVariable(index);  // For boolean values, create steps.
                                           serieOption.type = device.content.PlotType;
                                           serieOption.marker.enabled = true;
                                       }
                                       
                                       //Add plot
                                       self.chart.addSeries( serieOption, false, false); // Do not redraw immediately
										         ChartIndex = ChartIndex + 1;
                                           
                                       if (device.content.PlotType === "arearange") {
                                           //Add Ranges
                                           if (deviceIsSummary[index]) {
                                               self.chart.addSeries({
                                                   id: 'range_' + self.seriesUuid[index],
                                                   index: ChartIndex,
                                                   data: range,
                                                   dataGrouping: {
                                                       enabled: false
                                                   },
                                                   name: legendText + '(Min,Max)',
                                                   linkedTo: self.seriesUuid[index],
                                                   color: color,
                                                   yAxis: yAxisName,
                                                   type: device.content.PlotType,
                                                   connectNulls: false,
                                                   lineWidth: 0,
                                                   fillOpacity: 0.3,
                                                   zIndex: 0
                                               }, false, false); // Do not redraw immediately
											   
                                               ChartIndex = ChartIndex + 1;

                                               var serieRange = self.chart.get('range_' + self.seriesUuid[index]);

                                               // Add Units for ranges
                                               if (serieRange)
                                                   serieRange.units = $.t(self.keywordInfo[index].units);
                                           }
                                       }
                                   } catch (err2) {
                                       console.log('Fail to create serie : ' + err2);
                                   }

                                   var serie = self.chart.get(self.seriesUuid[index]);

                                   //we save the unit in the serie
                                   if (serie) {
                                       serie.units = $.t(self.keywordInfo[index].units);

                                       // If only one axis, we show the legend. In otherwise we destroy it
                                       if (parseBool(self.widget.configuration.oneAxis.checkbox)) {
                                           serie.options.showInLegend = true;
                                           self.chart.legend.renderItem(serie);
                                       } else {
                                           serie.options.showInLegend = false;
                                           serie.legendItem = null;
                                           self.chart.legend.destroyItem(serie);
                                       }
                                       self.chart.legend.render();

                                   }

                                   self.refreshingData = false;
                               })
                               .fail(function (error) {
                                  notifyError($.t("widgets/chart:errorDuringGettingDeviceData"), error);
                               });
                           }
                       });

                       $.whenAll(arrayOfDeffered).done(function () {
                           self.finalRefresh();
                       });
                   }
               } catch (err) {
                   console.error(err.message);
                   self.refreshingData = false;
               }
           }
       };


       this.finalRefresh = function () {
           var self = this;

           var noAvailableData = true;

           $.each(self.chart.series, function (index, value) {
               if (value.xData.length !== 0)
                   noAvailableData = false;
           });

           // If for all data, length == 0, we display no Data Available
           if (noAvailableData && !self.incompatibility) {
               self.chart.showLoading($.t("widgets/chart:noAvailableData"));
           }
           else if (self.incompatibility) {
              self.chart.showLoading($.t("widgets/chart:incompatibilityDifferential"));
           }
           else {
               self.chart.hideLoading();
           }

           self.chart.redraw(false); //without animation
       };
	   
       this.cleanUpChart = function (serie, finaldate, dateInMilliSecondes) {
           var ex = false;

           var isofinaldate = DateTimeFormatter.isoDateToDate(finaldate)._d.getTime().valueOf();
                      
           while (!ex) {
               if (!isNullOrUndefined(serie.points)) {
                   if (!isNullOrUndefined(serie.points[0])) {
                       if ((isofinaldate - serie.points[0].x) > dateInMilliSecondes)
                           serie.removePoint(0, true); // If false, we never delete the point -> infinite loop
                       else
                           ex = true;
                   } else
                       ex = true;
               } else
                   ex = true;
           }
       };      
      
       this.periodicCleaningTask = function()
       {
         var self = this;
         
         console.log ("cleaning chart");
         
         var cleanValue;
         var prefix;
         
         // Cleaning ranges switch
         switch (self.interval) {
             case "HOUR":
                 cleanValue = 3600000;
                 prefix = "minute";
                 break;
             case "DAY":
                 cleanValue = 3600000 * 24;
                 prefix = "hour";
                 break;
             case "WEEK":
                 cleanValue = 3600000 * 24 * 7;
                 prefix = "hour";
                 break;
             case "MONTH":
                 cleanValue = 3600000 * 24 * 30;
                 prefix = "day";
                 break;
             case "HALF_YEAR":
                 cleanValue = 3600000 * 24 * 182;
                 prefix = "day";
                 break;
             case "YEAR":
                 cleanValue = 3600000 * 24 * 365;
                 prefix = "day";
                 break;
             default:
                 cleanValue = 3600000;
                 prefix = "hour";
                 break;
         }
         
         var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(prefix).subtract(1, 'seconds'));
         
         $.each(self.seriesUuid, function (index, value) {
         
            var serie = self.chart.get(value);
            var serieRange = self.chart.get('range_' + value);
             
            // If a serie is available  // Clean points > cleanValue for serie
            if (!isNullOrUndefined(serie))
               self.cleanUpChart(serie, dateTo, cleanValue);

             // Clean points > cleanValue for ranges, if any
            if (!isNullOrUndefined(serieRange))
               self.cleanUpChart(serieRange, dateTo, cleanValue);
         });
       };
      
       this.DisplaySummary = function (index, nb, device, range, prefix) {
           var self = this;

           try {
               //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
               //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
               
               var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(prefix).subtract(1, 'seconds'));
               var dateFrom = DateTimeFormatter.dateToIsoDate(moment().subtract(nb, range).startOf(prefix));

               RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + "/" + prefix + "/" + dateFrom + "/" + dateTo)
                  .done(function (data) {
                      try {
                          if (data.data[data.data.length-1] != undefined) {
                              self.chart.hideLoading(); // If a text was displayed before
							  
                              var serie = self.chart.get(self.seriesUuid[index]);
                              var serieRange = self.chart.get('range_' + self.seriesUuid[index]);
                              var registerDate = DateTimeFormatter.isoDateToDate(data.data[data.data.length-1].date)._d.getTime().valueOf();
                              
                              if (self.differentialDisplay[index])
                              {
                                  if (!isNullOrUndefined(self.chartLastValue[index]))
                                  {
                                     serie.addPoint([registerDate, parseFloat(data.data[data.data.length-1][self.periodValueType[index]])-self.chartLastValue[index]], 
                                                    true, 
                                                    false, 
                                                    true);
                                  }
                                  
                                  self.chartLastValue[index] = parseFloat(data.data[data.data.length-1][self.periodValueType[index]]);
                              }
                              else                              
                                 serie.addPoint([registerDate, parseFloat(data.data[data.data.length-1][self.periodValueType[index]])], true, false, true);
						      
                              //Add also for ranges if any
                              if (serieRange && !self.differentialDisplay[index])
                              {                           
                                 serieRange.addPoint([registerDate, parseFloat(data.data[data.data.length-1].min), parseFloat(data.data[data.data.length-1].max)], true, false, true);
                              }
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
           if (self.chart) {

               if (self.seriesUuid.length === 0)
                   return;

               try {
                   $.each(self.widget.configuration.devices, function (index, device) {
                       if (keywordId === device.content.source.keywordId) {
                           //we've found the device

                           var serie = self.chart.get(self.seriesUuid[index]);

                           // If a serie is available
                           if (!isNullOrUndefined(serie)) {

                              // date received in iso format to compare
                              var isolastdate = DateTimeFormatter.isoDateToDate(data.date)._d.getTime().valueOf();
                           
                               // Add new point depending of the interval
                               switch (self.interval) {
                                   case "HOUR":
                                       if (!isNullOrUndefined(serie)) {
                                           self.chart.hideLoading(); // If a text was displayed before
                                           
                                           if (self.differentialDisplay[index])
                                           {
                                              if (serie.points.length > 0 && !isNullOrUndefined(self.chartLastValue[index]))
                                              {
                                                 serie.addPoint([data.date.valueOf(), parseFloat(data.value) - self.chartLastValue[index]], true, false, true);
                                              }
                                              self.chartLastValue[index] = parseFloat(data.value);                                                 
                                           }
                                           else
                                              serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
                                       }
                                       break;
                                   case "DAY":
                                       if ((serie.points.length > 0) && ((isolastdate - serie.points[serie.points.length - 1].x) > 3600000 * 2))
                                           self.DisplaySummary(index, 1, device, "hours", "hour");
                                       break;

                                   case "WEEK":
                                       if ((serie.points.length > 0) && ((isolastdate - serie.points[serie.points.length - 1].x) > 3600000 * 2))
                                           self.DisplaySummary(index, 1, device, "hours", "hour");

                                       break;
                                   case "MONTH":
                                       if ((serie.points.length > 0) && ((isolastdate - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2))
                                           self.DisplaySummary(index, 1, device, "days", "day");

                                       break;
                                   case "HALF_YEAR":

                                       if ((serie.points.length > 0) && ((isolastdate - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2))
                                           self.DisplaySummary(index, 6, device, "days", "day");

                                       break;
                                   case "YEAR":

                                       if ((serie.points.length > 0) && ((isolastdate - serie.points[serie.points.length - 1].x) > 3600000 * 24 * 2))
                                           self.DisplaySummary(index, 1, device, "days", "day");

                                       break;
                                   default:
                                       break;
                               }
                           }
                       }
                   });
               } catch (err) {
                   console.error(err);
               }
           }
       };
       
       // TODO this function will be called every minute (from server time)
       this.onTime = function (serverLocalTime) {
          // ...
       }
   };