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
   this.temperature = ko.observable(25).extend({ numeric: 1 });

   this.batteryStep = ko.observable(3).extend({ numeric: 0 });
   this.manageBatteryLevel = ko.observable(false);
   
   //widget identifier
   this.widget = null;

   this.batterylevelKeywordId = undefined;
   
   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   this.configurationChanged = function() {
      //we check if the widget manage battery level. If yes we will display this additionnal information
      //by default the battery level is no more managed
      this.batterylevelKeywordId = undefined;
      this.manageBatteryLevel(false);
      
      var self = this;
      
      $.getJSON("rest/device/" + this.widget.configuration.device.deviceId + "/get/batteryLevel")
            .done(function( data ) {
               //we parse the json answer
               if (data.result != "true")
               {
                  //TODO : i18n
                  notifyError($.t("ERROR"), JSON.stringify(data));
                  return;
               }
               if (data.data.keyword.length > 0) {
                  //the device has the capacity
                  self.manageBatteryLevel(true);
                  self.batterylevelKeywordId = data.data.keyword[0].id;
               }
            })
            .fail(function() {notifyError($.t("switch:errorDuringGettingDeviceInformation"));});
   }
   
   /**
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
    * @param deviceId
    */
   this.dispatch = function(device, data) {
      var self = this;
      if ((self.widget.configuration !== undefined) && (self.widget.configuration.device !== undefined)) {
         if (device == self.widget.configuration.device) {
            //it is the good device
            self.temperature(data.value);
         }
         else {
            if (!isNullOrUndefined(self.batterylevelKeywordId)) {
               if ((device.deviceId == self.widget.configuration.device.deviceId) && (device.keywordId == self.batterylevelKeywordId)) {
                  //we set the step associated to the battery level. There is 4 step (0-3)
                  self.batteryStep(Math.round(data.value / 33.3));
               }
            }
         }
      }
   };

   this.getDevicesToListen = function() {
      var result = [];
      if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
         result.push(this.widget.configuration.device);
         
         //we look if the battery level is managed
         if (!isNullOrUndefined(this.batterylevelKeywordId)) {
            result.push({deviceId: this.widget.configuration.device.deviceId, keywordId: this.batterylevelKeywordId});
         }
      }
      
      return result;
   }

};
