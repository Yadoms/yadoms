widgetViewModelCtor =

   /**
    * Create a teleInfoForecast ViewModel
    * @constructor
    */
      function teleInfoForecastViewModel() {
		  
          //observable data
          this.bg_color = ko.observable("");

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
             self = this;
             
             //we configure the toolbar
             self.widgetApi.toolbar({
                 activated: true,
                 displayTitle: true,
                 batteryItem: true
             });
          };

          /**
           * ConfigurationChanged method
           * @param none
           */          
          this.configurationChanged = function () {
              var self = this;
              var readOnlyMode=false;
              var arrayOfDeffered = [];

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              // registration of the keyword
              if  (!isNullOrUndefined(this.widget.configuration.device)) {
                self.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);                 
              }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;
              
              if (!isNullOrUndefined(this.widget.configuration)){

                 //Check if first device
                 if (!isNullOrUndefined(this.widget.configuration.device)) {
                    
                      if (keywordId === this.widget.configuration.device.keywordId) {
                         
                          //data.value
                          if (data.value === "RED")
                          {
                             self.bg_color("red");
                          }else if (data.value === "BLUE")
                          {
                             self.bg_color("blue");
                          }else if (data.value === "WHITE")
                          {
                             self.bg_color("white");
                          }else
                          {
                             self.bg_color("grey");
                          }
                      }
                 }
              }
          };
      };