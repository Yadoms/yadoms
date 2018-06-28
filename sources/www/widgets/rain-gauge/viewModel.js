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
    
    //observable data - last receive information
    this.shouldBeVisible = ko.observable(false);
    this.lastReceiveDate = ko.observable($.t("widgets.rain-gauge:loadingData"));
    this.serverTime = null;
    this.acquisitionData = [];
    
    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
       var self = this;
       var d = new $.Deferred();
       var arrayOfDeffered = [];
       
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: true
        });
      
      arrayOfDeffered.push(widgetApi.askServerLocalTime(function (serverLocalTime) {
                              self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
                          }));
      
      $.when.apply($,deferredArray)
      .done(function(){
         d.resolve();
      })
      .fail(d.reject);
      
      return d.promise();
    };
  
    this.getValues = function (keywordId) {
       var self = this;
       
       // Retrieve last values
       var date24h = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'days').startOf('minute'));
       var deffered2 = RestEngine.getJson("rest/acquisition/keyword/" + keywordId + "/" + date24h);
       
       deffered2
       .done(function (data) {
          if (data.data !==""){
             self.acquisitionData.splice(0, self.acquisitionData.length);
             Array.prototype.push.apply(self.acquisitionData, data.data);
          }
          
          // treat the tab
          self.analyzeBuffer(keywordId);
       })
      .fail(function(error) {
         var message = "Fail to get server data : " + error;
         console.warn(message);
      });
      
      return deffered2;
    };
       
    this.analyzeBuffer = function (keywordId) {
       var self = this;       
       
       // When all values are outside the range, only 1 value is sent.
       if (self.acquisitionData.length == 1)
       {
          // We check if this value is up to date. We delete it if not
          if (self.serverTime - DateTimeFormatter.isoDateToDate(self.acquisitionData[0].date) > 86400000)
             self.acquisitionData.shift();
       }       
       
       //clean old values at the begin of the tab
       if (self.acquisitionData.length != 0)
       {
          var index = self.acquisitionData.length-1;
       
          // retrieve all values
          var lastValue = self.acquisitionData[index];
          var last24h = self.acquisitionData[0];
          
          self.rate_24h( (parseFloat(lastValue.key) - parseFloat(last24h.key) ).toFixed(1).toString());
          
          // Retrieve 1h ago
          
          while (index>=0 && (self.serverTime - DateTimeFormatter.isoDateToDate(self.acquisitionData[index].date) < 3600000) ){
             if (index >= 0) index--;
          }
          
          if (index != -1)
          {
             var last1h = self.acquisitionData[index];
             self.rate_1h( (parseFloat(lastValue.key) - parseFloat(last1h.key) ).toFixed(1).toString());
          }
          else
             self.rate_1h("-");
       }
       else
       {
          self.rate_1h("-");
          self.rate_24h("-");
       }
       
       // treat the time  
       if (self.shouldBeVisible())
       {
         if (self.acquisitionData.length!=0)
            self.lastReceiveDate(DateTimeFormatter.isoDateToDate(lastValue.date).calendar().toString());
         else
            self.lastReceiveDate($.t("widgets.rain-gauge:NoAcquisition"));
       }
    };
  
    this.configurationChanged = function () {
        var self = this;
        var arrayOfDeffered = [];
        var d = new $.Deferred();
        
        //we get the unit of the keyword
        var deffered1 = self.widgetApi.getKeywordInformation(self.widget.configuration.device.keywordId);
        arrayOfDeffered.push(deffered1);
        
        deffered1
        .done(function (keyword) {
            self.unit($.t(keyword.units));
        });

        //we register keyword new acquisition
        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);
        
        //we fill the deviceId of the battery indicator
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        self.shouldBeVisible(self.widget.configuration.dateDisplay);
        arrayOfDeffered.push(getValues(self.widget.configuration.device.keywordId));
      
      $.when.apply($, arrayOfDeffered) // The first failing array fail the when.apply
      .done(d.resolve)
      .fail(d.reject);
      
      return d.promise();
    }

    this.onTime = function (serverLocalTime) {
         var self = this;
         
         self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
         
         while(self.acquisitionData.length>0){
          if (self.serverTime - DateTimeFormatter.isoDateToDate(self.acquisitionData[0].date) > 86400000){
             self.acquisitionData.shift();
          }
          else
             break;
       }         
     };
     
    /**
    * event wakeUp (after a reconnexion, wake down of a smartphone, tab, ...)
    */    
    this.onWakeUp = function () {
       var self = this;
       
       self.widgetApi.askServerLocalTime(function (serverLocalTime) {
          self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
       }).done(function(data) {
          getValues(self.widget.configuration.device.keywordId);
       })
       .fail(function(error) {
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
            //it is the right device
            if (data.value !==""){
               if (self.acquisitionData.length!=0){
                  if (data.date != DateTimeFormatter.isoDateToDate(self.acquisitionData[self.acquisitionData.length-1].date)) {
                     self.acquisitionData.push({date: DateTimeFormatter.dateToIsoDate(data.date), key: data.value});
                  }
               }
               else{
                  self.acquisitionData.push({date: DateTimeFormatter.dateToIsoDate(data.date), key: data.value});
               }
            }
            self.analyzeBuffer(keywordId);
        }
    };
};