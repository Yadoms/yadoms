ko.extenders.numeric = function(target, precision) {
   //create a writeable computed observable to intercept writes to our observable
   var result = ko.computed({
      read: target,  //always return the original observables value
      write: function(newValue) {
         var current = target(),
            roundingMultiplier = Math.pow(10, precision),
            newValueAsNum = isNaN(newValue) ? 0 : parseFloat(+newValue),
            valueToWrite = Math.round(newValueAsNum * roundingMultiplier) / roundingMultiplier;

         //only write if it changed
         if (valueToWrite !== current) {
            target(valueToWrite);
         } else {
            //if the rounded value is the same, but a different value was written, force a notification for the current field
            if (newValue !== current) {
               target.notifySubscribers(valueToWrite);
            }
         }
      }
   }).extend({ notify: 'always' });

   //initialize with current value to make sure it is rounded appropriately
   result(target());

   //return the new computed observable
   return result;
};

widgetViewModelCtor =

/**
 * Create a Temperature ViewModel
 * @constructor
 */
function TemperatureViewModel() {
   //observable data
   this.temperature = ko.observable(25).extend({ numeric: 2 });
   this.battery = ko.observable(100).extend({ numeric: 0 });

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
   this.dispatch = function(device, data) {
      var self = this;
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         if (device == this.widget.configuration.device) {
            //it is the good device
            self.temperature(data.value);
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
