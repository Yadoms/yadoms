function TemperatureViewModel() {
    this.data;

}

TemperatureViewModel.prototype.dispatch = function(data) { alert("dispatch temperature " + data);};

widgetViewModel = new TemperatureViewModel();
widgetViewModel.data = ko.observable({ temperature: 24, rssi: 80 });