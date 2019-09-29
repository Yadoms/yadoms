widgetViewModelCtor =

/**
 * Create a Chart ViewModel
 * @constructor
 */
function chartViewModel() {
    this.seriesUuid = [];
    this.interval = 0;
    this.deviceInfo = [];
    this.keywordPosition = [];
    this.unit = [];                      // The unit of the plot
    this.differentialDisplay = [];
    this.incompatibility = false;
    this.serverTime = null;
    this.precision = [];
    this.ConfigurationLegendLabels = "";
    this.timeDeffered = null;
    this.cleanValue = 0;                  // defined by the configuration of the widget chart for cleaning old points
    this.prefix = "";
    this.summaryTimeBetweenNewPoint = 0;  // defined by the configuration of the widget chart for adding new summary points for continuous display
    this.chartLastXPosition = [];         // This variable is used to know the last x position, and used to add new values in continuous display
    this.chartLastValue = [];             // This variable is used in differential display
    this.periodValueType = [];            // This variable is for the selection of the computed value used for the chart (min, avg, max)
    this.pluginInstanceType = [];         // This variable is only used for keyword of type enum. It returns the plugin instance type for this keyword
    this.coeff = [];                      // Adaptation coefficient, if units have been adapted
    this.automaticScale = true;           // Automatic unit scale
	this.displayDefinition = [];

    /**
     * Configure the toolbar
     */    
    this.configureToolbar = function(interval) {
       self=this;
       
       var menuItem = [
          { custom: "<div class=\"widget-toolbar-button before\"><span class=\"glyphicon glyphicon-chevron-left\"></div>" },
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HOUR\"><span data-i18n=\"widgets.chart:navigator.hour\"/></div>" },
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"DAY\"><span data-i18n=\"widgets.chart:navigator.day\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"WEEK\"><span data-i18n=\"widgets.chart:navigator.week\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"MONTH\"><span data-i18n=\"widgets.chart:navigator.month\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"HALF_YEAR\"><span data-i18n=\"widgets.chart:navigator.half_year\"/></div>"},
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"YEAR\"><span data-i18n=\"widgets.chart:navigator.year\"/></div>" },
          { custom: "<div class=\"widget-toolbar-button range-btn\" interval=\"FIVE_YEAR\"><span data-i18n=\"widgets.chart:navigator.five_year\"/></div>" },
          { custom: "<div class=\"widget-toolbar-button after\"><span class=\"glyphicon glyphicon-chevron-right\"></div>" },
          { separator: ""}
          ];
       
       switch(interval){
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
      $btns.off("click").on("click", self.navigatorBtnClick());
      self.widgetApi.widget.$toolbar.find(".print-command").off("click").on("click", self.printBtnClick());
      self.widgetApi.widget.$toolbar.find(".export-command").off("click").on("click", self.exportBtnClick());
      
      self.widgetApi.widget.$toolbar.find(".before").off("click").on("click", self.displayBefore());
      self.widgetApi.widget.$toolbar.find(".after").off("click").on("click", self.displayAfter());      
    };
    
    this.displayBefore = function () {
    };
    
    this.displayAfter = function () {
    };    
    
    this.exportBtnClick = function () {
        var self = this;
        return function (e) {
          try{
             self.chart.exportChartLocal({
                 type: $(e.currentTarget).attr("mime-type"),
                 filename: 'export'
             });
          }
          catch(error){
             notifyError($.t("widgets.chart:formatNotSupported", {format: $(e.currentTarget).attr("mime-type")}));
          }              
        };
     };
     
     this.printBtnClick = function () {
        var self = this;
        return function (e) {
           self.chart.print();
           window.location.reload(); //force page reload because after having displayed printing page, the widget disposition is erroneous
        };
     };     
    
    /**
     * Initialization method
     */
    this.initialize = function () {
        var self = this;
        var arrayOfDeffered = [];
        var d = new $.Deferred();
		self.displayDefinition["HOUR"] = {nb:1, type:'hours'};
		self.displayDefinition["DAY"] = {nb:1, type:'days'};
		self.displayDefinition["WEEK"] = {nb:1, type:'weeks'};
		self.displayDefinition["MONTH"] = {nb:1, type:'months'};
		self.displayDefinition["HALF_YEAR"] = {nb:6, type:'months'};
		self.displayDefinition["YEAR"] = {nb:1, type:'years'};
		self.displayDefinition["FIVE_YEAR"] = {nb:5, type:'years'};
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
                    marginTop: 10,
                    zoomType: "xy",
                    panning: true,
                    panKey: 'shift'
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
                    },
                    series: {
                       events: {
                          hide: function () {
                             this.chart.keyword[this.keywordTabId].visible = this.visible;
                          },
                          show: function () {
                             this.chart.keyword[this.keywordTabId].visible = this.visible;
                          }                          
                       }
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
                                   if (this.series.chart.keyword[this.series.keywordTabId].dataType === "Enum") {
                                      s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.chart.keyword[this.series.keywordTabId].typeInfo.translatedValues[parseInt(this.y)];
                                   }else {
                                    s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.name + " : " + roundNumber(this.y, this.series.precision) + " " + this.series.units;
                                   }
                                } else { //Range Serie
                                    s += "<br/><i style=\"color: " + this.series.color + ";\" class=\"fa fa-circle\"></i>&nbsp;" +
                                        this.series.name + " : " + "<i style=\"color: " + this.series.color + ";\" class=\"fa fa-long-arrow-down\"></i>&nbsp;" +
                                        roundNumber(this.point.low,this.series.precision) + " " + this.series.units + " " +
                                        "<i style=\"color: " + this.series.color + ";\" class=\"fa fa-long-arrow-up\"></i>&nbsp;" +
                                        roundNumber(this.point.high,this.series.precision) + " " + this.series.units;
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
            Highcharts.setOptions({
              global: {
                useUTC: false
              }
            });
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
	  var duration = moment.duration(self.displayDefinition[interval].nb, self.displayDefinition[interval].type);
      self.cleanValue = duration.asMilliseconds();
      
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
        
        if (!isNullOrUndefined(self.widget.configuration.automaticScale)){
           self.automaticScale = parseBool(self.widget.configuration.automaticScale);
        }
        
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
            var keywordId = device.content.source.keywordId;
            //Register the keyword position into the tab
            self.keywordPosition[keywordId] = index;
            
            //we update uuid if they don't exist
            if (isNullOrUndefined(self.seriesUuid[keywordId]))
                self.seriesUuid[keywordId] = createUUID();
            
            // reading the precision for the display of the value and the unit
            if (!isNullOrUndefined(device.content.advancedConfiguration.content.precision))
               self.precision[keywordId] = parseInt(device.content.advancedConfiguration.content.precision, 10);
            else
               self.precision[keywordId] = 1;
            
            var deffered = self.widgetApi.getDeviceInformation(device.content.source.deviceId);
            arrayOfDeffered.push(deffered);
            
            deffered
            .done(function (device) {
               self.deviceInfo[keywordId] = device;
            })
            .fail(function (error) {
               self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
            });

            keywords.push(keywordId);
        });
        
        var defferedPluginInstance = new $.Deferred();
        arrayOfDeffered.push(defferedPluginInstance);
        
        var deffered2 = self.widgetApi.getKeywordsInformation(keywords, ["accessMode", "capacity", "friendlyName", "unit", "measure", "dataType", "pluginId", "typeInfo"]);
         arrayOfDeffered.push(deffered2);
         deffered2.done(function (keywordsInformation) {
            $.each(keywordsInformation, function (index, keyword) {
                self.chart.keyword[keyword.keywordId] = keyword;
                self.chart.keyword[keyword.keywordId].visible = true;

                function measureManagement(measure){
                  if (measure === "Cumulative"){
                     self.differentialDisplay[keyword.keywordId] = true;
                     self.periodValueType[keyword.keywordId] = "max";
                  }
                  else{ // Default values and Absolute value
                     self.differentialDisplay[keyword.keywordId] = false;
                     self.periodValueType[keyword.keywordId] = "avg";
                  }
                }
                
                try{
                   if (parseBool(self.widget.configuration.devices[self.keywordPosition[keyword.keywordId]].content.advancedConfiguration.checkbox)){
                      self.differentialDisplay[keyword.keywordId] = (self.widget.configuration.devices[self.keywordPosition[keyword.keywordId]].content.advancedConfiguration.content.differentialDisplay ==="relative") ? true : false;
                      self.periodValueType[keyword.keywordId] = self.widget.configuration.devices[self.keywordPosition[keyword.keywordId]].content.advancedConfiguration.content.periodtype;

                   }else{ // automatic managment : the managment of the information is done from the measure type of the keyword
                      measureManagement(keyword.measure);
                  }
               }
               catch(error){
                  console.log('error detecting during differential configuration display => automatic management');
                  measureManagement(keyword.measure);
               }
               
               if (self.differentialDisplay[keyword.keywordId] && self.widget.configuration.devices[self.keywordPosition[keyword.keywordId]].content.PlotType === "arearange"){
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
                     self.pluginInstanceType[keyword.keywordId] = pluginInstance.type;
                     self.chart.keyword[keyword.keywordId].typeInfo.translatedValues = [];
                     // Translate enum values only for enum keyword
                     $.each(self.chart.keyword[keyword.keywordId].typeInfo.values, function (index2, value) {
                        self.chart.keyword[keyword.keywordId].typeInfo.translatedValues[index2] = $.t("plugins." + self.pluginInstanceType[keyword.keywordId] + ":enumerations." + self.chart.keyword[keyword.keywordId].typeInfo.name + ".values." + value, { defaultValue:value} );
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
           if (self.chart) { self.chart.interval = self.interval;}//we save interval in the chart
           self.refreshData(calculateBeginDate(self.interval, self.serverTime, self.prefix))
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
               
               self.widgetApi.askServerLocalTime(function (serverLocalTime) { //we ask the new time server, and we refresh information
                  self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
                  if (self.chart) { self.chart.interval = interval;}//we save interval in the chart
                  self.refreshData(calculateBeginDate(interval, self.serverTime, self.prefix));
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

    this.refreshData = function (dateFrom) {
       var self = this;
       var d = new $.Deferred();
       var plot = [];
       var range = [];    
       
       if (!self.chart) 
          return d.promise().reject(); // Liberate immediately the deffered
       
      try {
        self.chart.showLoading($.t("widgets.chart:loadingData"));
        var deviceIsSummary = [];
        
        //ensure all series and axis are removed (may cause some crash if not done)
        while (self.chart.series.length > 0)
            self.chart.series[0].remove(false);

        while (self.chart.yAxis.length > 0)
           self.chart.yAxis[0].remove(false);

        var arrayOfDeffered = [];
        var arrayOfDeffered1 = [];
        var arrayOfDeffered2 = [];
        self.chartLastValue = [];
        
        //we compute the date from the configuration
        changexAxisBound(self.chart, dateFrom);
        var dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf(self.prefix).subtract(1, 'seconds'));
        var timeBetweenTwoConsecutiveValues = moment.duration(1, self.prefix).asMilliseconds();              
        
        //for each plot in the configuration we request for data
        $.each(self.widget.configuration.devices, function (index, device) {
            var prefixUri = "/" + self.prefix;
            // Index to find all information about each keyword
            var keywordId = device.content.source.keywordId;
            
            // Initialize work tables
            plot[keywordId] = [];
            range[keywordId] = [];
            self.coeff[keywordId] = 1; // Default adaptation coeff for unit
            
            //If the device is a bool or a enum, you have to modify
            if (isBoolVariable(self.chart.keyword[keywordId]) || isEnumVariable(self.chart.keyword[keywordId])) {
                dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
                prefixUri = "";                      
                deviceIsSummary[keywordId] = false; // We change the summary for the boolean device.
            } else {
               // if self.prefix = "minute" then whe want all data
               if (self.prefix == "minute"){
                  dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime)); // rewriting the final time
                  //we request all data
                  timeBetweenTwoConsecutiveValues = undefined;
                  deviceIsSummary[keywordId] = false;
                  prefixUri = ""; // rewrite the prefix                          
              }else if (self.prefix === "week") {
                  // the prefix week doesn't exist at the server side we have to to it manually
                  // we use the day prefix
                  prefixUri = "/day";
                  deviceIsSummary[keywordId] = true;
              }else{
                  deviceIsSummary[keywordId] = true; // By default, it's the summary define above.
              }
            }

             var deffered = RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
             arrayOfDeffered.push(deffered);
             arrayOfDeffered1[keywordId] = new $.Deferred();
             arrayOfDeffered2[keywordId] = new $.Deferred();
             deffered.done(function (data) {
                 var lastDate;
                 var d;
                 var dataVector = data.data;

                 if (!(deviceIsSummary[keywordId])) {
                     //data comes from acquisition table
                     $.each(data.data, function (index2,value) {
                         lastDate = d;
                         d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();

                         var v;
                         if (!isNullOrUndefined(value.key)) {
                            if (isEnumVariable(self.chart.keyword[keywordId])) {
                       v = getKeyByValue(self.chart.keyword[keywordId].typeInfo.values, value.key);
                            }else {
                               v = parseFloat(value.key);
                            }
                         } else {
                            self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
                            notifyError($.t("widgets.chart:errorInitialization"));
                         }
                         
                         // The differential display is disabled if the type of the data is enum or boolean
                         if (self.differentialDisplay[keywordId] && !isBoolVariable(self.chart.keyword[keywordId]) && !isEnumVariable(self.chart.keyword[keywordId])){
                            if (!isNullOrUndefined(self.chartLastValue[keywordId]))
                               plot[keywordId].push([d, v-self.chartLastValue[keywordId]]);

                            self.chartLastValue[keywordId] = v;
                         }
                         else // standard display
                            plot[keywordId].push([d, v]);
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

                         if (!isNullOrUndefined(value[self.periodValueType[keywordId]])) {
                             // read the computed desired value (avg/min/max)
                             vplot = parseFloat(value[self.periodValueType[keywordId]]);
                             vMin = parseFloat(value.min);
                             vMax = parseFloat(value.max);
                         } else {
                            self.widgetApi.setState (widgetStateEnum.InvalidConfiguration);
                            notifyError($.t("widgets.chart:errorInitialization"));
                            throw $.t("widgets.chart:errorInitialization");
                         }

                         //we manage the missing data
                         if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
                         (lastDate + timeBetweenTwoConsecutiveValues < d)) {

                             if (device.content.PlotType === "arearange")
                                 range[keywordId].push([d, null, null]);

                             plot[keywordId].push([d, null]);
                         }
                         
                         // The differential display is disabled if the type of the data is enum or boolean
                         if (self.differentialDisplay[keywordId] && !isBoolVariable(self.chart.keyword[keywordId]) && !isEnumVariable(self.chart.keyword[keywordId])){  
                             if (self.periodValueType[keywordId] =="avg") {
                               if (!isNullOrUndefined(self.chartLastValue[keywordId]))
                                 plot[keywordId].push([d, vplot-self.chartLastValue[keywordId]]);
                               
                               self.chartLastValue[keywordId] = vplot;
                            }
                            else if (self.periodValueType[keywordId] =="max") { // In this case, we display vMax-vMin
                               plot[keywordId].push([d, vMax-vMin]);
                               self.chartLastValue[keywordId] = vMax;
                            }
                         }
                         else{
                            if (device.content.PlotType === "arearange")
                                range[keywordId].push([d, vMin, vMax]);

                            plot[keywordId].push([d, vplot]);                                                   
                         }
                     });
                     
                     // Add here missing last data at the end
                     if (!isNullOrUndefinedOrEmpty(dataVector)){
                        d = DateTimeFormatter.isoDateToDate(dataVector[dataVector.length-1].date)._d.getTime();
                        var time = moment(self.serverTime).startOf(self.prefix)._d.getTime().valueOf();
                        var registerDate = moment(self.serverTime).startOf(self.prefix).subtract(1, self.prefix + 's')._d.getTime().valueOf();
                        
                        if ((time - d) > self.summaryTimeBetweenNewPoint){
                            if (device.content.PlotType === "arearange")
                                 range[keywordId].push([registerDate, null, null]);

                            plot[keywordId].push([registerDate, null]);                                             
                        }
                     }
                 }
             })
             .fail(function (error) {
                notifyError($.t("widgets.chart:errorDuringGettingDeviceData"), error);
                d.reject();
             });
        });

        var newPlots = [];
        var newRanges = [];
        var newUnits = [];
        
        // Unit traitments
        $.when.apply($, arrayOfDeffered).done(function () {
           // automatic unit
           if (self.automaticScale){
              $.each(self.widget.configuration.devices, function (index, device) {
                 var keywordId = device.content.source.keywordId;
                 
                 // Adapt units if needed
                 adaptValuesAndUnit(plot[keywordId], range[keywordId], self.chart.keyword[keywordId].unit, function(newValues, newRange, newUnit, newcoeff){
                    newPlots[keywordId] = newValues;
                    newRanges[keywordId] = newRange;
                    newUnits[keywordId] = newUnit;
                    self.coeff[keywordId] = newcoeff;
                    arrayOfDeffered1[keywordId].resolve();
                 });
              });
           }else{
              $.each(self.widget.configuration.devices, function (index, device) {
                 var keywordId = device.content.source.keywordId;
                 self.unit[keywordId] = self.chart.keyword[keywordId].unit;
                 arrayOfDeffered1[keywordId].resolve();
              });
           }
        });
        
        // Display each curves
        $.when.apply($, arrayOfDeffered1).done(function () {
           // automatic unit
           if (self.automaticScale){
              if (parseBool(self.widget.configuration.oneAxis.checkbox)) { // We the same axis we adapt all identical units the the same correct unit
                 // Analysis of all adaptations
                 $.each(self.widget.configuration.devices, function (index, device1) {
                    var keywordId1 = device1.content.source.keywordId;
                    
                    $.each(self.widget.configuration.devices, function (index, device2) {
                       var keywordId2 = device2.content.source.keywordId;
                       
                       if (self.chart.keyword[keywordId1].unit === self.chart.keyword[keywordId2].unit){
                          // If applicable adaptation are different
                          if (self.coeff[keywordId1] != self.coeff[keywordId2]){
                             //We keep the lowest one
                             if (self.coeff[keywordId1] < self.coeff[keywordId2]){
                                // Adapt the second one
                                newPlots[keywordId2] = adaptArray(plot[keywordId2], self.coeff[keywordId1]);
                                newRanges[keywordId2] = adaptRange(range[keywordId2], self.coeff[keywordId1]);
                                newUnits[keywordId2] = newUnits[keywordId1];
                                self.coeff[keywordId2] = self.coeff[keywordId1];
                             }else{
                                // Adapt the first one
                                newPlots[keywordId1] = adaptArray(plot[keywordId1], self.coeff[keywordId2]);
                                newRanges[keywordId1] = adaptRange(range[keywordId1], self.coeff[keywordId2]);
                                newUnits[keywordId1] = newUnits[keywordId2];
                                self.coeff[keywordId1] = self.coeff[keywordId2];                                
                             }
                          }
                       }
                    });
                    
                    plot[keywordId1] = newPlots[keywordId1];
                    range[keywordId1] = newRanges[keywordId1];
                    self.unit[keywordId1] = newUnits[keywordId1];
                 });
              }else{
                 $.each(self.widget.configuration.devices, function (index, device) {
                    var keywordId = device.content.source.keywordId;
                    
                    plot[keywordId] = newPlots[keywordId];
                    range[keywordId] = newRanges[keywordId];
                    self.unit[keywordId] = newUnits[keywordId];
                 });
              }
           }
           
           // Creates all axis and curves
           $.each(self.widget.configuration.devices, function (index, device) {
              var keywordId = device.content.source.keywordId;
              
              var axisName = createAxis(keywordId, self.chart, self.seriesUuid, self.widget.configuration, self.precision[keywordId], self.unit[keywordId], device);
              var legendText="";
              
              try{
                 // series names
                 if (self.ConfigurationLegendLabels ==="Device")
                    legendText = self.deviceInfo[keywordId].friendlyName;
                 else if (self.ConfigurationLegendLabels ==="Keyword")
                    legendText = self.chart.keyword[keywordId].friendlyName;                                       
                 else
                    legendText = self.deviceInfo[keywordId].friendlyName + "/" + self.chart.keyword[keywordId].friendlyName;
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
              
              if (plot[keywordId].length != 0)
                 self.chartLastXPosition[keywordId] = plot[keywordId][plot[keywordId].length-1][0];
              
              try {
                 // Standard options
                 var serieOption = {
                     id: self.seriesUuid[keywordId],
                     data: plot[keywordId],
                     dataGrouping: {
                        enabled: false
                     },
                     name: legendText,
                     connectNulls: isBoolVariable(self.chart.keyword[keywordId]), // TODO : false si self.prefix === "minute"
                     marker: {
                        enabled: null,
                        radius: 2,
                        symbol: "circle"
                     },
                     color: device.content.color,
                     yAxis: axisName,
                     lineWidth: 2,
                     showInLegend: legendConfiguration,
                     animation: false,
                     visible: self.chart.keyword[keywordId].visible
                  };
                 
                  if (device.content.PlotType === "arearange") { // arearange
                      serieOption.type = 'line';
                  }else {                                             // default option
                      serieOption.step = isBoolVariable(self.chart.keyword[keywordId]);  // For boolean values, create steps.
                      serieOption.type = device.content.PlotType;
                  }
                  
                  //Add plot
                  serie = self.chart.addSeries(serieOption, false, false); // Do not redraw immediately

                  if (device.content.PlotType === "arearange" && deviceIsSummary[keywordId]) {
                     //Add Ranges
                     var serieRange = self.chart.addSeries({
                         id: 'range_' + self.seriesUuid[keywordId],
                         data: range[keywordId],
                         dataGrouping: {
                            enabled: false
                         },
                         name: legendText + '(Min,Max)',
                         linkedTo: self.seriesUuid[keywordId],
                         color: device.content.color,
                         yAxis: axisName,
                         type: device.content.PlotType,
                         connectNulls: false,
                         lineWidth: 0,
                         fillOpacity: 0.3,
                         zIndex: 0,
                         visible: self.chart.keyword[keywordId].visible
                     }, false, false); // Do not redraw immediately

                     // Add Units and precision for ranges
                     if (serieRange){
                        try{
                           serieRange.units = $.t(self.unit[keywordId]);
                        }
                        catch(error){
                           serieRange.units="";
                        }
                        serieRange.precision = self.precision[keywordId];
                        serieRange.keywordTabId = keywordId;
                     }
                  }
              } catch (err2){
                  console.error('Fail to create serie : ' + err2);
              }

              if (serie){
                 //we save the unit in the serie for tooltip formatting
                 serie.units = $.t(self.unit[keywordId]);
                 
                 // register the precision for each serie into the serie
                 serie.precision = self.precision[keywordId];
                 serie.keywordTabId = keywordId;
              }
              arrayOfDeffered2[keywordId].resolve();
           });
        })
       .fail(function (error) {
          notifyError($.t("widgets.chart:errorDuringGettingDeviceData"), error);
          arrayOfDeffered2[keywordId].reject();
       });

       // Final traitment
        $.when.apply($, arrayOfDeffered2).done(function () {
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
       var d= null;
       
      $.each(self.widget.configuration.devices, function (index, device) {
         var keywordId = device.content.source.keywordId;
         var serie = self.chart.get(self.seriesUuid[keywordId]);
         var time = moment(self.serverTime)._d.getTime().valueOf();
          
          if (!isNullOrUndefined(serie)){
             var lastDate = self.chartLastXPosition[keywordId];
             if (((time - lastDate) > (self.summaryTimeBetweenNewPoint))){
				d = self.DisplaySummary(serie, keywordId, self.displayDefinition[self.interval].nb, self.displayDefinition[self.interval].type, self.prefix, lastDate);
             }
          }
      });
      return d;
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
      self.timeDeffered = self.addContinuousSummaryPoint();
      
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
   
    this.DisplaySummary = function (serie, keywordId, nb, range, prefix, lastPointDate) {
        var self = this;
        var d = null;
        try {
            //The goal is to ask to the server the elapsed time only. Example : 22h00 -> 22h59mn59s. 
            //If you ask 22h00 -> 23h00, the system return also the average for 23h. If 23h is not complete, the value will be wrong.
            var serieRange = self.chart.get('range_' + self.seriesUuid[keywordId]);
            var dateTo = DateTimeFormatter.dateToIsoDate(moment().startOf(prefix).subtract(1, prefix + 's'));
            var request_prefix = "";
            
            // we ask only from the last point registered
            var dateFrom = DateTimeFormatter.dateToIsoDate(moment(lastPointDate+1));
            
            //the week is not well calculated
            if (prefix === "week") 
               request_prefix = "day/";
            else if (prefix === "minute")
               request_prefix = "";
            else
               request_prefix = prefix + "/";
         
            d = RestEngine.getJson("rest/acquisition/keyword/" + keywordId + "/" + request_prefix + dateFrom + "/" + dateTo);
            d.done(function (data) {
               // If there is no data => return immediatly
               if (data.data.length == 0)
                  return;
                try {
                   // Weeks is not returned by the server, so we calculate here the weeks vector
                   if (prefix === "week")
                      vectorToAnalyze = getWeeks(data.data);
                   else 
                      vectorToAnalyze = data.data;
                   
                    if (!isNullOrUndefined(vectorToAnalyze) && !isNullOrUndefinedOrEmpty(vectorToAnalyze[vectorToAnalyze.length-1])) {
                       var registerDate = DateTimeFormatter.isoDateToDate(vectorToAnalyze[vectorToAnalyze.length-1].date)._d.getTime().valueOf();
                       if (registerDate != serie.points[serie.points.length-1].x){
                          self.chart.hideLoading(); // If a text was displayed before
                          var valueToDisplay = parseFloat(vectorToAnalyze[vectorToAnalyze.length-1][self.periodValueType[keywordId]]);
                    
                          if (self.differentialDisplay[keywordId]){
                             if (serie && !isNullOrUndefined(self.chartLastValue[keywordId])){
                                serie.addPoint([registerDate, (valueToDisplay-self.chartLastValue[keywordId])*self.coeff[keywordId]], 
                                               true,  // redraw. When more than 1 => false.
                                               false, // shift if true, one point at left is remove
                                               true); // animation.
                             }
                             self.chartLastValue[keywordId] = valueToDisplay;
                          }
                          else                              
                             serie.addPoint([registerDate, valueToDisplay*self.coeff[keywordId]], true, false, true);
                     
                          //Add also for ranges if any
                          if (serieRange && !self.differentialDisplay[keywordId]){
                             serieRange.addPoint([registerDate, 
                                                  parseFloat(vectorToAnalyze[vectorToAnalyze.length-1].min)*self.coeff[keywordId], 
                                                  parseFloat(vectorToAnalyze[vectorToAnalyze.length-1].max)*self.coeff[keywordId]], 
                                                  true, false, true);
                          }
                       }
                    }
                    else{ // Add null for this date
                       var registerDate = moment(self.serverTime).subtract(1, 'hours').startOf(prefix)._d.getTime().valueOf();
                       
                       if ((registerDate - serie.points[serie.points.length-1].x) > self.cleanValue)
                          serie.addPoint([registerDate, null], true, false, true);
                       
                       if (serieRange && !self.differentialDisplay[keywordId])
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
        return d.promise();
    };
    
    /**
    * event keyword deleted handler
    * @param keywordId keyword Id removed from Yadoms
    */    
    this.onKeywordDeletion = function (keywordId) {
       var self = this;
       
       if (!self.chart)
          return;

       $.each(self.chart.keyword, function (index, keyword) {
          if (keywordId.id == keyword.id){ // we found the keyword associated, index to the corresponding series
             var serie = self.chart.get(self.seriesUuid[index]);
             var serieRange = self.chart.get('range_' + self.seriesUuid[index]);             
             
             // Remove corresponding series to the keyword
             if (serie) serie.remove();
             if (serieRange) serieRange.remove();
          }
       });
    };
    
    /**
    * event wakeUp (after a reconnexion, wake down of a smartphone, tab, ...)
    */    
    this.onWakeUp = function () {
       var self = this;
       self.widgetApi.askServerLocalTime(function (serverLocalTime) {
          self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
       }).done(function(data) {
          
          // We wait that the timeDeffered is finished
          if (self.chart) { self.chart.interval = self.interval;}//we save interval in the chart
          if (self.timeDeffered!= null) {
                self.timeDeffered.done(function() {
                self.refreshData(calculateBeginDate(self.interval, self.serverTime, self.prefix));
             });
          }else{
             self.refreshData(calculateBeginDate(self.interval, self.serverTime, self.prefix));
          }
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
                     var serie = self.chart.get(self.seriesUuid[keywordId]);
                     // If a serie is available
                     if (!isNullOrUndefined(serie) && data.date!=="" & data.value!=="") {
                         // others points are treated into the onTime function
                         if  (self.prefix === "minute") {
                            var time  = moment(self.serverTime)._d.getTime().valueOf();
                            var isolastdate = DateTimeFormatter.isoDateToDate(data.date)._d.getTime().valueOf();
                            if (time - isolastdate < 3600000){ // Only if the last value is in last hour
                               self.chart.hideLoading(); // If a text was displayed before
                               if (self.differentialDisplay[keywordId]){
                                  if (serie.points.length > 0 && !isNullOrUndefined(self.chartLastValue[keywordId]))
                                     serie.addPoint([isolastdate, (parseFloat(data.value) - self.chartLastValue[keywordId])*self.coeff[keywordId]], true, false, true);
                                  self.chartLastValue[keywordId] = parseFloat(data.value);                                                 
                               }else if (isEnumVariable(keywordId)){
								  var value = getKeyByValue(self.chart.keyword[keywordId].typeInfo.values, data.value);
                                  serie.addPoint([isolastdate, value], true, false, true);
                               }else
                                  serie.addPoint([isolastdate, parseFloat(data.value)*self.coeff[keywordId]], true, false, true);
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