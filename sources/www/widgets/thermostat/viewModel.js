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
    this.disableStateTemperature = ko.observable(false);
    this.thermostatStateType = "";
    
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
    };    
    
    this.configurationChanged = function () {
        var self = this;
        
        if (!parseBool(self.widget.configuration.thermostatStateSection.checkbox))
           self.widgetApi.find(".icon-div").css("visibility", "hidden");
        
        self.widgetApi.registerAdditionalInformation(["dataType", "units"]); // We would like the unit !
        
        var keywordRegistered = [];
        keywordRegistered.push(self.widget.configuration.controlSection.content.temperatureSet.keywordId);
        
        if (parseBool(self.widget.configuration.thermostatStateSection.checkbox))
           keywordRegistered.push(self.widget.configuration.thermostatStateSection.content.state.keywordId);
        
        if (parseBool(self.widget.configuration.LivetemperatureSection.checkbox))
           keywordRegistered.push(self.widget.configuration.LivetemperatureSection.content.temperatureDevice.keywordId);
		
        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(keywordRegistered);	   
	   
		//we register keyword for get last value at web client startup
		self.widgetApi.getLastValue(keywordRegistered);
        
        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.controlSection.content.temperatureSet.deviceId);
        
        //Read the step
        self.step(self.widget.configuration.controlSection.content.stepValue);
        
        // Visibility of the temperature
        self.isTemperatureVisible(parseBool(self.widget.configuration.LivetemperatureSection.checkbox));
        
        if (!parseBool(self.widget.configuration.LivetemperatureSection.checkbox) && !parseBool(self.widget.configuration.thermostatStateSection.checkbox))
           self.disableStateTemperature(true);
        else
           self.disableStateTemperature(false);
    }

    this.resized = function () {
        var self = this;
        
        if (self.widget.getHeight() == 200){
           self.widgetApi.find(".btn").addClass("btn-md");
           self.widgetApi.find(".btn").removeClass("btn-lg btn-th");
        }
        else if (self.widget.getHeight() == 300){
           self.widgetApi.find(".btn").addClass("btn-lg");
           self.widgetApi.find(".btn").removeClass("btn-th btn-md");
        }
        else if (self.widget.getHeight() == 100){
           self.widgetApi.find(".btn").addClass("btn-th");
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
        
        if (keywordId === parseInt(self.widget.configuration.LivetemperatureSection.content.temperatureDevice.keywordId)) {
           if (!isNullOrUndefinedOrEmpty(data.unit))
              self.unit($.t(data.unit));
           
            //it is the right device
            if (data.value !==""){
               var temp = parseFloat(data.value).toFixed(1);
               self.temperature(temp.toString());
            }
            else 
               self.temperature("-");
        }
        else if (keywordId === parseInt(self.widget.configuration.controlSection.content.temperatureSet.keywordId)) {
            //it is the right device
            if (data.value !==""){
               var temp = parseFloat(data.value).toFixed(1);
               self.temperatureSet(temp);
            }
            else 
               self.temperatureSet("-");
        } 
        else if (keywordId === parseInt(self.widget.configuration.thermostatStateSection.content.state.keywordId)) {
           if (!isNullOrUndefinedOrEmpty(data.type))
              self.thermostatStateType = data.type;           
           
            //it is the right device
            if (data.value !==""){
               if (self.thermostatStateType === "Bool") {
                  if (!parseBool(data.value))
                     this.widgetApi.find(".icon-div").css("visibility", "hidden");
                  else 
                     this.widgetApi.find(".icon-div").css("visibility", "visible");
               }
               else if (self.thermostatStateType === "String") {
                  if (data.value === "Idle")
                     this.widgetApi.find(".icon-div").css("visibility", "hidden");
                  else if (data.value === "Heating")
                     this.widgetApi.find(".icon-div").css("visibility", "visible");
                  else {}
               }
               else {}
            }
            else 
               this.widgetApi.find(".icon-div").css("visibility", "hidden");
        }
    };
};