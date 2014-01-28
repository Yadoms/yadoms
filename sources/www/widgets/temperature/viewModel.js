function TemperatureViewModel() {
    this.data;
}

TemperatureViewModel.prototype.dispatch = function(data) { alert("dispatch temperature " + data);};



var temperatureViewModel = new TemperatureViewModel();
temperatureViewModel.data = ko.observable({ temperature: 24, rssi: 80 });