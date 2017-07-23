widgetViewModelCtor =

/**
 * Create a Numeric Display ViewModel
 * @constructor
 */
function numericDisplayViewModel() {
    //observable data
    this.data = ko.observable("-");
    this.unit = ko.observable("");
    this.shouldBeVisible = ko.observable(false);
    this.lastReceiveDate = ko.observable("");

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
        self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);
        
        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        
        try{
           self.shouldBeVisible(parseBool(self.widget.configuration.dateDisplay));
        }
        catch(error)
        {
           self.shouldBeVisible(false);
           console.warn (error);
        }
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
            if (data.value !=="")
            {
               var temp = parseFloat(data.value).toFixed(1);
               self.data(temp.toString());
            }
            else 
               self.data("-");
            
            self.widgetApi.fitText();
            
            if (self.shouldBeVisible())
            {
               if (data.date!=="")
                  self.lastReceiveDate(moment(data.date).calendar().toString());
               else
                  self.lastReceiveDate($.t("widgets/numeric-display:NoAcquisition"));
            }
        }
    };
};