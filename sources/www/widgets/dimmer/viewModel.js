/**
 * Create a Dimmer ViewModel
 * @constructor
 */
widgetViewModelCtor = function DimmerViewModel() {
   

   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");


   //widget identifier
   this.step = ko.observable(0.1).extend({ numeric: 1 });


   this.commandClickPlus = function () {
      this.commandClick(this.step());

   }
   this.commandClickMinus = function() {
      this.commandClick(-this.step());
   }

   this.commandClick = function (value) {
      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         this.data(this.data() + value);
         KeywordManager.sendCommand(this.widget.configuration.device.keywordId, this.data().toString());
      }
      this.widgetApi.find(".widget-api-textfit").fitText();
   };



   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function () {
      var self = this;
      self.widgetApi.loadCss("widgets/dimmer/widget.css")
      .done(function () {
         self.widgetApi.find(".widget-api-textfit").fitText();
      });

      //we create the battery indicator
      this.widgetApi.toolbar.addBatteryIconToWidget();
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
      self.widgetApi.toolbar.configureBatteryIcon(self.widget.configuration.device.deviceId);

      //Read the step
      self.step(self.widget.configuration.StepValue);
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
         self.data(parseFloat(data.value));
         self.widgetApi.find(".widget-api-textfit").fitText();
      }
   };
};