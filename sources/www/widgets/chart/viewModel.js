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
       this.differentialDisplay = [];
       this.incompatibility = false;
       this.serverTime = null;
       this.precision = 1;
       this.ConfigurationLegendLabels="";
       
       // defined by the configuration of the widget chart for cleaning old points
       this.cleanValue = 0;
       this.prefix = "";
       
       //defined by the configuration of the widget chart for adding new summary points for continuous display
       this.summaryTimeBetweenNewPoint = 0;
       
       
       //This variable is used in differential display
       this.chartLastValue = [];
       
       //this variable is for the selection of the computed value used for the chart (min, avg, max)
       this.periodValueType = [];

       /**
        * Helpers
        */       
       
       function isOdd(num) {return num % 2;}
       
       this.changexAxisBound = function(dateMin){
          var self = this;
          
          var datet = DateTimeFormatter.isoDateToDate(dateMin)._d.getTime();
          self.chart.xAxis[0].setExtremes(datet, null);
       };
       
       this.calculateBeginDate = function(interval, time, prefix) {
        var dateValue;
        switch (interval) {
            case "HOUR":
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'hours').startOf(prefix));
                break;
            default:
            case "DAY": //we request hour summary data
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'days').startOf(prefix));
                break;
            case "WEEK": //we request hour summary data
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'weeks').startOf(prefix));
                break;
            case "MONTH": //we request day summary data
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'months').startOf(prefix));
                break;
            case "HALF_YEAR": //we request day summary data
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(6, 'months').startOf(prefix));
                break;
            case "YEAR": //we request day summary data
                dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'years').startOf(prefix));
                break;
        }
        return dateValue;
       };
       
       this.createAxis = function (index, configuration, device) {
           var self = this;
           
           var colorAxis = "#606060"; // default color
           var yAxisName;
           
           // treat oneAxis configuration option => axis name and color
           if (parseBool(configuration.oneAxis.checkbox)) {
              yAxisName = 'axis' + self.seriesUuid[0];
           }
           else {
              yAxisName = 'axis' + self.seriesUuid[index];
              colorAxis = device.content.color;
           }
           
           //create axis if needed
           if (isNullOrUndefined(self.chart.get(yAxisName))) {
               try {
                   
                   function getAxisTitle(){
                      // create the structure
                      var response= {
                         text: null,
                         style:{
                            color: colorAxis
                         }
                      };
                      return response;
                   }

                   var align = 'right';
                   if (isOdd(index))
                       align = 'left';

                   var unit="";
                   try {
                      unit = $.t(self.keywordInfo[index].units);
                   }
                   catch(error)
                   {
                      console.log ("unit is empty for keyword ", device.content.source.keywordId);
                   }

                   self.chart.addAxis({
                       // new axis
                       id: yAxisName, //The same id as the serie with axis at the beginning
                       reversedStacks: false,
                       title: getAxisTitle(),
                       labels: {
                           align: align,
                           format: '{value:.' + self.precision.toString() + 'f} ' + unit,
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

           if ((parseBool(configuration.oneAxis.checkbox))) {

               //Configure the min/max in this case
               try {
                   var yAxis = self.chart.get(yAxisName);

                   // Avec un seul axe, pas de nom
                   yAxis.setTitle({ text: "" });

                   if (parseBool(configuration.oneAxis.content.customYAxisMinMax.checkbox)) {
                       //we manage min and max scale y axis
                       var min = parseFloat(configuration.oneAxis.content.customYAxisMinMax.content.minimumValue);
                       var max = parseFloat(configuration.oneAxis.content.customYAxisMinMax.content.maximumValue);
                       yAxis.setExtremes(min, max);
                   } else {
                       //we cancel previous extremes
                       yAxis.setExtremes(null, null);
                   }
               } catch (error2) {
                   console.log(error2);
               }
           }
           
           return yAxisName; // Return the name of the axis
       };
       
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
               "libs/highstock/js/modules/offline-exporting.js",
               "libs/highstock/js/modules/canvas-tools.js",
               "libs/highstock/js/modules/boost.js",
               "libs/export-csv/js/export-csv.min.js",
               "libs/highcharts-export-clientside/js/highcharts-export-clientside.min.js"
           ]).done(function () {
               self.chartOption = {
                   chart: {
                       type: 'line',
                       animation: false,
                       marginTop: 10
                   },
                   boost: {
                      allowForce: true,
                      useGPUTranslations: true
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
                                           this.series.name + " : " + this.y.toFixed(this.series.chart.precision) + " " + this.series.units;
                                   } else { //Range Serie
                                       s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                           this.series.name + " : " + this.point.low.toFixed(this.series.chart.precision) + "-" + this.point.high.toFixed(this.series.chart.precision) + " " + this.series.units;
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
                   batteryItem: false,
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
                   window.location.reload(); //force page reload because after having displayed printing page, the widget disposition is erroneous
               });

               self.widgetApi.find(".export-command").unbind("click").bind("click", function (e) {
                   try{
                      self.chart.exportChartLocal({
                          type: $(e.currentTarget).attr("mime-type"),
                          filename: 'export'
                      });
                   }
                   catch(error)
                   {
                      notifyError($.t("widgets/chart:formatNotSupported", {format: $(e.currentTarget).attr("mime-type")}));
                   }
               });
               
               self.widgetApi.askServerLocalTime(function (serverLocalTime) {
                  self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
               }).done(function(data) {
                  d.resolve();
               })
               .fail(function(error) {
                  d.reject();
               });
           })
           .fail(function (error) {
               notifyError($.t("widgets/chart:errorInitialization"), error);
               throw $.t("widgets/chart:errorInitialization");
               d.reject();
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

       this.chartParametersConfiguration = function () {
         var self = this;
         
         // Cleaning ranges switch
         switch (self.interval) {
             case "HOUR":
                 self.cleanValue = 3600000;
                 self.prefix = "minute";
                 break;
             case "DAY":
                 self.cleanValue = 3600000 * 24;
                 self.prefix = "hour";
                 break;
             case "WEEK":
                 self.cleanValue = 3600000 * 24 * 7;
                 self.prefix = "hour";
                 break;
             case "MONTH":
                 self.cleanValue = 3600000 * 24 * 30;
                 self.prefix = "day";
                 break;
             case "HALF_YEAR":
                 self.cleanValue = 3600000 * 24 * 182;
                 self.prefix = "day";
                 break;
             case "YEAR":
                 self.cleanValue = 3600000 * 24 * 365;
                 self.prefix = "day";
                 break;
             default:
                 self.cleanValue = 3600000;
                 self.prefix = "hour";
                 break;
         }
         
         if (self.prefix === "hour")
            self.summaryTimeBetweenNewPoint = 3600000 * 2 + 60000;
         else if (self.prefix === "day")
            self.summaryTimeBetweenNewPoint = 3600000 * 24 * 2 + 60000;
       };
       
       this.configurationChanged = function () {
           var self = this;

           // Reset of some values
           self.periodValueType = [];
           self.seriesUuid = [];
           var d = new $.Deferred();

           if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
               return;


           self.interval = self.widget.configuration.interval;
           
           self.chartParametersConfiguration();
           
           //Desactivate the old button
           self.widgetApi.find(".range-btn[interval='" + self.interval + "']").removeClass("widget-toolbar-pressed-button");

           //Activate the new button
           self.widgetApi.find(".range-btn[interval='" + self.interval + "']").addClass("widget-toolbar-pressed-button");

           //just update some viewmodel info
           self.devicesList = self.widget.configuration.devices.slice(0);
            
           try{
             self.ConfigurationLegendLabels = self.widget.configuration.legends.content.legendLabels;
           }catch(error){ // If the configuration doesn't exist (migration, ...) -> default value
             console.log ("default value for legend labels : device name + keyword name");
           }            
           
           var arrayOfDeffered = [];

           //we create an uuid for each serie
           $.each(self.widget.configuration.devices, function (index, device) {
               //we update uuid if they don't exist
               if (isNullOrUndefined(self.seriesUuid[index]))
                   self.seriesUuid[index] = createUUID();
               
               // reading of the precision for the display of the value and the unit
               if (!isNullOrUndefined(device.content.advancedConfiguration.content.precision))
                  self.precision = parseInt(device.content.advancedConfiguration.content.precision, 10);
               else
                  self.precision = 1;
               
               self.chart.precision  = self.precision;
               
               // We ask the current device name
               var deffered = DeviceManager.get(device.content.source.deviceId);
               arrayOfDeffered.push(deffered);
               deffered.done(function (data) {
                   self.deviceInfo[index] = data;
               })
               .fail(function (error) {
                  notifyError($.t("widgets/chart:deviceNotFound", {Id: device.content.source.deviceId}));
               });

               //we ask the current value
               var deffered2 = self.widgetApi.getKeywordInformation(device.content.source.keywordId);
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
                     console.log('error detecting during defferential configuration display => automatic management.');
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
                  
                   //we register the keyword for new acquisition if the device exist
                   self.widgetApi.registerKeywordAcquisitions(device.content.source.keywordId);                  
               })
               .fail(function (error) {
                  notifyError($.t("widgets/chart:keywordNotFound", {Id: device.content.source.keywordId}));
                  //throw $.t("widgets/chart:errorInitialization");
               });               
           });
           
           // fail function doesn't exist for whenAll
           $.whenAll(arrayOfDeffered).done(function () {
               self.refreshData(self.widget.configuration.interval).always(function () {
                  d.resolve();
               })
               .fail(function (error) {
                  d.reject();
               });
           })
           
           return d.promise();
       };

       this.navigatorBtnClick = function () {
           var self = this;
           return function (e) {
               //we manage activation
               var interval = $(e.currentTarget).attr("interval");
               
               self.interval = interval;
               self.chartParametersConfiguration();

               //we manage button inversion
               self.widgetApi.find(".range-btn[interval='" + interval + "']").addClass("widget-toolbar-pressed-button");
               self.widgetApi.find(".range-btn[interval!='" + interval + "']").removeClass("widget-toolbar-pressed-button");

               //we ask the new time server, and we refresh information
               self.widgetApi.askServerLocalTime(function (serverLocalTime) {
                  self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
                  self.refreshData(interval);
               });
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
           var d = new $.Deferred();

           if (self.chart) {
               self.interval = interval;
               //we save interval in the chart
               self.chart.interval = interval;

               try {
                   if (!self.refreshingData) {
                       self.chart.showLoading($.t("widgets/chart:loadingData"));
                       self.refreshingData = true;
                       //debugger;
                       //we compute the date from the configuration
                       var dateFrom = self.calculateBeginDate(interval, self.serverTime, self.prefix);
                       //debugger;
                       var isSummaryData = true; // by default
                       var deviceIsSummary = [];
                       var dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf(self.prefix).subtract(1, 'seconds'));
                       var prefixUri = "/" + self.prefix;
                       var timeBetweenTwoConsecutiveValues = moment.duration(1, self.prefix).asMilliseconds();

                       if (interval == "HOUR")
                       {
                         dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime)); // rewriting the final time
                         //we request all data
                         timeBetweenTwoConsecutiveValues = undefined;
                         isSummaryData = false; // rewrite the isSummaryData
                         prefixUri = ""; // rewrite the prefix                          
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
                                       dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
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
                                       
                                       // Add here missing last data at the end
                                       if (!isNullOrUndefinedOrEmpty(data.data))
                                       {
                                          d = DateTimeFormatter.isoDateToDate(data.data[data.data.length-1].date)._d.getTime();
                                          var time = moment(self.serverTime).startOf(self.prefix)._d.getTime().valueOf();
                                          var registerDate = moment(self.serverTime).startOf(self.prefix).subtract(1, self.prefix + 's')._d.getTime().valueOf();
                                          
                                          if ((time - d) > self.summaryTimeBetweenNewPoint)
                                          {
                                              if (device.content.PlotType === "arearange")
                                                   range.push([registerDate, null, null]);

                                               plot.push([registerDate, null]);                                             
                                          }
                                       }
                                   }
                                   var axisName;
                                   try{
                                      axisName = self.createAxis(index, self.widget.configuration, device);
                                   }catch(error){
                                      console.error (error);
                                   }
                                   
                                   var legendText="";
                                   try{
                                      // series names
                                      if (self.ConfigurationLegendLabels ==="Device")
                                         legendText = self.deviceInfo[index].friendlyName;
                                      else if (self.ConfigurationLegendLabels ==="Keyword")
                                         legendText = self.keywordInfo[index].friendlyName;                                       
                                      else
                                         legendText = self.deviceInfo[index].friendlyName + "/" + self.keywordInfo[index].friendlyName;
                                   }catch(error)
                                   {
                                      legendText = $.t("widgets/chart:keywordNotFound", {Id: device.content.source.keywordId});
                                   }
                                   
                                   var serie = null;
                                   var legendConfiguration;
                                   try{
                                      legendConfiguration = parseBool(self.widget.configuration.legends.checkbox);
                                   }catch(err){
                                      legendConfiguration = true;
                                   }
                                   
                                   try {
                                       // Standard options
                                       var serieOption = {
                                          id: self.seriesUuid[index],
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
                                          color: device.content.color,
                                          yAxis: axisName,
                                          lineWidth: 2,
                                          showInLegend: legendConfiguration,
                                          animation: false
                                       };
                                      
                                       if (device.content.PlotType === "arearange") { // arearange
                                           serieOption.type = 'line';
                                       }else {                                             // default option
                                           serieOption.step = self.isBoolVariable(index);  // For boolean values, create steps.
                                           serieOption.type = device.content.PlotType;
                                       }
                                       
                                       //Add plot
                                       serie = self.chart.addSeries(serieOption, false, false); // Do not redraw immediately
                                           
                                       if (device.content.PlotType === "arearange") {
                                           //Add Ranges
                                           if (deviceIsSummary[index]) {
                                               var serieRange = null;
                                               
                                               serieRange = self.chart.addSeries({
                                                   id: 'range_' + self.seriesUuid[index],
                                                   data: range,
                                                   dataGrouping: {
                                                       enabled: false
                                                   },
                                                   name: legendText[index] + '(Min,Max)',
                                                   linkedTo: self.seriesUuid[index],
                                                   color: device.content.color,
                                                   yAxis: axisName,
                                                   type: device.content.PlotType,
                                                   connectNulls: false,
                                                   lineWidth: 0,
                                                   fillOpacity: 0.3,
                                                   zIndex: 0
                                               }, false, false); // Do not redraw immediately

                                               // Add Units for ranges
                                               if (serieRange)
                                               {
                                                  try{
                                                     serieRange.units = $.t(self.keywordInfo[index].units);
                                                  }
                                                  catch(error){
                                                     serieRange.units="";
                                                  }
                                               }
                                           }
                                       }
                                   } catch (err2){
                                       console.error('Fail to create serie : ' + err2);
                                   }

                                   if (serie) {
                                      
                                      //we save the unit in the serie for tooltip formatting
                                      try{
                                         serie.units = $.t(self.keywordInfo[index].units);
                                      }catch(error){
                                         serie.units = "";
                                      }
                                   }
                                   self.refreshingData = false;
                               })
                               .fail(function (error) {
                                  notifyError($.t("widgets/chart:errorDuringGettingDeviceData"), error);
                                  d.reject();
                               });
                           }
                       });
                       $.whenAll(arrayOfDeffered).done(function () {
                           self.finalRefresh();
                           d.resolve();
                       })
                      .fail(function (error) {
                         notifyError($.t("widgets/chart:errorDuringGettingDeviceData"), error);
                         d.reject();
                      });
                   }
               } catch (err) {
                   console.error(err.message);
                   notifyError(err.message);
                   self.refreshingData = false;
                   d.reject();
               }
           }
           return d.promise();
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

       this.addContinuousSummaryPoint = function () {
          var self = this;
          
         $.each(self.widget.configuration.devices, function (index, device) {
            var serie = self.chart.get(self.seriesUuid[index]);
            
             var time = moment(self.serverTime)._d.getTime().valueOf();
             var lastDate = 0;
             
             if (!isNullOrUndefined(serie)){
                if ((serie.points.length > 0) && ((time - lastDate) > (self.summaryTimeBetweenNewPoint)))
                {
                   lastDate = serie.points[serie.points.length - 1].x;
                   
                   switch (self.interval) {
                     case "DAY":
                        self.DisplaySummary(index, 1, device, "days", "hour", lastDate);
                        break;
                     case "WEEK":
                        self.DisplaySummary(index, 1, device, "weeks", "hour", lastDate);
                        break;
                     case "MONTH":
                        self.DisplaySummary(index, 1, device, "months", "day", lastDate);
                        break;
                     case "HALF_YEAR":
                        self.DisplaySummary(index, 6, device, "months", "day", lastDate);
                        break
                     case "YEAR":
                        self.DisplaySummary(index, 1, device, "years", "day", lastDate);
                        break;
                     default:
                        break;
                   }
                }
             }
         });
       };
       
       this.cleanUpChart = function (serie, finaldate, dateInMilliSecondes) {
          var self = this;
                      
            if (!isNullOrUndefined(serie.points)) {
                if (!isNullOrUndefined(serie.points[0])) {
                    if ((finaldate - serie.points[0].x) > dateInMilliSecondes)
                        self.changexAxisBound(self.calculateBeginDate(self.interval, self.serverTime, self.prefix));
                }
            }
       };
       
       this.onTime = function (serverLocalTime) {
         var self = this;
         self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime); // Update the serverTime
         
         var dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf(self.prefix).subtract(1, 'seconds'));
         var isofinaldate = DateTimeFormatter.isoDateToDate(dateTo)._d.getTime().valueOf();
            
         // If a serie is available
         self.addContinuousSummaryPoint();
         
         $.each(self.seriesUuid, function (index, value) {
         
            var serie = self.chart.get(value);
            var serieRange = self.chart.get('range_' + value);
            
            // If a serie is available  // Clean points > cleanValue for serie
            if (!isNullOrUndefined(serie))
               self.cleanUpChart(serie, isofinaldate, self.cleanValue);

             // Clean points > cleanValue for ranges, if any
            if (!isNullOrUndefined(serieRange))
               self.cleanUpChart(serieRange, isofinaldate, self.cleanValue);
         });         
       };
      
       this.DisplaySummary = function (index, nb, device, range, prefix, lastPointDate) {
           var self = this;

           try {
               //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
               //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
               
               var serie = self.chart.get(self.seriesUuid[index]);
               var serieRange = self.chart.get('range_' + self.seriesUuid[index]);
               
               var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(prefix).subtract(1, prefix + 's'));
               
               // we ask only from the last point registered
               var dateFrom = DateTimeFormatter.dateToIsoDate(moment(lastPointDate+1));
            
               RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + "/" + prefix + "/" + dateFrom + "/" + dateTo)
                  .done(function (data) {
                      try {                          
                          if (data.data[data.data.length-1] != undefined) {
                              self.chart.hideLoading(); // If a text was displayed before

                              var registerDate = DateTimeFormatter.isoDateToDate(data.data[data.data.length-1].date)._d.getTime().valueOf();
                              var valueToDisplay = parseFloat(data.data[data.data.length-1][self.periodValueType[index]]);
                       
                              if (self.differentialDisplay[index])
                              {
                                  if (serie && !isNullOrUndefined(self.chartLastValue[index]))
                                  {
                                     serie.addPoint([registerDate, valueToDisplay-self.chartLastValue[index]], 
                                                    true,  // redraw. When more than 1 => false.
                                                    false, // shift if true, one point at left is remove
                                                    true); // animation.
                                  }
                                  self.chartLastValue[index] = valueToDisplay;
                              }
                              else                              
                                 serie.addPoint([registerDate, valueToDisplay], true, false, true);
                        
                              //Add also for ranges if any
                              if (serieRange && !self.differentialDisplay[index])
                              {
                                 serieRange.addPoint([registerDate, parseFloat(data.data[data.data.length-1].min), parseFloat(data.data[data.data.length-1].max)], 
                                                     true, false, true);
                              }
                          }
						        else{ // Add null for this date
							        var registerDate = moment(self.serverTime).subtract(1, 'hours').startOf(prefix)._d.getTime().valueOf();
							        serie.addPoint([registerDate, null], true, false, true);
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
               if (self.seriesUuid.length == 0)
                   return;
                
               try {
                   $.each(self.widget.configuration.devices, function (index, device) {
                       if (keywordId === device.content.source.keywordId) {
                           var serie = self.chart.get(self.seriesUuid[index]);
                           // If a serie is available
                           if (!isNullOrUndefined(serie) && data.date!=="" & data.value!=="") {
                               // Add new point only for HOUR interval
							          // others points are treated into the onTime function
                               switch (self.interval) {
                                   case "HOUR":
                                       if (!isNullOrUndefined(serie)) {
                                           var time  = moment(self.serverTime)._d.getTime().valueOf();
                                           var isolastdate = DateTimeFormatter.isoDateToDate(data.date)._d.getTime().valueOf();
                                           if (time - isolastdate < 3600000){ // Only if the last value is in last hour
                                              self.chart.hideLoading(); // If a text was displayed before
                                              if (self.differentialDisplay[index]){
                                                 if (serie.points.length > 0 && !isNullOrUndefined(self.chartLastValue[index]))
                                                    serie.addPoint([data.date.valueOf(), parseFloat(data.value) - self.chartLastValue[index]], true, false, true);
                                                 self.chartLastValue[index] = parseFloat(data.value);                                                 
                                              }
                                              else
                                                 serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
                                           }
                                       }
                                       break;
                                   default:
                                       break;
                               }
                           }
                           else
                              console.log ("no data to be display for keyword ", data.keywordId);
                       }
                   });
               } catch (err) {
                   console.error(err);
               }
           }
       };
   };