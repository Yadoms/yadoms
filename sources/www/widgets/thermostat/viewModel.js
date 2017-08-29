widgetViewModelCtor =

/**
 * Create a Thermostat ViewModel
 * @constructor
 */
function thermostatViewModel() {
   
    //observable data
    this.temperature = ko.observable("-");
    this.temperatureSet = ko.observable(0.0).extend({ numeric: 1 });
    this.unit = ko.observable("");
    this.step = ko.observable(0.1).extend({ numeric: 1 });
    this.isTemperatureVisible = ko.observable(true);
    //

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

    this.commandClickPlus = function () {
        this.commandClick(this.step());

    }
    this.commandClickMinus = function () {
        this.commandClick(-this.step());
    }
    
    this.commandClick = function (value) {
        if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.controlSection.content.temperatureSet))) {
            this.temperatureSet(parseFloat(this.temperatureSet()) + parseFloat(value));
            console.log ("temperature set", this.temperatureSet());
            KeywordManager.sendCommand(this.widget.configuration.controlSection.content.temperatureSet.keywordId, this.temperatureSet().toString());
        }
        this.widgetApi.find(".textfit").fitText();
    };    
    
    this.configurationChanged = function () {
        var self = this;

        //we get the unit of the keyword
        self.widgetApi.getKeywordInformation(self.widget.configuration.LivetemperatureSection.content.temperatureDevice.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions([self.widget.configuration.LivetemperatureSection.content.temperatureDevice.keywordId,
                                                    self.widget.configuration.controlSection.content.temperatureSet.keywordId,
                                                    self.widget.configuration.thermostatStateSection.content.state.keywordId
                                                   ]);
        
        //we fill the deviceId of the battery indicator
        //TODO : handle all keywords
        self.widgetApi.configureBatteryIcon(self.widget.configuration.controlSection.content.temperatureSet.deviceId);
        
        //Read the step
        self.step(self.widget.configuration.controlSection.content.stepValue);
        
        // Visibility of the temperature
        self.isTemperatureVisible(self.widget.configuration.LivetemperatureSection.checkbox);
    }

    this.resized = function () {
        var self = this;
        
        if (self.widget.getHeight() == 200)
        {
           self.widgetApi.find(".btn").addClass("btn-md");
           self.widgetApi.find(".btn").removeClass("btn-lg btn-xs");
        }
        else if (self.widget.getHeight() == 300)
        {
           self.widgetApi.find(".btn").addClass("btn-lg");
           self.widgetApi.find(".btn").removeClass("btn-xs btn-md");
        }
        else if (self.widget.getHeight() == 100)
        {
           self.widgetApi.find(".btn").addClass("btn-xs");
           self.widgetApi.find(".btn").removeClass("btn-lg btn-md");
        }
        else{}      
    };
    
    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === self.widget.configuration.LivetemperatureSection.content.temperatureDevice.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(1);
               self.temperature(temp.toString());
            }
            else 
               self.temperature("-");
        }
        else if (keywordId === self.widget.configuration.controlSection.content.temperatureSet.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(1);
               self.temperatureSet(temp);
            }
            else 
               self.temperatureSet("-");
        } 
        else if (keywordId === self.widget.configuration.thermostatStateSection.content.state.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               if (!parseBool(data.value))
                  this.widgetApi.find(".icon-div").css("visibility", "hidden");
               else 
                  this.widgetApi.find(".icon-div").css("visibility", "visible");
            }
            else 
               this.widgetApi.find(".icon-div").css("visibility", "hidden");
        }
        self.widgetApi.fitText();
    };
};