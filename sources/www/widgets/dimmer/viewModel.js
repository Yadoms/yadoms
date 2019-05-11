/**
 * Create a Dimmer ViewModel
 * @constructor
 */
widgetViewModelCtor = function dimmerViewModel() {
    //observable data
    this.data = ko.observable(0).extend({ numeric: 1 });
    this.unit = ko.observable("");

    //widget identifier
    this.step = ko.observable(0.1).extend({ numeric: 1 });

    this.commandClickPlus = function () {
       this.commandClick(this.step());
    }
    this.commandClickMinus = function () {
       this.commandClick(-this.step());
    }

    this.commandClick = function (value) {
       if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
          this.data(this.data() + value);
          KeywordManager.sendCommand(parseInt(this.widget.configuration.device.keywordId), this.data().toString());
       }
    };

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: true
        });
    };

    this.configurationChanged = function () {
        var self = this;
        self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.device.keywordId));
		  self.widgetApi.getLastValue(parseInt(self.widget.configuration.device.keywordId));
        self.widgetApi.registerAdditionalInformation(["unit"]); // return unit with the getLast Value
        self.widgetApi.configureBatteryIcon(parseInt(self.widget.configuration.device.deviceId));
        self.step(parseInt(self.widget.configuration.StepValue));
    };

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === parseInt(self.widget.configuration.device.keywordId)) {
           if (!isNullOrUndefinedOrEmpty(data.unit))
              self.unit($.t(self.rawUnit = data.unit));

           self.data(parseFloat(data.value));
        }
    };
};