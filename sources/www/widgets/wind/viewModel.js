/**
 * Create a Weather ViewModel
 * @constructor
 */
widgetViewModelCtor = function weatherViewModel() {
    //observable data
    this.windspeed = ko.observable("-");
    this.direction = "-";
    this.speedUnit = "kmh";
    this.background = ko.observable("widgets/wind/images/Windrose-fr.png");
    this.correction = 0;

    this.displayNeedle = function (direction) {
        var self = this;
        var element = self.widgetApi.find(".wind-canvas");
        var mainDiv = self.widgetApi.find(".mainDiv");
        var background = self.widgetApi.find(".background")

        //Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
        element.attr('width', mainDiv.width());
        element.attr('height', mainDiv.height());
        element.css('width', mainDiv.width());
        element.css('height', mainDiv.height());

        //get a reference to the canvas
        var ctx = element.get(0).getContext("2d");

        // Refresh the canvas, clear all existing information
        ctx.clearRect(0, 0, mainDiv.width(), mainDiv.height());
        var offsetX = (mainDiv.width() - background.width()) / 2 + 2;
        var offsetY = 0;
        var centerX = offsetX + background.width() / 2;
        var centerY = offsetY + background.height() / 2;

        var needleLenght = Math.min(background.width(), background.height()) / (200 / 70);

        var radius = direction * Math.PI / -180;

        ctx.fillStyle = "rgb(128,0,0)";
        ctx.strokeStyle = '#ff0000';
        ctx.beginPath();
        ctx.lineWidth = 1;
        ctx.moveTo(centerX, centerY);
        ctx.lineTo(centerX - needleLenght * Math.sin(radius - 10 * Math.PI / 180) / 3, centerY - needleLenght * Math.cos(radius - 10 * Math.PI / 180) / 3);
        ctx.lineTo(centerX - needleLenght * Math.sin(radius), centerY - needleLenght * Math.cos(radius));
        ctx.lineTo(centerX - needleLenght * Math.sin(radius + 10 * Math.PI / 180) / 3, centerY - needleLenght * Math.cos(radius + 10 * Math.PI / 180) / 3);
        ctx.lineTo(centerX, centerY);
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
    };

    this.displayWindSpeed = function () {
        self = this;
        var divSpeed = self.widgetApi.find(".windspeed");
        var mainDiv = self.widgetApi.find(".mainDiv");

        divSpeed.css('top', mainDiv.height() - 22);
    };

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;

        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: true
        });
    };

    this.mod = function (number, n) {
        var m = ((number % n) + n) % n;
        return m < 0 ? m + Math.abs(n) : m;
    };

    this.knotsToBeaufort = function (knots) {
        if (knots <= 1) return 0;
        if (knots < 3.5) return 1;
        if (knots < 6.5) return 2;
        if (knots < 10.5) return 3;
        if (knots < 16.5) return 4;
        if (knots < 21.5) return 5;
        if (knots < 27.5) return 6;
        if (knots < 33.5) return 7;
        if (knots < 40.5) return 8;
        if (knots < 47.5) return 9;
        if (knots < 55.5) return 10;
        if (knots < 63.5) return 11;
        if (knots < 74.5) return 12;
        if (knots < 80.5) return 13;
        if (knots < 89.5) return 14;
        return 15;
    }

    this.formatSpeed = function (speed) {
        var speedTxt = "";
        switch (self.speedUnit) {
            case "ms":
                speedTxt = parseFloat(speed).toFixed(1);
                break;
            case "knots":
                speedTxt = (parseFloat(speed) * 1.94384).toFixed(1);
                break;
            case "beaufort":
                speedTxt = self.knotsToBeaufort((parseFloat(speed) * 1.94384));
                break;
            default: // = kmh
                speedTxt = (parseFloat(speed) * 3600 / 1000).toFixed(1);
                break;
        }
        return speedTxt + " " + $.t("widgets.wind:configurationSchema.options.content.speedUnit.values." + self.speedUnit);
    }

    /**
     * New acquisition handler
     * @param keywordId keywordId on which new acquisition was received
     * @param data Acquisition data
     */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (parseBool(self.widget.configuration.speedDisplay.checkbox)) {
            if (keywordId === self.widget.configuration.speedDisplay.content.windSpeed.keywordId) {
                self.windspeed(self.formatSpeed(data.value));
            }
        }

        if (keywordId === self.widget.configuration.windDirection.keywordId) {
            self.direction = self.mod(parseFloat(data.value) + self.correction, 360);
            self.displayNeedle(self.direction);
        }
    };

    this.resized = function () {
        var self = this;

        self.displayNeedle(self.direction);
        self.displayWindSpeed();
    };

    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;

        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions([self.widget.configuration.windDirection.keywordId]);

        //we register keyword for get last value at web client startup
        self.widgetApi.getLastValue([self.widget.configuration.windDirection.keywordId]);

        // we add only the speed if we have ckeck it
        if (parseBool(self.widget.configuration.speedDisplay.checkbox)) {
            self.widgetApi.registerKeywordForNewAcquisitions([self.widget.configuration.speedDisplay.content.windSpeed.keywordId]);
            self.widgetApi.getLastValue([self.widget.configuration.speedDisplay.content.windSpeed.keywordId]);
            self.widgetApi.find(".windspeed").css("visibility", "visible");
        } else
            self.widgetApi.find(".windspeed").css("visibility", "hidden");

        if (parseBool(self.widget.configuration.options.checkbox)) {
            self.correction = parseInt(self.widget.configuration.options.content.directionCorrection);
            self.speedUnit = self.widget.configuration.options.content.speedUnit;
        } else {
            self.correction = 0;
            self.speedUnit = "kmh";
        }

        self.displayNeedle("-");
    }
};