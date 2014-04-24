widgetViewModelCtor =

/**
 * Create a Switch ViewModel
 * @constructor
 */
function SwitchViewModel() {
   //observable data
   //this.data = ko.observable({ temperature: 24, battery: 80 });

   this.command = ko.observable("on");

   //widget identifier
   this.widget = null;

   this.kind = ko.observable("simple");

   this.switchText = ko.observable("");

   this.showDeviceName = ko.observable(true);

   this.commandClick = function(newState) {
      if (!isNullOrUndefined(this.widget.configuration.device)) {
         $.ajax({
            type: "POST",
            url: "/rest/device/" + this.widget.configuration.device + "/command",
            data: JSON.stringify({ command: newState }),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError("Error during sending command", JSON.stringify(data));
                  return;
               }

            })
            .fail(function() {notifyError("Error during sending command"); });
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
                  //TODO : i18N
                  notifyError($.t("I18N"), JSON.stringify(data));
                  return;
               }
               self.switchText(data.data.name);

            })
            //TODO : i18N
            .fail(function() {notifyError($.t("I18N"));});
      }
   };

   /**
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
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
                  case "command" :
                     self.command(keyword.value);
                     break;
               };
            });
         }
      }
   };

   this.getDevicesToListen = function() {
      var result = new Array();

      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         result.push(this.widget.configuration.device);
      }
      return result;
   }

};
