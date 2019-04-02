widgetViewModelCtor =

/**
 * Create a State Display ViewModel
 * @constructor
 */
function stateDisplayViewModel() {
   
    //observable data
    this.data = ko.observable("-");
    this.pluginInstanceType = "";
    this.typeInfoName = "";
    
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
       var defferedConfigurationChangedFinished = new $.Deferred();
        
        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);
		  self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 		
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        self.widgetApi.registerAdditionalInformation(["pluginId", "typeInfo"]);
    }

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        
        if (keywordId !== parseInt(self.widget.configuration.device.keywordId))
           return;
        
        if (!isNullOrUndefinedOrEmpty(data.typeInfo))
           self.typeInfoName = data.typeInfo.name;
       
        if (!isNullOrUndefinedOrEmpty(data.pluginId)){
           self.widgetApi.getPluginInstanceInformation(data.pluginId)
            .done(function (pluginInstance) {
               self.pluginInstanceType = pluginInstance.type;
               if (data.value !==""){
                  var translatedEnumValue = $.t("plugins." + self.pluginInstanceType + ":enumerations." + self.typeInfoName + ".values." + data.value, 
                  { defaultValue:data.value} );
                  
                  self.data(translatedEnumValue);
                  self.widgetApi.fitText();
               }
               else 
                  self.data("-");                     
            });
        }
     
      //it is the right device
      if (data.value !=="" && !isNullOrUndefinedOrEmpty(self.pluginInstanceType)){         
         var translatedEnumValue = $.t("plugins." + self.pluginInstanceType + ":enumerations." + self.typeInfoName + ".values." + data.value, 
         { defaultValue:data.value} );
         self.data(translatedEnumValue);
      }
      else 
         self.data("-");
    };
};