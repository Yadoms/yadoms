widgetViewModelCtor =

/**
 * Create a Numeric Display ViewModel
 * @constructor
 */
function numericDisplayViewModel() {
    //observable data
    this.data = ko.observable(0).extend({ numeric: 1 });
    this.unit = ko.observable("");
    this.fontSize = ko.observable(25);

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {

        //we create the battery indicator
       this.widgetApi.toolbar.addBatteryIconToWidget();
    };

    /**
     * FontSize Method for the size of the Data to display
     */

    this.fontStyleCSS = ko.computed(function () {
        return { "fontSize": this.fontSize() + "px" };
    }, this);

    this.configurationChanged = function () {

        var self = this;
        /*
        //we get the unit of the keyword
        KeywordManager.get(self.widget.configuration.device.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        this.widget.ListenKeyword(this.widget.configuration.device.keywordId);

        //we fill the deviceId of the battery indicator
        ToolbarApi.configureBatteryIcon(this.widget, self.widget.configuration.device.deviceId);
        */
        self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);

        //we fill the deviceId of the battery indicator
        self.widgetApi.toolbar.configureBatteryIcon(self.widget.configuration.device.deviceId);
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId == self.widget.configuration.device.keywordId) {
            //it is the right device
           self.data(data.value);
           self.widgetApi.find(".widget-api-textfit").fitText();
        }
    };

    this.resized = function () {
    };
};