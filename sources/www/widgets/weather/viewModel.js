/**
 * Create a Weather ViewModel
 * @constructor
 */
widgetViewModelCtor = function weatherViewModel() {

    var self = this;
    //observable data
    this.data = ko.observable("");
    this.city = ko.observable("");
    this.temperature = ko.observable("");

    this.condition = ko.observable("sunny");

    this.conditionClass = ko.computed(function () {
        return "wi wi-wu-" + self.condition().toLowerCase();
    });

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;
        var d = new $.Deferred();

        // create the chart
        self.$chart = self.widgetApi.find("div.container");
        self.widgetApi.loadCss("libs/weather-icons/css/weather-icons.min.css").done(function() {

            //we configure the toolbar
            self.widgetApi.toolbar({
                activated: false
            });

            d.resolve();
        })
        .fail(d.reject);
        
        return d.promise();
    };

    /**
     * New acquisition handler
     * @param keywordId keywordId on which new acquisition was received
     * @param data Acquisition data
     */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        if (keywordId === self.widget.configuration.device.keywordId) {
            var obj = jQuery.parseJSON(data.value);

            //We only keep the city name
            var res = obj.city.split(",");
			   
            if (res.length >=2)
               self.city(res[1]);
            else
               self.city(res[0]);            

            self.condition(obj.Conditions.WeatherCondition);
            self.temperature(obj.Conditions.Temp + $.t(obj.Units.temperature));
        }
    };

    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;

        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
	   
		//we register keyword for get last value at web client startup
		self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 
    }
};