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
      var self = this;
      var d = new $.Deferred();
      var arrayOfDeffered = [];
      self.widgetApi.toolbar({
         activated: false
      });
      
      return self.widgetApi.askServerLocalTime(self.updateTime);
   };

   /**
   * Time event handler
   * @param serverLocalTime Time data
   */
   this.onTime = function (serverLocalTime) {
      var self = this;
      self.updateTime(serverLocalTime);
   }
};
