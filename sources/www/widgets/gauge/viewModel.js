
/**
 * Create a Gauge ViewModel
 * @constructor
 */
widgetViewModelCtor = function gaugeViewModel() {

    /**
     * Initialization method
     * @param widget widget class object
     */

    this.initialize = function () {
        
        var self = this;

        //observable data

        self.fontLabelSize = ko.observable("1.5em");
        self.fontUnitSize = ko.observable("1.5em");

        this.fontLabelSizeCss = ko.computed(function () {
            return { "font-size": self.fontLabelSize() };
        }, this);
        this.fontUnitSizeCss = ko.computed(function () {
            return { "font-size": self.fontUnitSize() };
        }, this);

        self.unit = "";

        self.stopsArray = null;

        self.$chart = self.widgetApi.find("div.innerDiv");

        var d = new $.Deferred();
        self.widgetApi.loadLibrary([
            "libs/highstock/js/highstock.js",
            "libs/highstock/js/highcharts-more.js",
            "libs/highstock/js/modules/exporting.js",
            "libs/highstock/js/modules/solid-gauge.js",
            "libs/highcharts-export-clientside/js/highcharts-export-clientside.min.js"
        ]).done(function () {
            
            //we create the battery indicator
            self.widget.$toolbar.append("<div class=\"widget-toolbar-battery\" deviceId=\"\"></div>");

            d.resolve();
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
            }
        }
    };

    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

        //we register keyword new acquisition
        WidgetApi.keyword.registerKeywordAcquisitions(self.widget, self.widget.configuration.device.keywordId);

        // Delete all elements in stopsArray
        self.stopsArray = [];

        //we fill the deviceId of the battery indicator
        WidgetApi.toolbar.configureBatteryIcon(self.widget, self.widget.configuration.device.deviceId);

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
        KeywordManager.get(self.widget.configuration.device.keywordId)
        .done(function (keyword) {
            if (self.chart) {
                var serie = self.chart.series[0];
                serie.units = $.t(keyword.units);
            }
        });

        var gaugeOptions = {

            chart: {
                type: "solidgauge"
            },

            title: null,
            pane: {
                center: ["50%", "85%"],
                size: "140%",
                startAngle: -90,
                endAngle: 90,
                background: {
                    backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || "#EEE",
                    innerRadius: "60%",
                    outerRadius: "100%",
                    shape: "arc"
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
                minorTickInterval: null,
                tickPixelInterval: 400,
                tickWidth: 0,
                labels: {
                    enabled: true,
                    align: "left",
                    x: -5,
                    y: 16
                },
                min: parseInt(self.widget.configuration.customYAxisMinMax.content.minimumValue), //Minimum value
                max: parseInt(self.widget.configuration.customYAxisMinMax.content.maximumValue), //Maximal value
                stops: self.stopsArray // Stops for bar colors with thresholds
            },

            plotOptions: {
                solidgauge: {
                    dataLabels: {
                        enabled: true,
                        y: 5,
                        borderWidth: 0,
                        useHTML: true
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
                    formatter: function() {
                        return '<div class="value" style="text-align:center"><span>' + this.y.toFixed(1) + '</span><br/>' +
                                '<span class="unit">' + this.series.units + '</span></div>';
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
        //debugger;
        if (!isNullOrUndefined(self.chart)) {
            //this.chart.setSize(self.widget.width() - 10, self.widget.height() - 10, true);
            
        }
       
        //we manage the font size of value and its unit
        var width = this.widget.width();
        var height = this.widget.height();
        
        if (width <= 100) {
            self.fontLabelSize("1em");
            self.fontUnitSize("1em");
            this.chart.panes[0].options.size = "60%";
        } else if (width <= 200) {
            self.fontLabelSize("1em");
            self.fontUnitSize("1em");
            this.chart.panes[0].options.size = "80%";
        } else if (width <= 300) {
            self.fontLabelSize("1em");
            self.fontUnitSize("1em");
            this.chart.panes[0].options.size = "140%";
        } else {
            self.fontLabelSize("1em");
            self.fontUnitSize("1em");
            this.chart.panes[0].options.size = "200%";
        }

        $(window).trigger("resize");
        /*else {
            this.chart.panes[0].options.size = "140%";
            //we look for height and width
            if (width <= 200) {
                if (height <= 100) {
                    self.fontLabelSize("3em");
                    self.fontUnitSize("4.5em");
                } else {
                    self.fontLabelSize("3em");
                    self.fontUnitSize("4.5em");
                }
            } else {
                if (height <= 100) {
                    self.fontLabelSize("3em");
                    self.fontUnitSize("4.5em");
                } else {
                    self.fontLabelSize("3em");
                    self.fontUnitSize("4.5em");
                }
            }
        }*/

    };
};