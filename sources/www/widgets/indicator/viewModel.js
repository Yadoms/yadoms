/**
 * Create a Indicator ViewModel
 * @constructor
 */
widgetViewModelCtor = function IndicatorViewModel() {

   //observable data
   this.command = ko.observable(1);

   //observable data
   this.icon = ko.observable("");

   //widget identifier
   this.widget = null;

   this.capacity = null;

   //kind will be available with analog devices
   this.kind = ko.observable("digital");

   this.indicatorText = ko.observable("");

   this.showDeviceName = ko.observable(true);

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function (widget) {
      this.widget = widget;

      //we create the battery indicator
      this.widget.$toolbar.append("<div class=\"widget-toolbar-battery\" deviceId=\"\"></div>");
   };

   this.configurationChanged = function () {
      //we update the kind observable property
      var self = this;
      self.widget.ListenKeyword(this.widget.configuration.device.keywordId);

      //we fill the deviceId of the battery indicator
      self.widget.$toolbar.find(".widget-toolbar-battery").attr("deviceId", self.widget.configuration.device.deviceId);

      try {
         self.showDeviceName(parseBool(self.widget.configuration.showDeviceName));
      }
      catch (err) { }

      try {
         self.icon("glyphicon " + self.widget.configuration.icon);
      }
      catch (err) { }

      try {
         //we ask for device information
         if (this.widget.configuration.device && this.widget.configuration.device.deviceId) {
            DeviceManager.get(this.widget.configuration.device.deviceId)
            .done(function(device) {
               self.indicatorText(device.friendlyName);
            });
         }
         // Get the capacity of the keyword
         if (this.widget.configuration.device && this.widget.configuration.device.keywordId) {
            KeywordManager.get(this.widget.configuration.device.keywordId)
            .done(function(keyword) {
               self.capacity = keyword.capacityName;
            });
         }
      }
      catch (err) { }
   };

   /**
   * New acquisition handler
   * @param keywordId keywordId on which new acquisition was received
   * @param data Acquisition data
   */
   this.onNewAcquisition = function (keywordId, data) {
      var self = this;
      try {
         if (keywordId == this.widget.configuration.device.keywordId) {
            //it is the right device
            // Adapt for dimmable or switch capacities
            self.command(parseInt(data.value) != 0 ? "1" : "0");
         }
      }
      catch (err) { }
   };
};
