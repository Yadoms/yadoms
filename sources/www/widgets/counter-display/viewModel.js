widgetViewModelCtor =

/**
 * Create a Counter Display ViewModel
 * @constructor
 */
function counterDisplayViewModel() {
   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");
   this.fontSize = ko.observable(20);

   this.minimumIntegerDigit = 9;
   this.odometer = null;

  /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function () {
      window.odometerOptions = {
         auto: false // Don't automatically initialize everything with class 'odometer'
      };

      // For each odometer, initialize with the theme passed in:
      this.odometer = new Odometer(
      {
         el: this.widgetApi.find(".odometer")[0],
         format: '(.ddd)',
         value: 0,
         theme: 'car',
         duration: 1000,
         selector: '.my-numbers',
         minimumIntegerDigit: this.minimumIntegerDigit
      });

      //we create the battery indicator
      this.widgetApi.addBatteryIconToWidget();
   };


   this.configurationChanged = function () {

      var self = this;

      //we register keyword new acquisition
      self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);

      //we fill the deviceId of the battery indicator
      self.widgetApi.toolbar.configureBatteryIcon(self.widget.configuration.device.deviceId);

      //we get the unit of the keyword
      KeywordManager.get(self.widget.configuration.device.keywordId)
      .done(function (keyword) {
         // Read the unit
         self.unit($.t(keyword.units));

         //Set the Unit for odometer
         self.odometer.SetUnit(self.unit());

         self.resizefont();
      });
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
         self.widgetApi.find(".odometer").html(data.value);
      }
   };

   this.resizefont = function () {
      var self = this;

      //Compute the font-size needed.
      self.fontSize(((this.widget.width() - 9 - ((this.minimumIntegerDigit / 3) - 1) * 8.33) / (this.minimumIntegerDigit + self.unit().length)) * 1.11);

      //Change the font-size value
      self.widgetApi.find(".odometer").css({ "font-size": self.fontSize() + "px", "margin": "auto", "display": "table" });
   }

   this.resized = function () {
      var self = this;

      self.resizefont();
   };

};