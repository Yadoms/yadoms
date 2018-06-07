widgetViewModelCtor =

/**
 * Create a State Display ViewModel
 * @constructor
 */
function stateDisplayViewModel() {
   
    //observable data
    this.data = ko.observable("-");
    this.keyword = null;
    this.pluginInstanceType="";
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
       var defferedPluginInstance = new $.Deferred();
       var defferedConfigurationChangedFinished = new $.Deferred();
       var arrayOfDeffered = [];
        
        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
	   
		//we register keyword for get last value at web client startup
		self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 		
		
       //we fill the deviceId of the battery indicator
       self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
       
      //we get the unit of the keyword
      var defferedKeywordInformation = self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId);
      arrayOfDeffered.push(defferedKeywordInformation);
      defferedKeywordInformation
      .done(function (keyword) {
         self.keyword = keyword;
      });
      
      //we get the unit of the keyword
      arrayOfDeffered.push(defferedPluginInstance);
      self.widgetApi.getDeviceInformation(self.widget.configuration.device.deviceId)
      .done(function (device) {
         self.widgetApi.getPluginInstanceInformation(device.pluginId)
         .done(function (pluginInstance) {
            self.pluginInstanceType = pluginInstance.type;
            defferedPluginInstance.resolve();
         })
         .fail(function (error) {
            defferedPluginInstance.reject();
         });         
      })
      .fail(function (error) {
         notifyError(error);
      });
      
      $.when.apply(arrayOfDeffered)
      .done(function () {
         defferedConfigurationChangedFinished.resolve();
      })
      .fail(function (error) {
         defferedConfigurationChangedFinished.reject();
      });
      
      return defferedConfigurationChangedFinished.promise();
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;

        //
        // self.keyword!=undefined
        // Sometimes onNewAcquisition arrive before the end of the configurationChanged by websocket
        //
        
        if (keywordId === self.widget.configuration.device.keywordId && !isNullOrUndefined(self.keyword) && !isNullOrUndefinedOrEmpty(self.pluginInstanceType)) {
            //it is the right device
            if (data.value !==""){
               var translatedEnumValue = $.t("plugins." + self.pluginInstanceType + ":enumerations." + self.keyword.typeInfo.name + ".values." + data.value, 
               { defaultValue:data.value} );
               self.data(translatedEnumValue);
            }
            else 
               self.data("-");
        }
    };
};