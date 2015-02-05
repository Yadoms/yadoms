widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
      function SwitchViewModel() {
      //observable data
      this.command = ko.observable(1);

      //widget identifier
      this.widget = null;

      this.kind = ko.observable("simple");
      this.icon = ko.observable("");

      this.switchText = ko.observable("");
      
      this.capacity = null;

      this.showDeviceName = ko.observable(true);

      this.commandClick = function(newState) {
         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device)) && (!isNullOrUndefined(this.capacity))) {
            var cmd = null;
            switch (this.capacity) {
               case "dimmable": cmd = newState == 1 ? 100 : 0; break;
               case "event": cmd = 1; break;
               default: cmd = newState; break;
            }
            $.ajax({
               type: "POST",
               url: "/rest/device/keyword/" + this.widget.configuration.device.keywordId + "/command",
               data: cmd.toString(),
               contentType: "application/text; charset=utf-8",
               dataType: "json"
            })
               .done(function(data) {
                  //we parse the json answer
                  if (data.result != "true")
                  {
                     notifyError($.t("switch:errorDuringSendingCommand"), JSON.stringify(data));
                  }
               })
               .fail(function() {notifyError($.t("switch:errorDuringSendingCommand")); });
         }
      };

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function(widget) {
         this.widget = widget;
      };

      this.configurationChanged = function() {
         //we update the kind observable property

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         if (!isNullOrUndefined(this.widget.configuration.kind)) {
            this.kind(this.widget.configuration.kind.activeSection);
            if (this.widget.configuration.kind.content.pushButton.radio) {
               this.icon("glyphicon " + this.widget.configuration.kind.content.pushButton.content.icon);
            }
         }

         if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
            this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
         }

         //we ask for device information
         if ((!isNullOrUndefined(this.widget.configuration.device)) && (!isNullOrUndefined(this.widget.configuration.device.deviceId))) {
            var self = this;
            DeviceManager.get(this.widget.configuration.device.deviceId, function (device) {
               self.switchText(decodeURIComponent(device.friendlyName));
            });
            // Get the capacity of the keyword
            KeywordManager.get(this.widget.configuration.device.keywordId, function(keyword) {
               self.capacity = keyword.capacityName;
            });
         }
      };

      /**
       * Dispatch the data to the viewModel
       * @deviceId device identifier which make the values
       * @param data data to dispatch
       * @param device
       */
      this.dispatch = function(device, data) {
         var self = this;
         if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
            if (device == this.widget.configuration.device) {
               //it is the good device
               if (this.capacity == "event")
                  self.command(0);
               else
                  // Adapt for dimmable or switch capacities
                  self.command(parseInt(data.value) != 0 ? "1" : "0");
            }
         }
      };

      this.getDevicesToListen = function() {
         var result = [];

         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
            result.push(this.widget.configuration.device);
         }
         return result;
      }

   };
