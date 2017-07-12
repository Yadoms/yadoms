widgetViewModelCtor =

/**
 * Create a Rain Display ViewModel
 * @constructor
 */
function rainGaugeDisplayViewModel() {
    //observable data - values
    this.rate_1h = ko.observable("-");
    this.rate_24h = ko.observable("-");
    this.unit = ko.observable("");
    
    //observable data - titles
    this.title1 = ko.observable("");
    this.title2 = ko.observable("");
    
    //observable data - last receive information
    this.shouldBeVisible = ko.observable(false);
    this.lastReceiveDate = ko.observable("");    
    this.serverTime = null;
    this.deferredTime = null;

    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
       
       var self = this;
       
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: true
        });
      
      // Display traductions
      self.title1($.t("widgets/rain-gauge:title1"));
      self.title2($.t("widgets/rain-gauge:title2"));      
      
      return this.widgetApi.askServerLocalTime(function (serverLocalTime) {
         self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
      });
    };
  
    this.getValues = function (dataNow, keywordId) {
       self = this;
          
       console.log ("1 : ", dataNow);
       
       // date - 1h
       date1h = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'hours').startOf('minute'));
       var deffered1h = RestEngine.getJson("rest/acquisition/keyword/" + keywordId + "/" + date1h);
       deffered1h.done(function (data) {
          
          if (dataNow!=0)
          {
             if (data.data !=="")
             {
                self.rate_1h( (dataNow - parseFloat(data.data[0].key)).toFixed(1).toString() );
             }
             else
                self.rate_1h("0");
          }
       else
          self.rate_1h("-");
          
          console.log ("2 : ", data);
          
          // date - 24h
          date24h = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'days').startOf('minute'));
          var deffered24h = RestEngine.getJson("rest/acquisition/keyword/" + keywordId + "/" + date24h);
          deffered24h.done(function (data) {
             console.log ("3 : ", data);
             
             if (dataNow!=0)
             {
                if (data.data !=="")
                {
                   self.rate_24h( (dataNow - parseFloat(data.data[0].key)).toFixed(1).toString() );
                }
                else
                   self.rate_24h("0");
             }
             else
                self.rate_24h("-");             
          });
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
        
       // the actual time
       var date = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
       
       self.shouldBeVisible(self.widget.configuration.dateDisplay);       
       
       // Retrieve the last value
       var deffered = RestEngine.getJson("rest/acquisition/keyword/" + self.widget.configuration.device.keywordId + "/lastvalue");       
       
       deffered.done(function (data) {
          
          console.log ("data", data);
          // TODO : Check the time of the last update and the time of the server.
          
          // initial refresh value
          if (data.data instanceof Array)
          {
             self.getValues(parseFloat(data.data[data.data.length-1].key), self.widget.configuration.device.keywordId);
          }
          else if (data.data !=="")
          {
             self.getValues(parseFloat(data.data), self.widget.configuration.device.keywordId);
          }
          else
          {
             self.rate_1h("-");
             self.rate_24h("-");
          }
          
          self.widgetApi.fitText();
       });
       //TODO : Do the failed !
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
               self.getValues(parseFloat(data.value), keywordId);
            }
            else
            {
               self.rate_1h("-");
               self.rate_24h("-");
            }
            
            if (self.shouldBeVisible())
            {
               if (data.date!=="")
                  self.lastReceiveDate(moment(data.date).calendar().toString());
               else
                  self.lastReceiveDate($.t("widgets/rain-gauge:NoAcquisition"));
            }            
            
            self.widgetApi.fitText();
        }
    };
};