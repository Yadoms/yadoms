widgetViewModelCtor =

/**
 * Create a Numeric Display ViewModel
 * @constructor
 */
function numericDisplayViewModel() {
   
    this.precision = 1;
    this.data = ko.observable("-");
    this.rawUnit = "";
    this.unit = ko.observable("");
    this.shouldBeVisible = ko.observable(false);
    this.automaticScale = ko.observable(true);
    this.lastReceiveDate = ko.observable("");
    this.capacity = "";
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

    this.displayDuration = function (value) {
      var self = this;
      
      if (value ==="not-a-date-time"){
         self.data("-");
         return;
      }
      
      self.widgetApi.find(".unit").addClass("hidden");
      var d = moment.duration(parseFloat(value), "seconds");
      if (d.asSeconds() < 1) {  // Display in millisecond
         self.data(d.milliseconds().toString() + " ms");
      } else if (d.asSeconds() < 30) { // Display in seconds + milliseconds
         self.data((d.seconds() + d.milliseconds()/1000).toFixed(2).toString() + " s");
      } else { // Standard display
         // format function doesn't look to works for duration ...
         self.data(d.hours().toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false}) + ":" + 
                   d.minutes().toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false}) + ":" + 
                   d.seconds().toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false}));
      }
    };
    
    this.configurationChanged = function () {
       var self = this;
       
       try{
          self.shouldBeVisible(parseBool(self.widget.configuration.dateDisplay));
       }
       catch(error){
          self.shouldBeVisible(false);
          console.warn (error);
       }
       
       try{
          if (!isNullOrUndefined(self.widget.configuration.automaticScale)){
             self.automaticScale(parseBool(self.widget.configuration.automaticScale));
          } else{
             self.automaticScale(true);
          }
       }
       catch(error){
          self.automaticScale(true);
          console.warn (error);
       }       
        
       if (!isNullOrUndefined(self.widget.configuration.precision))
          self.precision = parseInt(self.widget.configuration.precision, 10);
       else
          self.precision = 1;
	   
        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.device.keywordId));
	   
		  //we register keyword for get last value at web client startup
        self.widgetApi.getLastValue(parseInt(self.widget.configuration.device.keywordId));
        
        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(parseInt(self.widget.configuration.device.deviceId));
        self.widgetApi.registerAdditionalInformation(["unit", "capacity"]);
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        if (keywordId === parseInt(self.widget.configuration.device.keywordId)) {
           // Receive at startup data.unit and data.capacity
           if (!isNullOrUndefinedOrEmpty(data.unit))
              self.rawUnit = data.unit;
           
           if (!isNullOrUndefinedOrEmpty(data.capacity))
              self.capacity = data.capacity;
           
           // If no unit, we hide the unit display
           if (self.rawUnit === "data.units.noUnit")
              self.widgetApi.find(".unit").addClass("hidden");
           else
              self.widgetApi.find(".unit").removeClass("hidden");
          
            //it is the right device
            if (data.value !==""){
               if (self.capacity ==="duration"){
                  self.displayDuration(data.value);
               }else {
                  var temp = parseFloat(data.value);
                  if (self.automaticScale()){
                     adaptValueAndUnit(temp, self.rawUnit, function(newValue, newUnit) {
                        self.unit($.t(newUnit));
                        self.data(newValue.toFixed(self.precision).toString());
                     });
                  }else{
                     self.unit($.t(self.rawUnit));
                     self.data(temp.toFixed(self.precision).toString());
                  }
               }
            }else 
               self.data("-");
            
            if (self.shouldBeVisible()){
               if (data.date!=="")
                  self.lastReceiveDate(DateTimeFormatter.isoDateToDate (data.date).calendar().toString());
               else
                  self.lastReceiveDate($.t("widgets/numeric-display:NoAcquisition"));
            }
        }
    };
};