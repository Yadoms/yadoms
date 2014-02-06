function TemperatureViewModel() {
    this.data;
    this.$widgetNode;
    this.$grisdterWidgetNode;
    this.configuration;

    this.initialize = function($widgetNode, $gridsterWidgetNode, configuration) {
        this.$widgetNode = $widgetNode;
        this.$grisdterWidgetNode = $gridsterWidgetNode;
        this.configuration = configuration;
    }
}

TemperatureViewModel.prototype.dispatch = function(data) { alert("dispatch temperature " + data);};

widgetViewModel = new TemperatureViewModel();
widgetViewModel.data = ko.observable({ temperature: 24, rssi: 80 });