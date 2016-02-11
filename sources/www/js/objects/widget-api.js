/**
 * Created by nicolasHILAIRE on 09/02/2016.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetApi() { }

/**
 * toolbar section
 */
WidgetApi.toolbar = {};

/**
 * Add a battery icon to the widget. DeviceId should be configured with ToolbarApi.configureBatteryIcon
 */
WidgetApi.toolbar.addBatteryIcon = function (widget) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    ToolbarApi.addBatteryIconToWidget(widget);
}

/**
 * Define the keyword Id of the battery toolbar icon. (ToolbarApi.addBatteryIconToWidget must have been called before)
 */
WidgetApi.toolbar.configureBatteryIcon = function (widget, deviceId) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
    ToolbarApi.configureBatteryIcon(widget, deviceId);
}

/**
 * Add a custom icon the toolbar of a widget
 */
WidgetApi.toolbar.appendCustom = function (widget, buttonDOM) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    assert(!isNullOrUndefined(buttonDOM), "buttonDOM must be defined");
    ToolbarApi.appendCustom(widget, buttonDOM);
}

/**
 * Add a separator on the toolbar of a widget
 */
WidgetApi.toolbar.appendSeparator = function (widget) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    ToolbarApi.appendSeparator(widget);
}

/**
 * Keyword section
 */
WidgetApi.keyword = {};

/**
 * Obtain information about a keyword
 * @param {} keywordId to query
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.keyword.getInformation = function(keywordId) {
    assert(!isNullOrUndefined(keywordId), "keywordId must be defined");
    return KeywordManager.get(keywordId);
};

/**
 * Register keywords to receive notifications when a new acquisition triggers
 * @param {} keywordIds to register (can be a single value or an array of values)
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.keyword.registerKeywordAcquisitions = function(widget, keywordIds) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    assert(!isNullOrUndefined(keywordIds), "keywordIds must be defined");

    if (!widget.listenedKeywords)
        widget.listenedKeywords = [];

    if (Array.isArray(keywordIds)) {
        $.each(keywordIds, function(index, value) {
            widget.listenedKeywords.push(value);
        });
    } else {
        widget.listenedKeywords.push(keywordIds);
    }
};

/**
 * Lazy load required libraries
 * @param {} librayNames to load (can be a single value or an array of values)
 * @returns {} a promise that's return done when libraries are loaded
 */
WidgetApi.loadLibrary = function (librayNames) {
    assert(!isNullOrUndefined(librayNames), "librayNames must be defined");

    if (Array.isArray(librayNames)) {
        return asyncLoadJSLibs(librayNames);
    } else {
        return asyncLoadJSLib(librayNames);
    }
};