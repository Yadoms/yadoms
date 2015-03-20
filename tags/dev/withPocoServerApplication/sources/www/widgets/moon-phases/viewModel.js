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
 * Create a Smiley ViewModel
 * @constructor
 */
function MoonPhasesViewModel() {

   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   
   //JMB : Essai avec la lune - Pour le moment en fixe
   this.photoName = ko.observable("widgets/moon-phases/images/moon01.png");

   self.unit = "%";

   /**
    * Widget identifier
    */
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

      if (device == self.widget.configuration.device) 
      {
         self.data(data.value);
      }
      if (device == self.widget.configuration.text) 
      {
         //It's a time (Ex:264:00:00). We split it 
         var res = data.value.split(":");
        
         //Hours are used to calculate the image number
         self.photoName ( "widgets/moon-phases/images/moon" + parseInt(res[0])/24 + ".png" );
         console.debug( self.photoName );
      }
   };

   this.getDevicesToListen = function() {
      var result = [];

      //Add the pourcentage value
      result.push(this.widget.configuration.device);

      //Add the day value
      result.push(this.widget.configuration.text);
      return result;
   }

};
