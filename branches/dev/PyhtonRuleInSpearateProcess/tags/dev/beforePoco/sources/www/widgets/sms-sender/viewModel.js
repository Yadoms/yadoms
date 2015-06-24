widgetViewModelCtor =

/**
 * Create a sms-sender ViewModel
 * @constructor
 */
function SmsSenderViewModel() {
   //observable data
   this.to = ko.observable();
   this.body = ko.observable();
   
   //widget identifier
   this.widget = null;

   this.smsSenderText = ko.observable();

   this.send = function() {
      // Check that widget is configured, and to/body data are valid
      if ((!isNullOrUndefined(this.widget.configuration.device)) && (this.to()) && (this.body())) {

         var sms = new Object();
         sms.to = this.to();
         sms.content = this.body();
         sms.acknowledgment = 'true';
      
         $.ajax({
            type: "POST",
            url: "/rest/device/keyword/" + this.widget.configuration.device.keywordId + "/command",
            data: JSON.stringify(sms),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("sms-sender:errorDuringSending"), JSON.stringify(data));
               }
            })
            .fail(function() {notifyError($.t("sms-sender:errorDuringSending")); });
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
         $.getJSON("rest/device/" + this.widget.configuration.device.deviceId)
            .done(function( data ) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("sms-sender:errorDuringGettingDeviceInformation"), JSON.stringify(data));
                  return;
               }
               self.smsSenderText(data.data.name);
            })
            .fail(function() {notifyError($.t("sms-sender:errorDuringGettingDeviceInformation"));});
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
            self.send(data.value);
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
