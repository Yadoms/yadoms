
/**
 * Create a Gauge ViewModel
 * @constructor
 */
widgetViewModelCtor = function gaugeViewModel() {

    var self = this;
    //observable data
    self.value = ko.observable(0).extend({ numeric: 1 });
    self.unit = ko.observable("");

    /**
     * Initialization method
     * @param widget widget class object
     */

    this.initialize = function () {
        
        var self = this;

        self.stopsArray = null;

        self.$chart = self.widgetApi.find("div.innerDiv");
        self.$value = self.widgetApi.find("div.value");

        var d = new $.Deferred();
        self.widgetApi.loadLibrary([
            "libs/highstock/js/highstock.js",
            "libs/highstock/js/highcharts-more.js",
            "libs/highstock/js/modules/exporting.js",
            "libs/highstock/js/modules/solid-gauge.js"
        ]).done(function () {
            
            //we configure the toolbar
            self.widgetApi.toolbar({
                activated: true,
                displayTitle: true,
                batteryItem: true
            });
            
            //we get the unit of the keyword
            self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
                self.unit($.t(keyword.units));
                d.resolve();
            })
            .fail(function (error) {
               notifyError($.t("widgets/gauge:errorInitialization"), error);
               throw $.t("widgets/gauge:errorInitialization");
               d.reject();
            });
        })
        .fail(function (error) {
            notifyError($.t("widgets/gauge:errorInitialization"), error);
            throw $.t("widgets/gauge:errorInitialization");
            d.reject();
        });
        return d.promise();
    };

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        if (keywordId === self.widget.configuration.device.keywordId) {
            //it is the right device
            if (self.chart) {
                var point = self.chart.series[0].points[0];
                point.update(parseFloat(data.value));
                self.value(data.value);
            }
        }
    };

    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);

        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);

        // Delete all elements in stopsArray
        self.stopsArray = [];

        switch (self.widget.configuration.displayMode.activeSection) {
            case "solidColor":
                self.stopsArray.push([0, self.widget.configuration.displayMode.content.solidColor.content.color]);
                break;
            case "thresholds":
                var previousColor = self.widget.configuration.displayMode.content.thresholds.content.firstColor;
                self.widget.configuration.displayMode.content.thresholds.content.addedThresholds.forEach(function (addedThreshold) {
                    var thresholdRatio = (addedThreshold.content.value - self.widget.configuration.customYAxisMinMax.content.minimumValue) /
                    (self.widget.configuration.customYAxisMinMax.content.maximumValue - self.widget.configuration.customYAxisMinMax.content.minimumValue);

                    self.stopsArray.push([thresholdRatio - 0.001, previousColor]);
                    self.stopsArray.push([thresholdRatio, addedThreshold.content.color]);

                    previousColor = addedThreshold.content.color;
                });
                break;
            case "gradient":
                self.stopsArray.push([0, self.widget.configuration.displayMode.content.gradient.content.minColor]);
                self.stopsArray.push([1, self.widget.configuration.displayMode.content.gradient.content.maxColor]);
                break;
        }

         //we get the unit of the keyword
         self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
             self.unit($.t(keyword.units));
         });        

        var minValue;
        var maxValue;
        if ((self.widget.configuration.customYAxisMinMax) && (self.widget.configuration.customYAxisMinMax.content)) {
            minValue = parseInt(self.widget.configuration.customYAxisMinMax.content.minimumValue);
            maxValue = parseInt(self.widget.configuration.customYAxisMinMax.content.maximumValue);
        }
            

        var gaugeOptions = {

            chart: {
                type: "solidgauge"
            },

            title: null,
            pane: {
                center: ["50%", "80%"],
                size: "100%",
                startAngle: -90,
                endAngle: 90,
                background: {
                    backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || "#EEE", 
                    innerRadius: "60%",
                    outerRadius: "100%",
                    //borderWidth: 0
                    shape: 'solid'
                }
            },

            tooltip: {
                enabled: false
            },
            credits: {
                enabled: true
            },

            // the value axis
            yAxis: {
                lineWidth: 0,
                tickWidth: 0,
                tickPositions: [],
                min: minValue,
                max: maxValue,
                stops: self.stopsArray // Stops for bar colors with thresholds
            },

            plotOptions: {
                solidgauge: {
                    borderWidth: 0,
                    dataLabels: {
                        enabled: false
                    },
                    linecap: 'round'
                }
            },
            exporting: {
                enabled: false
            },

            series: [{
                name: 'Data',
                data: [1],
                dataLabels: {
                    formatter: function() {
                        return '<div class="value text-fit" style="text-align:center"><span>' + this.y.toFixed(1) + '</span><br/>' +
                                '<span class="unit text-fit">' + self.units + '</span></div>';
                    }
                },
                
                units : "" //custom field
            }]
        };
        self.$chart.highcharts(gaugeOptions);
        self.chart = self.$chart.highcharts();
    };

    this.resized = function () {
        var self = this;
        if (!isNullOrUndefined(self.chart)) {
            this.chart.setSize(self.widget.getInnerWidth() - 20, self.$chart.height(), false);
            //we resize value and unit text to keep them into the gauge
            $(".valueAndUnit").css("height", (self.$chart.find("path")[0].getBBox().height / 1.5) + "px");
        }
        self.widgetApi.fitText();
        $(window).trigger("resize");
    };
};