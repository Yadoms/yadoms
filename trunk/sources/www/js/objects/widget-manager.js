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

    var page = PageManager.getPage(pageId);
    assert(page != null, "page Id doesn't exit");
    var widget = page.getWidget(widgetId);
    assert(widget != null, "widget Id doesn't exit in the page");
    return widget;
};

/**
 * Return the widget object attached to gridster element
 * @param $gridsterElement
 * @returns {Widget}
 */
WidgetManager.getFromGridsterElement = function($gridsterElement) {
   assert($gridsterElement !== undefined, "$gridsterElement must be defined");

   var pageId = $gridsterElement.attr("page-id");
   var widgetId = $gridsterElement.attr("widget-id");
   return WidgetManager.get(pageId, widgetId);
}

WidgetManager.getWidgetOfPageFromServer = function(page, callback) {
   assert(page !== undefined, "page must be defined");

   //we save the information that the widgets for this page have already been asked
   page.loaded = true;

   $.getJSON("/rest/page/" + page.id + "/widget")
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.widgetManager.unableToGetWidgets"), JSON.stringify(data));
            if (!isNullOrUndefined(callback))
               callback(null);
            return null;
         }

         var list = [];
         $.each(data.data.widget, function(index, value) {
            list.push(WidgetManager.factory(value));
         });

         if (!isNullOrUndefined(callback))
            callback(list);
         return list;
      })
      .fail(function() {
         notifyError($.t("objects.widgetManager.unableToGetWidgets"));
         if (!isNullOrUndefined(callback))
            callback(null);
         return null;
      });
}

WidgetManager.getViewFromServer = function(widget, callback) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(callback), "callback must be defined");

   $.get("widgets/" + widget.name + "/view.html")
      .done(function( data ) {
         if (!isNullOrUndefined(callback))
            callback(data);

      })
      .fail( function() {callback(null);} );
}

WidgetManager.getViewModelFromServer = function(widget, callback) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(callback), "callback must be defined");

   widgetViewModelCtor = null;
   $.getScript("widgets/" + widget.name + "/viewModel.js")
      .done(function(data, textStatus, jqxhr) {
            //viewModel.js has been executed and store the viewModel ctor in the global var widgetViewModelCtor
            callback(widgetViewModelCtor);
      })
      .fail(function() {callback(null);});
}

WidgetManager.updateToServer = function(widget, callback) {
    assert(!isNullOrUndefined(widget), "widget must be defined");

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
            notifyError($.t("objects.widgetManager.errorDuringModifyingWidget"), JSON.stringify(data));
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
            notifyWarning($.t("objects.widgetManager.exceptionDuringCallConfigurationChanged", {"widgetType" : widget.name}));
            console.warn(e);
            if (!isNullOrUndefined(callback))
                callback(false);
            return;
        }

        })
        .fail(function(widgetName) { return function() {
            notifyError($.t("objects.widgetManager.errorDuringModifyingWidgetNamed", {"widgetType" : widget.name}));
            if (!isNullOrUndefined(callback))
                callback(false);
        };}(widget.name));
}

WidgetManager.consolidate = function(widget, widgetPackage) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(widgetPackage), "widgetPackage must be defined");

   //we use to construct the viewModel of the current widget
   //noinspection JSPotentiallyInvalidConstructorUsage
   widget.viewModel = new widgetPackage.viewModelCtor();
   widget.package = widgetPackage.packageInformation;
}


WidgetManager.loadWidgetList = function(list, pageWhereToAdd) {
   assert(!isNullOrUndefined(list), "list must be defined");
   //for each of them we ask to download the full package (view and viewmodel) if it's not already done
   $.each(list, function(index, widget) {
      if (WidgetPackageManager.packageExists(widget.name)) {
         if (!WidgetPackageManager.widgetPackages[widget.name].viewAnViewModelHaveBeenDownloaded) {
            //we download all needed information and add to the dom all depending widgets
            //to downloaded only once we immediately set that they have been downloaded
            WidgetPackageManager.widgetPackages[widget.name].viewAnViewModelHaveBeenDownloaded = true;

            WidgetManager.getViewFromServer(widget, function(result) {
               if (result != null) {
                  //we append the view into the page
                  $("div#templates").append(result);

                  //we get i18n data
                  i18n.options.resGetPath = 'widgets/__ns__/locales/__lng__.json';
                  i18n.loadNamespace(widget.name);

                  //we get script to execute for this widget
                  WidgetManager.getViewModelFromServer(widget, function(viewModel) {
                     if (!isNullOrUndefined(viewModel)) {
                        WidgetPackageManager.widgetPackages[widget.name].viewModelCtor = widgetViewModelCtor;

                        //we have get all needed information to add widgets of this type to dom
                        //we look for all widgets of this kind
                        $.each(list, function(index, widgetToAdd) {
                           if (widget.name == widgetToAdd.name) {
                              //this widget is of the good type so we can add it to gui

                              WidgetManager.consolidate(widgetToAdd, WidgetPackageManager.widgetPackages[widgetToAdd.name]);
                              WidgetManager.addToDom(widgetToAdd);
                              //we add the widget to the collection
                              pageWhereToAdd.addWidget(widgetToAdd);
                              //we indicate the widget has been fully loaded
                              widgetToAdd.loaded = true;
                           }
                        });

                        //we look if all widget of the list have been fully loaded
                        //we get all widget that have not been loaded and if the collection is empty it's finished
                        var res = $.grep(list, function(item) {
                           return (item.loaded == false);
                        });
                        if (res.length == 0) {
                           //all widget have been loaded
                           //we can close the noty if opened
                           if (!isNullOrUndefined(loadWidgetsNotification)) {
                              loadWidgetsNotification.close();
                              loadWidgetsNotification = null;
                           }
                        }
                     }
                     else {
                        askForWidgetDelete(widget.name, list, $.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : widget.name}));
                     }
                  });
               }
               else {
                  askForWidgetDelete(widget.name, list, $.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : widget.name}));
               }
            });
         }
      }
      else {
         askForWidgetDelete(widget.name, list, $.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : widget.name}));
      }
   });
}

