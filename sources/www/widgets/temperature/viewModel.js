widgetViewModelCtor =

/**
 * Create a Temperature ViewModel
 * @constructor
 */
function TemperatureViewModel() {
   //observable data
   this.temperature = ko.observable(25);
   this.battery = ko.observable(100);

   //widget identifier
   this.widget = null;

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
    * @param deviceId
    */
   this.dispatch = function(deviceId, data) {
      var self = this;
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         if (deviceId == this.widget.configuration.device) {
            //it the good device
            //we browse the list of keywords values
            $.each(data, function(keywordIndex, keyword) {
               switch(keyword.key) {
                  case "temp" :
                     self.temperature(keyword.value);
                     break;
                  case "battery" :
                     self.battery(keyword.value);
                     break;
               }
            });
         }
      }
   };

   this.getDevicesToListen = function() {
      var result = [];
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         result.push(this.widget.configuration.device);
      }
      return result;
   }

};
