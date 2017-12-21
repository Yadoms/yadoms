widgetViewModelCtor =

/**
 * Create a Counter Display ViewModel
 * @constructor
 */
function counterDisplayViewModel() {
    //observable data
    this.data = ko.observable(0).extend({ numeric: 1 });
    this.unit = ko.observable("");
    this.fontSize = ko.observable(20);
    this.fontSizeCss = ko.computed(function () {
        return { "fontSize": this.fontSize() + "px" };
    }, this);

    this.minimumIntegerDigit = 9;
    this.odometer = null;

    /**
      * Initialization method
      * @param widget widget class object
      */
    this.initialize = function () {
        var self = this;

        var d = new $.Deferred();

        self.widgetApi.loadLibrary([
            "widgets/counter-display/lib/odometer-0.4.6/odometer.js"
        ]).done(function () {
            self.widgetApi.loadCss("widgets/counter-display/lib/odometer-0.4.6/themes/odometer-theme-car.css")
                .done(function () {

                    window.odometerOptions = {
                        auto: false // Don't automatically initialize everything with class 'odometer'
                    };

                    // For each odometer, initialize with the theme passed in:
                    self.odometer = new Odometer(
                    {
                        el: self.widgetApi.find(".odometer")[0],
                        format: '(.ddd)',
                        value: 0,
                        theme: 'car',
                        duration: 1000,
                        selector: '.my-numbers',
                        minimumIntegerDigit: self.minimumIntegerDigit
                    });

                    //we configure the toolbar
                    self.widgetApi.toolbar({
                        activated: true,
                        displayTitle: true,
                        batteryItem: true
                    });
                    d.resolve();
                })
                .fail(function (error) {
                   notifyError(error);
                   d.reject();
                });
        })
       .fail(function (error) {
          notifyError(error);
          d.reject();
       });        
       
       return d.promise();
    };

    this.configurationChanged = function () {

        var self = this;

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);

        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);

        //we get the unit of the keyword
        var deffered = self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId);
        
        deffered
        .done(function (keyword) {
            // Read the unit
            self.unit($.t(keyword.units));

            //Set the Unit for odometer
            self.odometer.SetUnit(self.unit());

            self.resizefont();
        });
        
        return deffered.promise();
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        if (keywordId === self.widget.configuration.device.keywordId) {
            //it is the right device
            self.widgetApi.find(".odometer").html(data.value);
        }
    };

    this.resizefont = function () {
        //Compute the font-size needed.
        this.fontSize(((this.widget.getWidth() - 9 - ((this.minimumIntegerDigit / 3) - 1) * 8.33) / (this.minimumIntegerDigit + this.unit().length)) * 1.11);
    }

    this.resized = function () {
        this.resizefont();
    };

};