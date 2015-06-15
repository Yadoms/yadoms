widgetViewModelCtor =

/**
 * Create a Numeric Display ViewModel
 * @constructor
 */
function NumericDisplayViewModel() {
   //observable data
   this.text = ko.observable("");
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");

   //widget identifier
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   this.configurationChanged = function() {
      this.text(this.widget.configuration.text);

      var self = this;

      //we get the unit of the keyword
      KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) {
         self.unit(keyword.units);
      });
   }
   
   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) {
      var self = this;

      if (device == self.widget.configuration.device) {
         //it is the good device
         self.data(data.value);
      }
   };

   this.getDevicesForAcquisitions = function() {
      var result = [];
      result.push(this.widget.configuration.device);

      return result;
   }

};
