widgetViewModelCtor =

/**
 * Create a Switch ViewModel
 * @constructor
 */
function SwitchViewModel() {
   //observable data
   this.command = ko.observable("on");

   //widget identifier
   this.widget = null;

   this.kind = ko.observable("simple");

   this.switchText = ko.observable("");

   this.showDeviceName = ko.observable(true);

   this.commandClick = function(newState) {
      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         $.ajax({
            type: "POST",
            url: "/rest/device/keyword/" + this.widget.configuration.device.keywordId + "/command",
            data: JSON.stringify({ command: newState }),
            contentType: "application/json; charset=utf-8",
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
      if ((isNullOrUndefined(this.widget)) || (isNullOrUndefined(this.widget.configuration)))
         return;

      if (!isNullOrUndefined(this.widget.configuration.kind)) {
         this.kind(this.widget.configuration.kind);
      }

      if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
         this.showDeviceName(this.widget.configuration.showDeviceName);
      }

      //we ask for device information
      if (!isNullOrUndefined(this.widget.configuration.device)) {
         var self = this;
         $.getJSON("rest/device/" + this.widget.configuration.device)
            .done(function( data ) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("switch:errorDuringGettingDeviceInformation"), JSON.stringify(data));
                  return;
               }
               self.switchText(data.data.name);
            })
            .fail(function() {notifyError($.t("switch:errorDuringGettingDeviceInformation"));});
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
            self.command(data.value);
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
