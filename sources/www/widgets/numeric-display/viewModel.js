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

    //widget identifier
    this.widget = null;

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function (widget) {
        this.widget = widget;

        //we create the battery indicator
        WidgetApi.toolbar.addBatteryIcon(this.widget);
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
        WidgetApi.keyword.getInformation(self.widget.configuration.device.keywordId).done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        WidgetApi.keyword.registerKeywordAcquisitions(self.widget, self.widget.configuration.device.keywordId);

        //we fill the deviceId of the battery indicator
        WidgetApi.toolbar.configureBatteryIcon(self.widget, self.widget.configuration.device.deviceId);
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

            self.resizefont();
        }
    };

    this.resizefont = function () {
        self = this;
        
        switch (self.data().toString().length + self.unit().toString().length) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                self.fontSize(30);
                break;
            case 7:
                if (self.widget.width() <= 100)
                    self.fontSize(25);
                else
                    self.fontSize(30);
                break;
            case 8:
                if (self.widget.width() <= 100)
                    self.fontSize(20);
                else
                    self.fontSize(30);
                break;
            case 9:
                if (self.widget.width() <= 100)
                    self.fontSize(17);
                else
                    self.fontSize(30);
                break;
            case 10:
            case 11:
                if (self.widget.width() <= 100)
                    self.fontSize(15);
                else
                    self.fontSize(30);
                break;
            default:
                self.fontSize(25);
                break;
        }
    }

    this.resized = function () {
        var self = this;

        self.resizefont();
    };
};