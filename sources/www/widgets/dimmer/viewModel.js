/**
 * Create a Dimmer ViewModel
 * @constructor
 */
widgetViewModelCtor = function DimmerViewModel() {
   //observable data
   this.command = ko.observable(1).extend({ numeric: 1 });

   this.DisplayValue = ko.observable("");

   //widget identifier
   this.widget = null;

   this.step = ko.observable(0.1).extend({ numeric: 1 });
   this.DimmerText = ko.observable("");

   this.unit = ko.observable("");

   this.commandClick = function (value) {
      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         this.command(this.command() + value);
         KeywordManager.sendCommand(this.widget.configuration.device.keywordId, this.command().toString());
      }

      this.DisplayValue(this.command().toString());
   };

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function (widget) {
      this.widget = widget;
   };

   this.configurationChanged = function () {
      var self = this;

      if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
         return;

      this.widget.ListenKeyword(this.widget.configuration.device.keywordId);

      //Read the step
      self.step(self.widget.configuration.StepValue);

      if (self.widget.configuration.device && self.widget.configuration.device.keywordId) {
         //we get the unit of the keyword
         KeywordManager.get(self.widget.configuration.device.keywordId)
         .done(function (keyword) {
            self.unit(keyword.units);
            self.DimmerText(keyword.friendlyName);
         });
      }
   };

   /**
   * New acquisition handler
   * @param keywordId keywordId on which new acquisition was received
   * @param data Acquisition data
   */
   this.onNewAcquisition = function (keywordId, data) {
      var self = this;
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         if (keywordId === this.widget.configuration.device.keywordId) {
            self.command(parseFloat(data.value));
            self.DisplayValue(self.command());
         }
      }
   };
};