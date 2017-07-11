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
        
      this.widgetApi.askServerLocalTime(function (serverLocalTime) {
         self.serverTime = DateTimeFormatter.isoDateToDate (serverLocalTime);
      });
      
      // Display traductions
      this.title1($.t("widgets/rain-gauge:title1"));
      this.title2($.t("widgets/rain-gauge:title2"));      
    };
/*
        dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
    dateFrom = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'hours').startOf('minute'));
    //we request all data
    timeBetweenTwoConsecutiveValues = undefined;
    isSummaryData = false;
    break;
default:
case "DAY":
    dateTo = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf('hour').subtract(1, 'seconds'));
    dateFrom = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'days').startOf('hour'));
    //we request hour summary data
    prefixUri = "/hour";
    timeBetweenTwoConsecutiveValues = 1000 * 3600;
    isSummaryData = true;
  

var deffered = RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + dateFrom + "/" + dateTo);
var deffered = RestEngine.getJson("rest/acquisition/keyword/" + device.content.source.keywordId + prefixUri + "/" + date);
arrayOfDeffered.push(deffered);
deffered.done(function (data) {
*/
  
    this.getValues = function (keywordId) {
       self = this;
       
       // the actual time
       var date = DateTimeFormatter.dateToIsoDate(moment(self.serverTime));
       
       // Retrieve the last value
       var deffered = RestEngine.getJson("rest/acquisition/keyword/" + /*device.content.source.*/keywordId + "/" + date);
       
       deffered.done(function (data) {
          
          console.log ("1 : ", data);
          
          // date - 1h
          date1h = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'hours').startOf('minute'));
          var deffered1h = RestEngine.getJson("rest/acquisition/keyword/" + /*device.content.source.*/keywordId + "/" + date1h);
          deffered1h.done(function (data) {
             
             console.log ("2 : ", data);
             
             // date - 24h
             date24h = DateTimeFormatter.dateToIsoDate(moment(self.serverTime).subtract(1, 'days').startOf('minute'));
             var deffered24h = RestEngine.getJson("rest/acquisition/keyword/" + /*device.content.source.*/keywordId + "/" + date24h);
             deffered24h.done(function (data) {
                console.log ("3 : ", data);
                
                self.rate_1h("0"/*temp.toString()*/);
                self.rate_24h("0"/*temp.toString()*/);
             });
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
               var temp = parseFloat(data.value).toFixed(1);
               self.getValues(keywordId);
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