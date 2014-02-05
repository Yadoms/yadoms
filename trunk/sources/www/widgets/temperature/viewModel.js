function TemperatureViewModel() {
    this.data;
    this.$widget;
    this.informations;

    this.initialized = function($widget, informations) {
        this.$widget = $widget;
        this.informations = informations;
    }
}

TemperatureViewModel.prototype.dispatch = function(data) { alert("dispatch temperature " + data);};

widgetViewModel = new TemperatureViewModel();
widgetViewModel.data = ko.observable({ temperature: 24, rssi: 80 });