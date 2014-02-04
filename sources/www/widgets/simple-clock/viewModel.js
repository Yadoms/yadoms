function SimpleClockViewModel() {
    this.data;
    this.resized = function($widget) {
        if ($widget.width() <= 150)
            $widget.find(".simpleClockWidget").css("font-size", "1.0em");
        else if ($widget.width() <= 300)
            $widget.find(".simpleClockWidget").css("font-size", "2.0em");
        else
            $widget.find(".simpleClockWidget").css("font-size", "4.0em");
    }
}

widgetViewModel = new SimpleClockViewModel();
widgetViewModel.data = ko.observable();
