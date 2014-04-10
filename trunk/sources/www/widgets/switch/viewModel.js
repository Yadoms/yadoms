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

   this.commandClick = function(newState) {
     //alert(this.command());

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
               notifyError("Error during sending command");
               return;
            }

         })
         .fail(function() {notifyError("Error during sending command"); });
   };

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   /**
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
    */
   this.dispatch = function(deviceId, data) {
      var self = this;
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         if (deviceId == this.widget.configuration.device) {
            //it the good device
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

      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         result.push(this.widget.configuration.device);
      }
      return result;
   }

};
