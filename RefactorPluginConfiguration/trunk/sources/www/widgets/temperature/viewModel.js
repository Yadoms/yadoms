widgetViewModelCtor =

/**
 * Create a Temperature ViewModel
 * @constructor
 */
function TemperatureViewModel() {
   //observable data
   this.data = ko.observable({ temperature: 24, rssi: 80 });

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
    * @param data data to dispatch
    */
   this.dispatch = function(data) {
      alert("dispatch temperature " + data);
   };
};
