/**
 * Create a Moon ViewModel
 * @constructor
 */
widgetViewModelCtor = function moonViewModel() {

    //observable data
    this.illuminatedMoonValue = ko.observable(0).extend({ numeric: 1 });
    this.iconText = ko.observable("");

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        this.widgetApi.toolbar({
            activated: false
        });
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
            if (obj) {
                //parse as integers
                var illuminatedMoon = parseInt(obj.IlluminatedMoon || 0);
                var dayOfMoon = parseInt(obj.DayOfMoon || 0);
                
                //we use a font that display moon phases using A to Z letter
                //A - N increase light of moon and M-Z decrease light, '0' display an empty moon and '1' a full moon
                //dayOfMoon goes from 0 to 30
                //so we make following association
                // 0  1 ----- 13  14  15  16 -------- 29  30
                //'0' dayOfMoon   '@' '@' '@' dayOfMoon  '0'

                //update value binding
                self.illuminatedMoonValue(illuminatedMoon);
                var text = "";
                if (dayOfMoon === 0)
                    text = "0";
                else if (dayOfMoon < 14)
                    text = String.fromCharCode(("A").charCodeAt(0) + dayOfMoon - 1);
                else if ((dayOfMoon >= 14) && (dayOfMoon <= 16))
                    text = "@";
                else if (dayOfMoon < 30)
                    text = String.fromCharCode(("A").charCodeAt(0) + dayOfMoon - 1 - 3);

                self.iconText(text);

            }
        }
    }

    this.configurationChanged = function () {
       var self = this;
        
       //we register keyword new acquisition
       self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
	   
		//we register keyword for get last value at web client startup
		self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 
    };
};