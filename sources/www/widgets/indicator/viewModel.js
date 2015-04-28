widgetViewModelCtor =

   /**
    * Create a Indicator ViewModel
    * @constructor
    */
      function IndicatorViewModel() {

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
      this.initialize = function(widget) {
         this.widget = widget;
      };

      this.configurationChanged = function() {
         //we update the kind observable property

         try {
            this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
         }
         catch(err) {}

         try {
            this.icon("glyphicon " + this.widget.configuration.icon);
         }
         catch(err) {}

         //we ask for device information
         try {
            var self = this;
            DeviceManager.get(this.widget.configuration.device.deviceId, function (device) {
                  self.indicatorText(decodeURIComponent(device.friendlyName));
            });
            // Get the capacity of the keyword
            KeywordManager.get(this.widget.configuration.device.keywordId, function(keyword) {
               self.capacity = keyword.capacityName;
            });
         }
         catch(err) {}
      };

      /**
       * New acquisition handler
       * @param searchedDevice Device on which new acquisition was received
       * @param data Acquisition data
       */
      this.onNewAcquisition = function(device, data) {
         var self = this;
         try {
            if (device == this.widget.configuration.device) {
               //it is the right device
               // Adapt for dimmable or switch capacities
               self.command(parseInt(data.value) != 0 ? "1" : "0");
            }
         }
         catch (err) {}
      };

      this.getDevicesForAcquisitions = function() {
         var result = [];

         try {
            result.push(this.widget.configuration.device);
         }
         catch (err) {}

         return result;
      }

   };
