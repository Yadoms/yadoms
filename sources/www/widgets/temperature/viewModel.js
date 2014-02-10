function TemperatureViewModel() {
   //observable data
   this.data;

   //page identifier
   this.parentPage;

   //widget identifier
   this.widget;

    this.initialize = function(page, widget) {
       this.parentPage = page;
       this.widget = widget;
    }
}

TemperatureViewModel.prototype.dispatch = function(data) { alert("dispatch temperature " + data);};

widgetViewModel = new TemperatureViewModel();
widgetViewModel.data = ko.observable({ temperature: 24, rssi: 80 });