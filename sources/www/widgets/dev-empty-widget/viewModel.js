/**
 * Create a dev-empty ViewModel
 * @constructor
 */

widgetViewModelCtor = function () {
    
    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;

        //toolbar API initialization
        self.widgetApi.toolbar({
            activated: false
        });
        
        /* Custom code here */
    };

    /**
    * Callback for a resized event
    */
    this.resized = function () {
        var width = this.widget.getWidth();
        var height = this.widget.getHeight();
        
        /* Custom code here */
    };

    /**
    * Callback for a configuration changed event
    */
    this.configurationChanged = function () {
        var self = this;

    if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
        return;

        /* Custom code here */
    }

    /**
    * Callback called every minute that received the time server
    * @param serverLocalTime the local Time of the server
    */
    
    this.onTime = function (serverLocalTime) {
       var self = this;
       
       /* Custom code here */
       
    };
    
    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        /* Custom code here */
    }         
};