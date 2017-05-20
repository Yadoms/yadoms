widgetViewModelCtor =

/**
 * Create a Clock ViewModel
 * @constructor
 */
function clockViewModel() {
   /**
   * Observable data
   */
   this.time = ko.observable("00:00");
   this.date = ko.observable("");

   var self = this;

   /**
   * Update the time onto the widget
   * @param target
   */
   this.updateTime = function(serverTime) {      
      serverTimeMoment = DateTimeFormatter.isoDateToDate(serverTime);
      self.time(serverTimeMoment.format("LT"));
      self.date(serverTimeMoment.format("LL"));
   };

   /**
   * Initialization method
   * @param widget widget class object
   */
   this.initialize = function () {

      self.widgetApi.toolbar({
         activated: false
      });
      
      self.widgetApi.askServerLocalTime(self.updateTime);
   };

   /**
   * Time event handler
   * @param serverLocalTime Time data
   */
   this.onTime = function (serverLocalTime) {
      self.updateTime(serverLocalTime);
   }
};
