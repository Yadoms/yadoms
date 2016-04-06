
/**
 * Create a Gauge ViewModel
 * @constructor
 */
widgetViewModelCtor = function gaugeViewModel() {

    var self = this;
    //observable data
    self.value = ko.observable("");
    self.unit = ko.observable("");

    this.valueAndUnit = ko.computed(function () {
        return self.value() + " " + self.unit();
    });

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
            "libs/highstock/js/modules/solid-gauge.js",
            "libs/highcharts-export-clientside/js/highcharts-export-clientside.min.js"
        ]).done(function () {
            
            //we create the battery indicator
            self.widgetApi.toolbar.addBatteryIconToWidget();

            //we get the unit of the keyword
            self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
                self.unit($.t(keyword.units));
            });

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

        // Delete all elements in stopsArray
        self.stopsArray = [];

        //we fill the deviceId of the battery indicator
        self.widgetApi.toolbar.configureBatteryIcon(self.widget.configuration.device.deviceId);

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
                //center: ["50%", "50%"],
                size: "100%",
                startAngle: 0,
                endAngle: 360,
                background: {
                    backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || "#EEE",  //TODO : change background color to adapt to color with opacity addded (ie Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0.3).get())
                    innerRadius: "70%",
                    outerRadius: "100%",
                    borderWidth: 0
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
                tickPositions: [],
                min: parseInt(self.widget.configuration.customYAxisMinMax.content.minimumValue), //Minimum value
                max: parseInt(self.widget.configuration.customYAxisMinMax.content.maximumValue), //Maximal value
                stops: self.stopsArray // Stops for bar colors with thresholds
            },

            plotOptions: {
                solidgauge: {
                    borderWidth: '40px',
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
                /*dataLabels: {
                    formatter: function() {
                        return '<div class="value" style="text-align:center"><span>' + this.y.toFixed(1) + '</span><br/>' +
                                '<span class="unit">' + this.series.units + '</span></div>';
                    }
                },*/
                units : "" //custom field
            }]
        };
        self.$chart.highcharts(gaugeOptions);
        self.chart = self.$chart.highcharts();
    };

    this.resized = function () {
        var self = this;
        debugger;
        if (!isNullOrUndefined(self.chart)) {
            this.chart.setSize(self.widget.innerWidth() - 10, self.widget.innerHeight() - self.$value.height(), true);
        }
        a = this;
        $(window).trigger("resize");
    };
};