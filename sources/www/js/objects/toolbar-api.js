/**
 * Created by nicolasHILAIRE on 04/12/2015.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function ToolbarApi() { }

/**
 * Check if the widget use battery item and configure it
 */
ToolbarApi.manageBatteryConfiguration = function (widget) {
    //battery
    var $battery = widget.$toolbar.find(".widget-toolbar-battery");
    if ($battery.length > 0) {
        //we clear the div that will contain the battery indicator
        $battery.empty();
        var deviceId = $battery.attr("deviceId");
        if (deviceId) {
            //we check for the device to look if it has battery keyword
            DeviceManager.getKeywordsByDeviceId(deviceId, function (keywords) {
                var batteryLevel = keywords.find(function (element) { return element.capacityName == "batteryLevel"; });
                if (batteryLevel) {
                    //it has capacity
                    $battery.append("<span class=\"\"/>");
                    $battery.attr("keywordId", batteryLevel.id);
                    //we add it to the filter of keyword for websockets
                    widget.ListenKeyword(batteryLevel.id);

                    //we ask immediately for the battery value
                    AcquisitionManager.getLastValue(batteryLevel.id, function (lastValue) {
                        ToolbarApi.updateBatteryLevel(widget, lastValue.value);
                    });
                }
                else {
                    //we can hide the div to prevent margin spaces before the title
                    $battery.hide();
                }
            });
        }
    }
}

/**
 * Apply batterylevel onto the icon of the toolbar
 */
ToolbarApi.updateBatteryLevel = function (widget, batteryLevel) {
    assert(!isNullOrUndefined(widget), "$widget must be defined");
    assert(!isNullOrUndefined(batteryLevel), "batteryLevel must be defined");

    //we compute the battery fill
    var fill;
    var lvl = parseInt(batteryLevel);
    if (lvl < 20.0)
        fill = 0;
    else if (lvl < 40.0)
        fill = 1;
    else if (lvl < 60.0)
        fill = 2;
    else if (lvl < 80.0)
        fill = 3;
    else
        fill = 4;

    widget.$toolbar.find("div.widget-toolbar-battery span").removeClass().addClass("fa fa-battery-" + fill);
};

/**
 * Add a battery icon to the widget. DeviceId should be configured with ToolbarApi.configureBatteryIcon
 */
ToolbarApi.addBatteryIconToWidget = function (widget) {
	widget.$toolbar.append("<div class=\"widget-toolbar-battery\" deviceId=\"\"></div>");
}

/**
 * Define the keyword Id of the battery toolbar icon. (ToolbarApi.addBatteryIconToWidget must have been called before)
 */
ToolbarApi.configureBatteryIcon = function (widget, deviceId) {
    var $batteryIcon = widget.$toolbar.find(".widget-toolbar-battery");
    if ($batteryIcon) {
        $batteryIcon.attr("deviceId", deviceId);
    }
}