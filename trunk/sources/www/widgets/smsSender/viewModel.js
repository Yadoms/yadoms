widgetViewModelCtor =

/**
 * Create a smsSender ViewModel
 * @constructor
 */
function SmsSenderViewModel() {
   //observable data
   this.to = ko.observable("0628084514");//TODO virer ?
   this.body = ko.observable("message");//TODO virer ?
   
   //widget identifier
   this.widget = null; //TODO à quoi ça sert ?

   this.kind = ko.observable("simple");//TODO à quoi ça sert ?

   this.smsSenderText = ko.observable("text");//TODO à quoi ça sert ?

   this.showDeviceName = ko.observable(true);//TODO à quoi ça sert ?

   this.send = function() {
         console.debug("OUAIS !");
         console.debug("to=" + this.to);
         console.debug("body=" + this.body);
         var sms = new Object();
         sms.to = to;
         sms.body = body;
         console.debug(JSON.stringify(sms));
      if (!isNullOrUndefined(this.widget.configuration.device)) {
         
         console.debug("OUAIS !");
      
         var sms = new Object();
         sms.to = to;
         sms.body = body;
         
         $.ajax({
            type: "POST",
            url: "/rest/device/" + this.widget.configuration.device + "/command",
            data: JSON.stringify(sms),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("smsSender:errorDuringSending"), JSON.stringify(data));
               }
            })
            .fail(function() {notifyError($.t("smsSender:errorDuringSending")); });
      }
   };

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   //TODO à virer ?
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
                  notifyError($.t("smsSender:errorDuringGettingDeviceInformation"), JSON.stringify(data));
                  return;
               }
               self.smsSenderText(data.data.name);
            })
            .fail(function() {notifyError($.t("smsSender:errorDuringGettingDeviceInformation"));});
      }
   };

   /**
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
    * @param deviceId
    */
   this.dispatch = function(deviceId, data) {
      var self = this;
      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         if (deviceId == this.widget.configuration.device) {
            //it is the good device
            //we browse the list of keywords values
            //debugger;
            $.each(data, function(keywordIndex, keyword) {
               switch(keyword.key) {
                  case "send" :
                     self.send(keyword.value);
                     break;
               }
            });
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
