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

function twoDigit(number) {
  var twodigit = number >= 10 ? number : "0"+number.toString();
  return twodigit;
}

widgetViewModelCtor =

/**
 * Create a MoonPhases ViewModel
 * @constructor
 */
function MoonPhasesViewModel() {

   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   
   //Default value - This value is overwrite after
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
		 var obj = jQuery.parseJSON( data.value );
		 self.data ( parseInt( obj.IlluminatedMoon ) );
		 var res = obj.DayOfMoon;	
		
         //Hours are used to calculate the image number
         self.photoName ( "widgets/moon-phases/images/moon" + (parseInt(res)-1) + ".png" );
      }
   }
   
   this.configurationChanged = function() {
      var self = this;
  }	 

   this.getDevicesToListen = function() {
      var result = [];

      //Add the moon keyword
      result.push(this.widget.configuration.device);
	  
	  return result;
   }
};