/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetManager(){}

WidgetManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.idPage), "json.idPage must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.sizeX), "json.sizeX must be defined");
   assert(!isNullOrUndefined(json.sizeY), "json.sizeY must be defined");
   assert(!isNullOrUndefined(json.positionX), "json.positionX must be defined");
   assert(!isNullOrUndefined(json.positionY), "json.positionY must be defined");
   assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");

   return new Widget(json.id, json.idPage, json.name, json.sizeX, json.sizeY, json.positionX, json.positionY, json.configuration);
};

WidgetManager.get = function(pageId, widgetId) {
    assert(pageId !== undefined, "page Id must be defined");
    assert(widgetId !== undefined, "Widget Id must be defined");

    var page = pageContainer.getPage(pageId);
    assert(page != null, "page Id doesn't exit");
    var widget = page.getWidget(widgetId);
    assert(widget != null, "widget Id doesn't exit in the page");
    return widget;
};

WidgetManager.updateToServer = function(widget, callback) {
    assert(!isNullOrUndefined(widget), "device must be defined");

    $.ajax({
        type: "PUT",
        url: "/rest/widget/" + widget.id,
        data: JSON.stringify(widget),
        contentType: "application/json; charset=utf-8",
        dataType: "json"
    })
    .done(function(data) {
        //we parse the json answer
        if (data.result != "true")
        {
            //TODO : relocate i18n
            notifyError($.t("modals.configure-widget.errorDuringModifyingWidget"), JSON.stringify(data));
            if (!isNullOrUndefined(callback))
                callback(false);
            return;
        }

        //we notify that configuration has changed
        try
        {
            if (widget.viewModel.configurationChanged !== undefined)
                widget.viewModel.configurationChanged();
            if (!isNullOrUndefined(callback))
                callback(true);
        }
        catch (e)
        {
            notifyWarning("The widget " + widget.name + " has generated an exception during call configurationChanged() method");
            console.warn(e);
            if (!isNullOrUndefined(callback))
                callback(false);
            return;
        }

        })
        .fail(function(widgetName) { return function() {
            notifyError($.t("modals.configure-widget.errorDuringModifyingWidgetNamed", {"widgetType" : widgetName}));
            if (!isNullOrUndefined(callback))
                callback(false);
        };}(widget.name));
}