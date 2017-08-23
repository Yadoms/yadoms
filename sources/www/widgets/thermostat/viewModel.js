widgetViewModelCtor =

/**
 * Create a Thermostat ViewModel
 * @constructor
 */
function thermostatViewModel() {
   
    //observable data
    this.data = ko.observable("-");
    this.unit = ko.observable("");
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

    this.configurationChanged = function () {
        var self = this;

        //we get the unit of the keyword
        self.widgetApi.getKeywordInformation(self.widget.configuration.temperatureDevice.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions([self.widget.configuration.temperatureDevice.keywordId,
                                                    self.widget.configuration.temperatureSet.keywordId,
                                                    self.widget.configuration.heating.keywordId
                                                   ]);
        
        //we fill the deviceId of the battery indicator
        //TODO : handle all keywords
        self.widgetApi.configureBatteryIcon(self.widget.configuration.temperatureSet.deviceId);
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === self.widget.configuration.temperatureDevice.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(self.precision);
               self.data(temp.toString());
            }
            else 
               self.data("-");
            
            self.widgetApi.fitText();
        }
        else if (keywordId === self.widget.configuration.temperatureSet.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(self.precision);
               self.data(temp.toString());
            }
            else 
               self.data("-");
            
            self.widgetApi.fitText();
        } else if (keywordId === self.widget.configuration.heating.keywordId) {
           
            //it is the right device
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(self.precision);
               self.data(temp.toString());
            }
            else 
               self.data("-");
            
            self.widgetApi.fitText();
        }
    };
};