/**
 * Create a Temperature ViewModel
 * @constructor
 */
function TemperatureViewModel() {
   //observable data
   this.data = null;

   //widget identifier
   this.widget = null;
}
/**
 * Initialization method
 * @param page page where the widget is
 * @param widget widget class object
 */
TemperatureViewModel.prototype.initialize = function(page, widget) {
   this.widget = widget;
};

/**
 * Dispatch the data to the viewModel
 * @param data data to dispatch
 */
TemperatureViewModel.prototype.dispatch = function(data) {
   alert("dispatch temperature " + data);
};

/**
 * Widget ViewModel instantiation
 */
widgetViewModel = new TemperatureViewModel();
widgetViewModel.data = ko.observable({ temperature: 24, rssi: 80 });