WidgetManager.addToDom = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   var widgetDivId = "widget-" + widget.id;
   widget.$gridsterWidget = WidgetManager.createGridsterWidget(widget);
   widget.$div = $("div#" + widgetDivId);

   //we check if we are in customization we must apply customization on the new item
   if (customization)
   {
      widget.$gridsterWidget.find(".customization-item").removeClass("hidden");
      widget.$gridsterWidget.addClass("liWidgetCustomization");
   }

   var page = PageManager.getPage(widget.idPage);
   assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

   //we apply binding to the view
   ko.applyBindings(widget.viewModel, widget.$div[0]);
   //we add minimum dimension constraint to the gridster widget
   if (widget.package.dimensions.min !== undefined)
   {
      //min dimension is set
      assert((widget.package.dimensions.min.x !== undefined) && (widget.package.dimensions.min.y !== undefined), "You can't set only one axis of the widget minimum dimension");
      widget.$gridsterWidget.data('coords').grid.min_size_x = widget.package.dimensions.min.x;
      widget.$gridsterWidget.data('coords').grid.min_size_y = widget.package.dimensions.min.y;
   }

   if (widget.package.dimensions.max !== undefined)
   {
      //min dimension is set
      assert((widget.package.dimensions.max.x !== undefined) && (widget.package.dimensions.max.y !== undefined), "You can't set only one axis of the widget maximum dimension");
      page.gridster.set_widget_max_size(widget.$gridsterWidget, [widget.package.dimensions.max.x, widget.package.dimensions.max.y]);
   }

   //we initialize the widget
   try
   {
      if (widget.viewModel.initialize !== undefined)
         widget.viewModel.initialize(widget);
   }
   catch (e)
   {
      notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'initialize'}));
      console.warn(e);
   }

   //we notify that configuration has changed
   try
   {
      if (widget.viewModel.configurationChanged !== undefined)
         widget.viewModel.configurationChanged();
   }
   catch (e)
   {
      notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'configurationChanged'}));
      console.warn(e);
   }

   //we notify that widget has been resized
   try
   {
      if (widget.viewModel.resized !== undefined) {
         widget.viewModel.resized();
      }
   }
   catch (e)
   {
      notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'resized'}));
      console.warn(e);
   }

   //we listen click event on configure click
   widget.$gridsterWidget.find('span.btn-configure-widget').bind('click', function (e) {
      var widgetDOMElement = $(e.currentTarget).parents("li.widget");
      var pageId = widgetDOMElement.attr("page-id");
      var widgetId = widgetDOMElement.attr("widget-id");
      modals.widgetConfiguration.load(function (pageId, widgetId) { return function() {
         var widgetToConfigure = WidgetManager.get(pageId, widgetId);
         configureWidget(widgetToConfigure, function () {
            WidgetManager.updateToServer(widgetToConfigure);
         });
      }}(pageId, widgetId));
   });

   //we listen click event on delete click
   widget.$gridsterWidget.find('span.btn-delete-widget').bind('click', function (e) {
      var widgetDOMElement = $(e.currentTarget).parents("li.widget");
      var pageId = widgetDOMElement.attr("page-id");
      var widgetId = widgetDOMElement.attr("widget-id");
      modals.widgetDelete.load(function (pageId, widgetId) {return function() {showDeleteWidgetModal(pageId, widgetId)}}(pageId, widgetId));
   });

   widget.$div.i18n();
}

/**
 * Create a new graphic Widget and add it to the corresponding gridster
 * @param widget widget to add
 * @returns {gridster}
 */
WidgetManager.createGridsterWidget = function(widget) {
   assert(widget !== undefined, "widget must be defined");

   var page = PageManager.getPage(widget.idPage);
   assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

   var domWidget = "<li class=\"widget\" page-id=\"" + widget.idPage + "\" widget-id=\"" + widget.id +"\">\n" +
      "<div class=\"widgetCustomizationToolbar customization-item hidden\">\n";

   if (!isNullOrUndefined(widget.package.configurationSchema)) {
      domWidget += "<span class=\"btn-configure-widget\"><i class=\"glyphicon glyphicon-cog\"></i></span>\n";
   }

   domWidget +=    "<span class=\"btn-delete-widget\"><i class=\"glyphicon glyphicon-trash\"></i></span>\n" +
      "</div>\n" +
      "<div id=\"widget-" + widget.id + "\" class=\"widgetDiv\" data-bind=\"template: { name: '" + widget.name + "-template' }\"/>\n" +
      "</li>\n"

   var item = page.gridster.add_widget(domWidget, widget.sizeX, widget.sizeY, widget.positionX, widget.positionY);

   item.i18n();
   return item;
}