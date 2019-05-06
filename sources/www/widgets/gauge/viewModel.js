/**
 * Create a Gauge ViewModel
 * @constructor
 */
widgetViewModelCtor = function gaugeViewModel() {
    //observable data
    this.value = ko.observable("");
    this.unit = ko.observable("");

    /**
     * Initialization method
     * @param widget widget class object
     */

    this.initialize = function () {
        var self = this;
        var d = new $.Deferred();
        self.stopsArray = null;
        self.$chart = self.widgetApi.find("div.innerDiv");

        //
        // For chart and gauge, compressed gz file will appears soon. At this time, there is some dependancies to handle.
        //        

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
                            shape: 'solid'
                        }
                    },
                    tooltip: {
                        enabled: false
                    },
                    credits: {
                        enabled: false
                    },
                    // the value axis
                    yAxis: {
                        lineWidth: 0,
                        tickWidth: 0,
                        tickPositions: [],
                        min: 0,
                        max: 100
                    },
                    plotOptions: {
                        solidgauge: {
                            borderWidth: 0,
                            dataLabels: {
                                enabled: false
                            }
                        }
                    },
                    exporting: {
                        enabled: false
                    },
                    series: [{
                        name: 'Data',
                        data: [1],
                        dataLabels: {
                            formatter: function () {
                                return '<div class="value" data-bind="text: value()" style="text-align:center"><div>' + this.y.toFixed(1) + '</div>' +
                                    '<div class="unit" data-bind="text: unit()"></div></div>';
                            }
                        }
                    }]
                };
                self.$chart.highcharts(gaugeOptions);
                self.chart = self.$chart.highcharts();
                d.resolve();
            })
            .fail(function (error) {
                self.widgetApi.setState(widgetStateEnum.InvalidConfiguration);
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
        try {
            if (keywordId === parseInt(self.widget.configuration.device.keywordId) && self.chart) {
                var point = self.chart.series[0].points[0];
                if (isNullOrUndefinedOrEmpty(data.value)) {
                    point.update(0);
                    self.value("-");
                } else {
                    point.update(parseFloat(data.value));
                    self.value(data.value);
                }

                self.unit(isNullOrUndefinedOrEmpty(data.unit) ? "" : $.t(data.unit));
            }
        } catch (error) {
            self.widgetApi.setState(widgetStateEnum.InvalidConfiguration);
        }
    };

    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);
        self.widgetApi.getLastValue(self.widget.configuration.device.keywordId);
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        self.widgetApi.registerAdditionalInformation(["unit"]);
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

        var minValue;
        var maxValue;
        if ((self.widget.configuration.customYAxisMinMax) && (self.widget.configuration.customYAxisMinMax.content)) {
            minValue = parseInt(self.widget.configuration.customYAxisMinMax.content.minimumValue);
            maxValue = parseInt(self.widget.configuration.customYAxisMinMax.content.maximumValue);
        }

        // Update only these options
        var gaugeOptions = {
            yAxis: {
                lineWidth: 0,
                tickWidth: 0,
                tickPositions: [],
                min: minValue,
                max: maxValue,
                stops: self.stopsArray // Stops for bar colors with thresholds
            }
        }
        self.chart.update(gaugeOptions);
    };

    this.resized = function () {
        var self = this;
        if (!isNullOrUndefined(self.chart)) {
            this.chart.setSize(self.widget.getInnerWidth() - 20, self.$chart.height(), false);
            //we resize value and unit text to keep them into the gauge
            $(".valueAndUnit").css("height", (self.$chart.find("path")[0].getBBox().height / 1.5) + "px");
        }
        $(window).trigger("resize");
    };
};