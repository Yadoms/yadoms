widgetViewModelCtor =

/**
 * Create a Chart ViewModel
 * @constructor
 */
function chartViewModel() {
    this.seriesUuid = [];
    this.interval = 0;
    this.deviceInfo = [];
    
    // The unit of the plot
    this.unit = [];
    
    this.differentialDisplay = [];
    this.incompatibility = false;
    this.serverTime = null;
    this.precision = [];
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
    
    // This variable is only used for keyword of type enum. It returns the plugin instance type for this keyword
    this.pluginInstanceType = [];

    /**
     * Configure the toolbar
     */    
    this.configureToolbar = function(interval) {
       self=this;
       
       var menuItem = [
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HOUR\"><span data-i18n=\"widgets.chart:navigator.hour\"/></div>" },
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"DAY\"><span data-i18n=\"widgets.chart:navigator.day\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"WEEK\"><span data-i18n=\"widgets.chart:navigator.week\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"MONTH\"><span data-i18n=\"widgets.chart:navigator.month\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HALF_YEAR\"><span data-i18n=\"widgets.chart:navigator.half_year\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"YEAR\"><span data-i18n=\"widgets.chart:navigator.year\"/></div>" },
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"FIVE_YEAR\"><span data-i18n=\"widgets.chart:navigator.five_year\"/></div>" },
          { separator: ""}
          ];
       
       switch(interval)
       {
          case "HOUR/minute":
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"minute\"><span data-i18n=\"widgets.chart:navigator.all\"/></div>" });
             break;
          case "DAY/minute":
          case "DAY/hour":
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"minute\"><span data-i18n=\"widgets.chart:navigator.all\"/></div>" });
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"hour\"><span data-i18n=\"widgets.chart:navigator.hourly\"/></div>"});
             break;
          case "WEEK/hour":
          case "WEEK/day":
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"hour\"><span data-i18n=\"widgets.chart:navigator.hourly\"/></div>"});
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"day\"><span data-i18n=\"widgets.chart:navigator.daily\"/></div>"});
          break;
          case "MONTH/hour":
          case "MONTH/day":
          case "MONTH/week":
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"hour\"><span data-i18n=\"widgets.chart:navigator.hourly\"/></div>"});
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"day\"><span data-i18n=\"widgets.chart:navigator.daily\"/></div>"});
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"week\"><span data-i18n=\"widgets.chart:navigator.weekly\"/></div>"});
             break;
         case "HALF_YEAR/day":
         case "HALF_YEAR/week":
         case "HALF_YEAR/month":
         case "YEAR/day":
         case "YEAR/week":
         case "FIVE_YEAR/day":
         case "FIVE_YEAR/week":
         case "FIVE_YEAR/month":
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"day\"><span data-i18n=\"widgets.chart:navigator.daily\"/></div>"});
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"week\"><span data-i18n=\"widgets.chart:navigator.weekly\"/></div>"});
             menuItem.push({ custom: "<div class=\"widget-toolbar-button range-btn\" prefix=\"month\"><span data-i18n=\"widgets.chart:navigator.monthly\"/></div>"});         
             break;
          default:
          break;
       }
       
       self.prefix = interval.substring(interval.indexOf("/") +1, interval.length);
       menuItem.push({ separator: ""});
       
       //push last menu items
       menuItem.push(
          { custom: "<div class=\"widget-toolbar-button export-btn dropdown\">" +
              "<a id=\"chartExportMenu" + self.widget.id + "\" data-target=\"#\" class=\"widget-toolbar-button export-btn dropdown\" role=\"button\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\">" +
                  "<span class=\"fa fa-bars\"/>" +
              "</a>" +
              "<ul class=\"dropdown-menu\" aria-labelledby=\"chartExportMenu" + self.widget.id + "\">" +
                  "<li><span class=\"print-command\" data-i18n=\"widgets.chart:export.print\"></span></li>" +
                  "<li role=\"separator\" class=\"divider\"></li>" +
                  "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export.png\" mime-type=\"image/png\"></span></li>" +
                  "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export.jpeg\" mime-type=\"image/jpeg\"></span></li>" +
                  "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export.svg\" mime-type=\"image/svg+xml\"></span></li>" +
                  "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export.csv\" mime-type=\"text/csv\"></span></li>" +
                  "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export.xls\" mime-type=\"application/vnd.ms-excel\"></span></li>" +
              "</ul>" +
           "</div>"
           });
       
       self.widgetApi.toolbar({
          activated: true,
          displayTitle: true,
          batteryItem: false,
          items: menuItem
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
          catch(error){
             notifyError($.t("widgets.chart:formatNotSupported", {format: $(e.currentTarget).attr("mime-type")}));
          }
      });       
    };
    
    /**
     * Initialization method
     */
    this.initialize = function () {
        var self = this;
        var arrayOfDeffered = [];
        var d = new $.Deferred();
        
        self.$chart = self.widgetApi.find("div.container");
        
        //
        // For chart and gauge, compressed gz file will appears soon. At this time, there is some dependancies to handle.
        //
        
        arrayOfDeffered.push(self.widgetApi.loadLibrary([
            "libs/highstock/js/highstock.js",
            "libs/highstock/js/highcharts-more.js",
            "libs/highstock/js/modules/exporting.js",
            "libs/highstock/js/modules/offline-exporting.js",
            "libs/highstock/js/modules/canvas-tools.js",
            "libs/highstock/js/modules/boost.js",
            "libs/export-csv/js/export-csv.min.js",
            "libs/highcharts-export-clientside/js/highcharts-export-clientside.min.js"
        ]));        
        
        arrayOfDeffered.push(self.widgetApi.loadLibrary("widgets/chart/helpers.js"));
        arrayOfDeffered.push(self.widgetApi.askServerLocalTime(function (serverLocalTime) {
           self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
        }));
        
        $.when.apply($,arrayOfDeffered).done(function () {
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
                    adaptToUpdatedData: true,
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
                      rotation: -20,
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
                               case "FIVE_YEAR":
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
                                   if (this.series.chart.keyword[this.series.keywordTabId].type === "Enum") {
                                      s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.chart.keyword[this.series.index].typeInfo.translatedValues[parseInt(this.y)];
                                   }else {
                                    s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.name + " : " + this.y.toFixed(this.series.precision) + this.series.units;
                                   }
                                } else { //Range Serie
                                    s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.name + " : " + "<i style=\"color: " + this.series.color + ";\" class=\"fa fa-long-arrow-down\"></i>&nbsp;" +
                                        this.point.low.toFixed(this.series.precision) + this.series.units + " " +
                                        "<i style=\"color: " + this.series.color + ";\" class=\"fa fa-long-arrow-up\"></i>&nbsp;" +
                                        this.point.high.toFixed(this.series.precision) + this.series.units;
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
            self.chart.keyword = [];
            d.resolve();
        })
        .fail(function (error) {
            self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
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

    this.chartParametersConfiguration = function (interval, prefix) {
      var self = this;
      
      // Cleaning ranges switch
      switch (interval) {
          case "HOUR":
              self.cleanValue = 3600000;
              break;
          case "DAY":
              self.cleanValue = 3600000 * 24;
              break;
          case "WEEK":
              self.cleanValue = 3600000 * 24 * 7;
              break;
          case "MONTH":
              self.cleanValue = 3600000 * 24 * 30;
              break;
          case "HALF_YEAR":
              self.cleanValue = 3600000 * 24 * 182;
              break;
          case "YEAR":
              self.cleanValue = 3600000 * 24 * 365;
              break;
          case "FIVE_YEAR":
              self.cleanValue = 3600000 * 24 * 365 * 5;
              break;              
          default:
              self.cleanValue = 3600000*24;
              break;
      }
      
      if (prefix === "hour")
         self.summaryTimeBetweenNewPoint = 3600000 * 2 + 60000;
      else if (prefix === "day")
         self.summaryTimeBetweenNewPoint = 3600000 * 24 * 2 + 60000;
    };
    
    this.configurationChanged = function () {
        var self = this;
        var arrayOfDeffered = [];
        var keywords = [];
        var d = new $.Deferred();        
        // Reset of some values
        self.periodValueType = [];
        self.seriesUuid = [];

        if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;
        
        var intervalConfiguration = compatibilityManagement(self.widget.configuration.interval);
        self.configureToolbar(intervalConfiguration);
        self.interval = intervalConfiguration.substring(0,intervalConfiguration.indexOf("/"));
        
        //Activate the new button
        self.widgetApi.find(".range-btn[interval='" + self.interval + "']").addClass("widget-toolbar-pressed-button");
        self.widgetApi.find(".range-btn[prefix='" + self.prefix + "']").addClass("widget-toolbar-pressed-button");

        //just update some viewmodel info
        self.chartParametersConfiguration(self.interval, self.prefix);
         
        try{
          self.ConfigurationLegendLabels = self.widget.configuration.legends.content.legendLabels;
        }catch(error){ // If the configuration doesn't exist (migration, ...) -> default value
          console.log ("default value for legend labels : device name + keyword name");
        }
        
        //we create an uuid for each serie
        $.each(self.widget.configuration.devices, function (index, device) {
            //we update uuid if they don't exist
            if (isNullOrUndefined(self.seriesUuid[index]))
                self.seriesUuid[index] = createUUID();
            
            // reading the precision for the display of the value and the unit
            if (!isNullOrUndefined(device.content.advancedConfiguration.content.precision))
               self.precision[index] = parseInt(device.content.advancedConfiguration.content.precision, 10);
            else
               self.precision[index] = 1;
            
            var deffered = self.widgetApi.getDeviceInformation(device.content.source.deviceId);
            arrayOfDeffered.push(deffered);
            
            deffered
            .done(function (device) {
               self.deviceInfo[index] = device;
            })
            .fail(function (error) {
               self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
            });

            keywords.push(device.content.source.keywordId);
        });
        
        var defferedPluginInstance = new $.Deferred();
        arrayOfDeffered.push(defferedPluginInstance);
        
        var deffered2 = self.widgetApi.getKeywordsInformation(keywords, ["accessMode", "capacity", "friendlyName", "unit", "measure"]);
         arrayOfDeffered.push(deffered2);
         deffered2.done(function (keywordsInformation) {
            $.each(keywordsInformation, function (index, keyword) {
                self.chart.keyword[index] = keyword;
                //self.chart.keyword[index].typeInfo.translatedValues = [];

                function measureManagement(measure){
                  if (measure === "Cumulative"){
                     self.differentialDisplay[index] = true;
                     self.periodValueType[index] = "max";
                  }
                  else{ // Default values and Absolute value
                     self.differentialDisplay[index] = false;
                     self.periodValueType[index] = "avg";
                  }
                }
                
                try{
                   if (parseBool(self.widget.configuration.devices[index].content.advancedConfiguration.checkbox)){
                      self.differentialDisplay[index] = (self.widget.configuration.devices[index].content.advancedConfiguration.content.differentialDisplay ==="relative") ? true : false;
                      self.periodValueType[index] = self.widget.configuration.devices[index].content.advancedConfiguration.content.periodtype;

                   }else{ // automatic managment : the managment of the information is done from the measure type of the keyword
                      measureManagement(keyword.measure);
                  }
               }
               catch(error){
                  console.log('error detecting during defferential configuration display => automatic management.');
                  measureManagement(keyword.measure);
               }
               
               if (self.differentialDisplay[index] && device.content.PlotType === "arearange"){
                  notifyError($.t("widgets.chart:incompatibilityDifferential"), "error");
                  self.incompatibility = true;
                  return;
               }
               
                //we register the keyword for new acquisition if the device exist
                self.widgetApi.registerKeywordForNewAcquisitions(keyword.keywordId);
                
                //TODO : The deffered doesnt work with more than 1 Enum
               if (isEnumVariable(keyword)) {
                  self.widgetApi.getPluginInstanceInformation(keyword.pluginId)
                  .done(function (pluginInstance) {
                     self.pluginInstanceType[index] = pluginInstance.type;
                     // Translate enum values only for enum keyword
                     $.each(self.chart.keywordInfo[index].typeInfo.values, function (index2, value) {
                        self.chart.keyword[index].typeInfo.translatedValues[index2] = $.t("plugins." + self.pluginInstanceType[index] + ":enumerations." + self.chart.keyword[index].typeInfo.name + ".values." + value, { defaultValue:value} );
                     });                        
                     defferedPluginInstance.resolve();
                  })
                  .fail(defferedPluginInstance.reject);
               }else
                  defferedPluginInstance.resolve();
            });
         })
         .fail(function (error) {
            self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
         });
        
        $.when.apply($, arrayOfDeffered) // The first failing array fail the when.apply
        .done(function () {
           self.refreshData(self.interval)
            .always(d.resolve)
            .fail(d.reject)
        })
        .fail(d.reject);
        
        return d.promise();
    };

    this.navigatorBtnClick = function () {
        var self = this;
        return function (e) {
            function ConfigureAndRefreshChart(interval, prefix) {
               self.chartParametersConfiguration(interval, prefix);
               
               //we ask the new time server, and we refresh information
               self.widgetApi.askServerLocalTime(function (serverLocalTime) {
                  self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
                  self.refreshData(interval);
               });
            }           
           
            //we manage activation
            var interval = $(e.currentTarget).attr("interval");
            var prefix = $(e.currentTarget).attr("prefix");
            
            if (!isNullOrUndefined(interval)) {
               //default prefix for each interval
               self.prefix = defaultPrefixForInterval(interval);
               self.configureToolbar(interval + "/" + self.prefix);
               self.widgetApi.find(".range-btn[interval='" + interval + "']").addClass("widget-toolbar-pressed-button");
               self.widgetApi.find(".range-btn[prefix='" + self.prefix + "']").addClass("widget-toolbar-pressed-button");
               
               if (interval != self.interval){
                  ConfigureAndRefreshChart (interval, self.prefix);
                  self.interval = interval;
               }
            }

            if (!isNullOrUndefined(prefix) && (prefix != self.prefix)) {
               self.widgetApi.find(".range-btn[prefix='" + prefix + "']").addClass("widget-toolbar-pressed-button");
               self.widgetApi.find(".range-btn[prefix='" + self.prefix + "']").removeClass("widget-toolbar-pressed-button");
               ConfigureAndRefreshChart (self.interval, prefix);
               self.prefix = prefix;
            }
        };
    };

    this.refreshData = function (interval) {
       var self = this;
       var d = new $.Deferred();
       
       if (self.chart) {
         //we save interval in the chart
         self.chart.interval = interval;

         try {
           self.chart.showLoading($.t("widgets.chart:loadingData"));
           var deviceIsSummary = [];
           
           //ensure all series and axis are removed (may cause some crash if not done)
           while (self.chart.series.length > 0)
               self.chart.series[0].remove(false);

           while (self.chart.yAxis.length > 0)
               self.chart.yAxis[0].remove(false);

           var arrayOfDeffered = [];
           self.chartLastValue = [];
           
           //we compute the date from the configuration
           var dateFrom = calculateBeginDate(interval, self.serverTime, self.prefix);
           changexAxisBound(self.chart, dateFrom);
           var dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf(self.prefix).subtract(1, 'seconds'));
           var prefixUri = "/" + self.prefix;
           var timeBetweenTwoConsecutiveValues = moment.duration(1, self.prefix).asMilliseconds();              
           
           //for each plot in the configuration we request for data
           $.each(self.widget.configuration.devices, function (index, device) {
               //If the device is a bool, you have to modify
               if (isBoolVariable(self.chart.keyword[index]) || isEnumVariable(self.chart.keyword[index])) {
                   dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
                   prefixUri = "";                      
                   deviceIsSummary[index] = false; // We change the summary for the boolean device.
               } else {
                  // if self.prefix = "minute" then whe want all data
                  if (self.prefix == "minute"){
                   dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime)); // rewriting the final time
                   //we request all data
                   timeBetweenTwoConsecutiveValues = undefined;
                   deviceIsSummary[index] = false;
                   prefixUri = ""; // rewrite the prefix                          
                 }else if (self.prefix === "week") {
                    // the prefix week doesn't exist at the server side we have to to it manually
                    // we use the day prefix
                    prefixUri = "/day";
                    deviceIsSummary[index] = true;
                 }else{
                    deviceIsSummary[index] = true; // By default, it's the summary define above.
                 }
               }

                var deffered = RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
                arrayOfDeffered.push(deffered);
                deffered.done(function (data) {
                    //we make the serie
                    var plot = [];
                    var range = [];
                    var lastDate;
                    var d;
                    var dataVector = data.data;

                    if (!(deviceIsSummary[index])) {
                        //data comes from acquisition table
                        $.each(data.data, function (index2,value) {
                            lastDate = d;
                            d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();

                            var v;
                            if (!isNullOrUndefined(value.key)) {
                               if (isEnumVariable(self.chart.keyword[index])) {
                                  v= self.chart.keyword[index].typeInfo.values.indexOf(value.key);
                               }else {
                                  v = parseFloat(value.key);
                               }
                            } else {
                               self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
                               notifyError($.t("widgets.chart:errorInitialization"));
                            }
                            
                            // The differential display is disabled if the type of the data is enum or boolean
                            if (self.differentialDisplay[index] && !isBoolVariable(self.chart.keyword[index]) && !isEnumVariable(self.chart.keyword[index])){
                               if (!isNullOrUndefined(self.chartLastValue[index]))
                                  plot.push([d, v-self.chartLastValue[index]]);

                               self.chartLastValue[index] = v;
                            }
                            else // standard display
                               plot.push([d, v]);
                        });
                    } else {
                        var vMin;
                        var vMax;
                        
                        //
                        // in case of week, we have to change manually the array
                        //
                        if (self.prefix === "week") {
                           dataVector = getWeeks(data.data);
                        }else{
                           dataVector = data.data;
                        }

                        $.each(dataVector, function (index2, value) {
                            lastDate = d;
                            d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
                            var vplot;
                    
                            if (!isNullOrUndefined(value[self.periodValueType[index]])) {
                                // read the computed desired value (avg/min/max)
                                vplot = parseFloat(value[self.periodValueType[index]]);
                                vMin = parseFloat(value.min);
                                vMax = parseFloat(value.max);
                            } else {
                               self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
                               notifyError($.t("widgets.chart:errorInitialization"));
                            }

                            //we manage the missing data
                            if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                            (lastDate + timeBetweenTwoConsecutiveValues < d)) {

                                if (device.content.PlotType === "arearange")
                                    range.push([d, null, null]);

                                plot.push([d, null]);
                            }

                            // The differential display is disabled if the type of the data is enum or boolean
                            if (self.differentialDisplay[index] && !isBoolVariable(self.chart.keyword[index]) && !isEnumVariable(self.chart.keyword[index])){  
                       if (self.periodValueType[index] =="avg") {
                         if (!isNullOrUndefined(self.chartLastValue[index]))
                           plot.push([d, vplot-self.chartLastValue[index]]);
                         
                         self.chartLastValue[index] = vplot;
                      }
                      else if (self.periodValueType[index] =="max") { // In this case, we display vMax-vMin
                         plot.push([d, vMax-vMin]);
                      }
                            }
                            else{
                               if (device.content.PlotType === "arearange")
                                   range.push([d, vMin, vMax]);

                               plot.push([d, vplot]);                                                   
                            }
                        });
                        
                        // Add here missing last data at the end
                        if (!isNullOrUndefinedOrEmpty(dataVector)){
                           d = DateTimeFormatter.isoDateToDate(dataVector[dataVector.length-1].date)._d.getTime();
                           var time = moment(self.serverTime).startOf(self.prefix)._d.getTime().valueOf();
                           var registerDate = moment(self.serverTime).startOf(self.prefix).subtract(1, self.prefix + 's')._d.getTime().valueOf();
                           
                           if ((time - d) > self.summaryTimeBetweenNewPoint){
                               if (device.content.PlotType === "arearange")
                                    range.push([registerDate, null, null]);

                               plot.push([registerDate, null]);                                             
                           }
                        }
                    }
                    
                    // Adapt units if needed
                    adaptValuesAndUnit(plot, range, self.chart.keyword[index].unit, function(newValues, newRange, newUnit){
                       plot = newValues;
                       range = newRange;
                       self.unit[index] = newUnit;
                    });
                    
                    var axisName;
                    try{
                       axisName = createAxis(index, self.chart, self.seriesUuid, self.widget.configuration, self.precision[index], self.unit[index], device);
                    }catch(error){
                       console.error (error);
                    }
                    
                    var legendText="";
                    try{
                       // series names
                       if (self.ConfigurationLegendLabels ==="Device")
                          legendText = self.deviceInfo[index].friendlyName;
                       else if (self.ConfigurationLegendLabels ==="Keyword")
                          legendText = self.chart.keyword[index].friendlyName;                                       
                       else
                          legendText = self.deviceInfo[index].friendlyName + "/" + self.chart.keyword[index].friendlyName;
                    }catch(error){
                       self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
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
                           connectNulls: isBoolVariable(self.chart.keyword[index]), // TODO : false si self.prefix === "minute"
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
                            serieOption.step = isBoolVariable(self.chart.keyword[index]);  // For boolean values, create steps.
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

                                // Add Units and precision for ranges
                                if (serieRange){
                                   try{
                                      serieRange.units = $.t(self.chart.keyword[index].unit);
                                   }
                                   catch(error){
                                      serieRange.units="";
                                   }
                                   serieRange.precision = self.precision[index];
                                   serieRange.keywordTabId = index;
                                }
                            }
                        }
                    } catch (err2){
                        console.error('Fail to create serie : ' + err2);
                    }

                    if (serie){
                       //we save the unit in the serie for tooltip formatting
                       try{
                          serie.units = $.t(self.unit);
                       }catch(error){
                          serie.units = "";
                       }
                       
                       // register the precision for each serie into the serie
                       serie.precision = self.precision[index];
                       serie.keywordTabId = index;
                    }
                })
                .fail(function (error) {
                   notifyError($.t("widgets.chart:errorDuringGettingDeviceData"), error);
                   d.reject();
                });
           });
           $.when.apply($, arrayOfDeffered).done(function () {
              self.finalRefresh();
              d.resolve();
           })
          .fail(function (error) {
             notifyError($.t("widgets.chart:errorDuringGettingDeviceData"), error);
             d.reject();
          });
         } catch (err) {
            console.error(err.message);
            notifyError(err.message);
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
          self.chart.showLoading($.t("widgets.chart:noAvailableData"));
       }else if (self.incompatibility) {
          self.chart.showLoading($.t("widgets.chart:incompatibilityDifferential"));
       }else {
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
             if ((serie.points.length > 0) && ((time - lastDate) > (self.summaryTimeBetweenNewPoint))){
                lastDate = serie.points[serie.points.length - 1].x;
                
                //debugger;
                
                switch (self.interval) {
                  case "DAY":
                     self.DisplaySummary(index, 1, device, "days", self.prefix, lastDate);
                     break;
                  case "WEEK":
                     self.DisplaySummary(index, 1, device, "weeks", self.prefix, lastDate);
                     break;
                  case "MONTH":
                     self.DisplaySummary(index, 1, device, "months", self.prefix, lastDate);
                     break;
                  case "HALF_YEAR":
                     self.DisplaySummary(index, 6, device, "months", self.prefix, lastDate);
                     break
                  case "YEAR":
                     self.DisplaySummary(index, 1, device, "years", self.prefix, lastDate);
                     break;
                  case "FIVE_YEAR":
                     self.DisplaySummary(index, 5, device, "years", self.prefix, lastDate);
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
                changexAxisBound(self.chart, calculateBeginDate(self.interval, self.serverTime, self.prefix));
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
            var request_prefix = "";
            
            // we ask only from the last point registered
            var dateFrom = DateTimeFormatter.dateToIsoDate(moment(lastPointDate+1));
            
            //the week is not well calculated
            if (prefix === "week") 
               request_prefix = "day";
            else
               request_prefix = prefix;
         
            RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + "/" + request_prefix + "/" + dateFrom + "/" + dateTo)
            .done(function (data) {
                try {
                   //debugger;
                   
                   // Weeks is not returned by the server, so we calculate here the weeks vector
                   if (prefix === "week")
                      vectorToAnalyze = getWeeks(data.data);
                   else 
                      vectorToAnalyze = data.data;
                   
                    if (!isNullOrUndefined(vectorToAnalyze) && !isNullOrUndefinedOrEmpty(vectorToAnalyze[vectorToAnalyze.length-1])) {
                       var registerDate = DateTimeFormatter.isoDateToDate(vectorToAnalyze[vectorToAnalyze.length-1].date)._d.getTime().valueOf();
                       if (registerDate != serie.points[serie.points.length-1].x){
                          self.chart.hideLoading(); // If a text was displayed before
                          var valueToDisplay = parseFloat(vectorToAnalyze[vectorToAnalyze.length-1][self.periodValueType[index]]);
                    
                          if (self.differentialDisplay[index]){
                             if (serie && !isNullOrUndefined(self.chartLastValue[index])){
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
                          if (serieRange && !self.differentialDisplay[index]){
                             serieRange.addPoint([registerDate, 
                                                  parseFloat(vectorToAnalyze[vectorToAnalyze.length-1].min), 
                                                  parseFloat(vectorToAnalyze[vectorToAnalyze.length-1].max)], 
                                                 true, false, true);
                          }
                       }
                    }
                    else{ // Add null for this date
                       var registerDate = moment(self.serverTime).subtract(1, 'hours').startOf(prefix)._d.getTime().valueOf();
                       
                       if ((registerDate - serie.points[serie.points.length-1].x) > self.cleanValue)
                          serie.addPoint([registerDate, null], true, false, true);
                       
                       if (serieRange && !self.differentialDisplay[index])
                       {
                          if ((registerDate - serieRange.points[serieRange.points.length-1].x) > self.cleanValue)
                             serieRange.addPoint([registerDate, null, null], true, false, true);
                       }
                    }
                } catch (err) {
                    console.error(err.message);
                }
            })
            .fail(function (error) {
               console.error(error);
            });
        } catch (err) {
            console.error(err.message);
        }
    };
    
    /**
    * event keyword deleted handler
    * @param keywordId keyword Id removed from Yadoms
    */    
    this.onKeywordDeletion = function (keywordId) {
       var self = this;
       
       if (self.chart){
          $.each(self.chart.keyword, function (index, keyword) {
             if (keywordId.id == keyword.id){ // we found the keyword associated, index to the corresponding series
                var serie = self.chart.get(self.seriesUuid[index]);
                var serieRange = self.chart.get('range_' + self.seriesUuid[index]);             
                
                // Remove corresponding series to the keyword
                if (serie) serie.remove();
                if (serieRange) serieRange.remove();
             }
          });
       }
    };
    
    /**
    * event wakeUp (after a reconnexion, wake down of a smartphone, tab, ...)
    */    
    this.onWakeUp = function () {
       var self = this;
       
       self.widgetApi.askServerLocalTime(function (serverLocalTime) {
          self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
       }).done(function(data) {
          self.refreshData(self.interval);
       })
       .fail(function(error) {
       });
    };

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
         try {
             $.each(self.widget.configuration.devices, function (index, device) {
                 if (keywordId === device.content.source.keywordId && self.chart) {
                     var serie = self.chart.get(self.seriesUuid[index]);
                     // If a serie is available
                     if (!isNullOrUndefined(serie) && data.date!=="" & data.value!=="") {
                         // Add new point only for HOUR interval
                         // others points are treated into the onTime function
                         if  (self.interval === "HOUR") {
                            var time  = moment(self.serverTime)._d.getTime().valueOf();
                            var isolastdate = DateTimeFormatter.isoDateToDate(data.date)._d.getTime().valueOf();
                            if (time - isolastdate < 3600000){ // Only if the last value is in last hour
                               self.chart.hideLoading(); // If a text was displayed before
                               if (self.differentialDisplay[index]){
                                  if (serie.points.length > 0 && !isNullOrUndefined(self.chartLastValue[index]))
                                     serie.addPoint([data.date.valueOf(), parseFloat(data.value) - self.chartLastValue[index]], true, false, true);
                                  self.chartLastValue[index] = parseFloat(data.value);                                                 
                               }else if (isEnumVariable(index)){
                                  var value = self.chart.keyword[index].typeInfo.values.indexOf(data.value);
                                  serie.addPoint([data.date.valueOf(), value], true, false, true);
                               }else
                                  serie.addPoint([data.date.valueOf(), parseFloat(data.value)], true, false, true);
                            }
                         }
                     }
                     else
                        console.log ("no data to be display for keyword ", data.keywordId);
                 }
             });
         } catch (err) {
             console.error(err);
         }
    };
};