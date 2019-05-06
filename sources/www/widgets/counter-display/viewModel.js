widgetViewModelCtor =

/**
 * Create a Counter Display ViewModel
 * @constructor
 */
function counterDisplayViewModel() {
    //observable data
    this.data = ko.observable(0).extend({ numeric: 1 });
    this.unit = ko.observable("");
    this.coeff = 1;
    this.fontSize = ko.observable(20);
    this.minimumIntegerDigit = 9;
    this.odometer = null;    
    this.fontSizeCss = ko.computed(function () {
        return { "fontSize": this.fontSize() + "px" };
    }, this);

    /**
      * Initialization method
      * @param widget widget class object
      */
    this.initialize = function () {
        var self = this;
        var d = new $.Deferred();
        var arrayOfDeffered = [];

        arrayOfDeffered.push(self.widgetApi.loadLibrary("widgets/counter-display/lib/odometer-0.4.6/odometer.js"));
        arrayOfDeffered.push(self.widgetApi.loadCss("widgets/counter-display/lib/odometer-0.4.6/themes/odometer-theme-car.css"));
        $.when.apply($,arrayOfDeffered)
        .done(function () {
            window.odometerOptions = {
               auto: false // Don't automatically initialize everything with class 'odometer'
            };

            // For each odometer, initialize with the theme passed in:
            self.odometer = new Odometer({
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
       return d.promise();
    };

    this.configurationChanged = function () {
        var self = this;

        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
	   
        //we register keyword for get last value at web client startup
        self.widgetApi.getLastValue(self.widget.configuration.device.keywordId);  
		
        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        self.widgetApi.registerAdditionalInformation(["unit"]);
        self.resizefont();
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
       var self = this;
       if (keywordId === parseInt(self.widget.configuration.device.keywordId)) {
          // Receive at startup data.unit
          if (!isNullOrUndefinedOrEmpty(data.unit)){
             self.rawUnit = data.unit;
             
            switch (self.rawUnit){
               case "data.units.wattPerHour":
                  self.rawUnit = "KWh";
                  self.coeff = 1;
                  break;
               default:
                  self.coeff = 1000;
                  break;
            }
            self.unit($.t(self.rawUnit));

            //Set the Unit for odometer
            self.odometer.SetUnit(self.unit());    
            self.resizefont();            
          }
          
          self.widgetApi.find(".odometer").html( parseFloat(data.value)*self.coeff);
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