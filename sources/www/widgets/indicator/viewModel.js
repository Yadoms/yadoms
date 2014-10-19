widgetViewModelCtor =

   /**
    * Create a Indicator ViewModel
    * @constructor
    */
      function IndicatorViewModel() {

      //observable data
      this.command = ko.observable(100);

      //observable data
      this.icon = ko.observable("");

      //widget identifier
      this.widget = null;

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
         }
         catch(err) {}
      };

      /**
       * Dispatch the data to the viewModel
       * @deviceId device identifier which make the values
       * @param data data to dispatch
       * @param device
       */
      this.dispatch = function(device, data) {
         var self = this;
         try {
            if (device == this.widget.configuration.device) {
               //it is the good device

               self.command(parseInt(data.value));
            }
         }
         catch (err) {}
      };

      this.getDevicesToListen = function() {
         var result = [];

         try {
            result.push(this.widget.configuration.device);
         }
         catch (err) {}

         return result;
      }

   };
