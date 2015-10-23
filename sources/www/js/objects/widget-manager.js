/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetManager(){}

WidgetManager.DeactivatedWidgetPackageName = "dev-deactivated-widget";

WidgetManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.idPage), "json.idPage must be defined");
   assert(!isNullOrUndefined(json.type), "json.name must be defined");
   assert(!isNullOrUndefined(json.sizeX), "json.sizeX must be defined");
   assert(!isNullOrUndefined(json.sizeY), "json.sizeY must be defined");
   assert(!isNullOrUndefined(json.positionX), "json.positionX must be defined");
   assert(!isNullOrUndefined(json.positionY), "json.positionY must be defined");
   assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");

   return new Widget(json.id, json.idPage, json.type, json.sizeX, json.sizeY, json.positionX, json.positionY, json.configuration);
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
 * Return the widget object attached to grid element
 * @param $gridElement
 * @returns {Widget}
 */
WidgetManager.getFromGridElement = function($gridElement) {
   assert($gridElement !== undefined, "$gridElement must be defined");

   var pageId = $gridElement.attr("page-id");
   var widgetId = $gridElement.attr("widget-id");
   return WidgetManager.get(pageId, widgetId);
};

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
            if ($.isFunction(callback))
               callback(null);
            return null;
         }

         var list = [];
         $.each(data.data.widget, function(index, value) {
            list.push(WidgetManager.factory(value));
         });

         if ($.isFunction(callback))
            callback(list);
         return list;
      })
      .fail(function() {
         notifyError($.t("objects.widgetManager.unableToGetWidgets"));
         if ($.isFunction(callback))
            callback(null);
         return null;
      });
};
/*
WidgetManager.getViewFromServerSync = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   var view = null;

   $.ajax({
      url:"widgets/" + widget.type + "/view.html",
      async:   false
   })
   .done(function( data ) {
      view = data;
   });

   return view;
};
*/
WidgetManager.getViewFromServer = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   var d = $.Deferred();
   RestEngine.get("widgets/" + widget.type + "/view.html", undefined, null)
      .done(function(data) {
         d.resolve(data);
      })
      .fail(function(errorMessage) {
         console.error("Fail to get view from server : " + errorMessage);
         d.reject(errorMessage);
      });
   
	return d.promise();
};



/*
WidgetManager.getViewModelFromServerSync = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   widgetViewModelCtor = null;
   $.ajax({
      url: "widgets/" + widget.type + "/viewModel.js",
      dataType: "script",
      async:   false
   });

   //if the ajax method works ok the widgetViewModelCtor is set

   return widgetViewModelCtor;
};
*/

WidgetManager.getViewModelFromServer = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   widgetViewModelCtor = null;
   var d = $.Deferred();
   RestEngine.get("widgets/" + widget.type + "/viewModel.js", undefined, "script")
      .done(function(data) {
         d.resolve(widgetViewModelCtor);
      })
      .fail(function(errorMessage) {
         console.error("Fail to get viewModel from server : " + errorMessage);
         d.reject(errorMessage);
      });
   
	return d.promise();
};

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
            if ($.isFunction(callback))
                callback(false);
        } else {

            //we notify that configuration has changed
            try {
                WidgetManager.updateWidgetConfiguration(widget);

                //we ask for a refresh of widget data
                updateWidgetPolling(widget);

                if ($.isFunction(callback))
                    callback(true);
            }
            catch (e) {
                notifyWarning($.t("objects.widgetManager.exceptionDuringCallConfigurationChanged", {"widgetType": widget.type}));
                console.warn(e);
                if ($.isFunction(callback))
                    callback(false);
            }
        }
        })
        .fail(function() { return function() {
            notifyError($.t("objects.widgetManager.errorDuringModifyingWidgetNamed", {"widgetType" : widget.type}));
            if ($.isFunction(callback))
                callback(false);
        };}(widget.type));
};

WidgetManager.updateWidgetConfiguration = function(widget) {
   try
   {
       // Update widget specific tvalues
      if (!isNullOrUndefined(widget.viewModel.configurationChanged))
         widget.viewModel.configurationChanged();
   }
   catch (e)
   {
      notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'configurationChanged'}));
      console.warn(e);
   }
};

WidgetManager.consolidate = function(widget, widgetPackage) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(widgetPackage), "widgetPackage must be defined");

   //we use to construct the viewModel of the current widget
   //noinspection JSPotentiallyInvalidConstructorUsage
   widget.viewModel = new widgetPackage.viewModelCtor();
   widget.package = widgetPackage.package;
};

WidgetManager.loadWidgetFinalize_ = function(pageWhereToAdd, widget, widgetType, downgraded, requiredType) {
   if(downgraded) {
      widget.requiredType = requiredType;
      widget.downgraded = true;
   }
   
   //we finalize the load of the widget
   WidgetManager.consolidate(widget, WidgetPackageManager.widgetPackages[widgetType]);
   WidgetManager.addToDom(widget);
   //we add the widget to the collection
   pageWhereToAdd.addWidget(widget);
}

WidgetManager.loadWidget = function(widget, pageWhereToAdd) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(pageWhereToAdd), "pageWhereToAdd must be defined");
   if (WidgetPackageManager.packageExists(widget.type)) {
      if (!WidgetPackageManager.widgetPackages[widget.type].viewAnViewModelHaveBeenDownloaded) {
         //we must download all missing information
         WidgetManager.getViewFromServer(widget)
            .done(function(view) {
               if (!isNullOrUndefined(view)) {
                  //we append the view into the page
                  $("div#templates").append(view);

                  WidgetManager.getViewModelFromServer(widget)
                     .done(function(viewModel) {
                        if (!isNullOrUndefined(viewModel)) {
                           WidgetPackageManager.widgetPackages[widget.type].viewModelCtor = viewModel;
                           //all job has been done without error
                           WidgetPackageManager.widgetPackages[widget.type].viewAnViewModelHaveBeenDownloaded = true;
                        }
                        else {
                           WidgetManager.loadAsDowngraded(widget, pageWhereToAdd);
                           return;
                        }                        
                        
                        WidgetManager.loadWidgetFinalize_(pageWhereToAdd, widget, widget.type);
                     })            
                     .fail(function(errorMessage){
                        WidgetManager.loadAsDowngraded(widget, pageWhereToAdd);
                     });
               }
               else {
                  WidgetManager.loadAsDowngraded(widget, pageWhereToAdd);
               }
            })
            .fail(function(errorMessage){
               WidgetManager.loadAsDowngraded(widget, pageWhereToAdd);
            });
      } else {
         WidgetManager.loadWidgetFinalize_(pageWhereToAdd,widget, widget.type);
      }
   }
   else {
      WidgetManager.loadAsDowngraded(widget, pageWhereToAdd);
   }
};

WidgetManager.loadAsDowngraded = function(widget, pageWhereToAdd) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(pageWhereToAdd), "pageWhereToAdd must be defined");

   notifyWarning($.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : widget.type}));
   //we set downgraded widget info to this widget.

   var deactivatedWidget = {};
   deactivatedWidget.type = WidgetManager.DeactivatedWidgetPackageName;

   if (WidgetPackageManager.packageExists(deactivatedWidget.type)) {
      if (!WidgetPackageManager.widgetPackages[deactivatedWidget.type].viewAnViewModelHaveBeenDownloaded) {
         //we must download all missing information
         WidgetManager.getViewFromServer(deactivatedWidget)
            .done(function(view) {
               if (!isNullOrUndefined(view)) {
                  //we append the view into the page
                  $("div#templates").append(view);

                  WidgetManager.getViewModelFromServer(deactivatedWidget)
                  .done(function(viewModel) {
                     if (!isNullOrUndefined(viewModel)) {
                        WidgetPackageManager.widgetPackages[deactivatedWidget.type].viewModelCtor = viewModel;
                        //all job has been done without error
                        WidgetPackageManager.widgetPackages[deactivatedWidget.type].viewAnViewModelHaveBeenDownloaded = true;
                     }
                     else {
                        notifyError($.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : deactivatedWidget.type}));
                        return;
                     }
                     WidgetManager.loadWidgetFinalize_(pageWhereToAdd, widget, deactivatedWidget.type, true, widget.type);
                  })
                  .fail(function(errorMessage) {
                     notifyError($.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : deactivatedWidget.type}));
                  });
               }
               else {
                  notifyError($.t("objects.widgetManager.partOfWidgetIsMissing", {widgetName : deactivatedWidget.type}));
               }
            })
            .fail(function(errorMessage) {
               WidgetManager.loadWidgetFinalize_(pageWhereToAdd, widget, deactivatedWidget.type, true, widget.type);
            }) ;
      }
      else {
         WidgetManager.loadWidgetFinalize_(pageWhereToAdd, widget, deactivatedWidget.type, true, widget.type);
      }
   }
};

WidgetManager.addToDom = function(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   var widgetDivId = "widget-" + widget.id;
   widget.$gridWidget = WidgetManager.createGridstackWidget(widget);
   widget.$div = $("div#" + widgetDivId);

   //we check if we are in customization we must apply customization on the new item
   WidgetManager.enableCustomization(widget, customization);
   if (customization)
   {
      widget.$gridWidget.find(".customization-item").removeClass("hidden");

   }

   var page = PageManager.getPage(widget.idPage);
   assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

   //we apply binding to the view
   ko.applyBindings(widget.viewModel, widget.$div[0]);

   widget.$gridWidget.attr("data-gs-x", gridWidth);



   //we initialize the widget
   try
   {
      if (widget.viewModel.initialize !== undefined)
         widget.viewModel.initialize(widget);
   }
   catch (e)
   {
      notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'initialize'}));
      console.warn(e);
   }

   //we notify that configuration has changed
   WidgetManager.updateWidgetConfiguration(widget);

   //we notify that widget has been resized
   try
   {
      if (widget.viewModel.resized !== undefined) {
         widget.viewModel.resized();
      }
   }
   catch (e)
   {
      notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'resized'}));
      console.warn(e);
   }

   //we listen click event on configure click
   widget.$gridWidget.find('div.btn-configure-widget').bind('click', function (e) {
      var widgetDOMElement = $(e.currentTarget).parents(".widget");
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
   widget.$gridWidget.find('div.btn-delete-widget').bind('click', function (e) {
      var widgetDOMElement = $(e.currentTarget).parents(".widget");
      var pageId = widgetDOMElement.attr("page-id");
      var widgetId = widgetDOMElement.attr("widget-id");
      modals.widgetDelete.load(function (pageId, widgetId) {return function() {showDeleteWidgetModal(pageId, widgetId)}}(pageId, widgetId));
   });

   widget.$div.i18n();

   //we ask for widget refresh data
   updateWidgetPolling(widget);
};

/**
 * Enable or disable customization on widget
 * @param enable
 */
WidgetManager.enableCustomization = function(widget, enable) {
   var page = PageManager.getPage(widget.idPage);
   assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

   page.grid.movable(widget.$gridWidget, enable);
   page.grid.resizable(widget.$gridWidget, enable);

   if (enable)
      widget.$gridWidget.find(".customization-item").removeClass("hidden");
   else
      widget.$gridWidget.find(".customization-item").addClass("hidden");
};

/**
 * Create a new graphic Widget and add it to the corresponding gridstack
 * @param widget widget to add
 * @returns {object}
 */
WidgetManager.createGridstackWidget = function(widget) {
   assert(widget !== undefined, "widget must be defined");

   var page = PageManager.getPage(widget.idPage);
   assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

   var domWidget = "<div class=\"widget\" page-id=\"" + widget.idPage + "\" widget-id=\"" + widget.id +"\"";

   //we add minimum dimension constraint to the grids widget
   if (!isNullOrUndefined(widget.package.dimensions)) {
      if (!isNullOrUndefined(widget.package.dimensions.min)) {
         if (!isNullOrUndefined(widget.package.dimensions.min.x))
            domWidget += " data-gs-min-width=\"" + widget.package.dimensions.min.x + "\"";
         if (!isNullOrUndefined(widget.package.dimensions.min.y))
            domWidget += " data-gs-min-height=\"" + widget.package.dimensions.min.y + "\"";
      }
      if (!isNullOrUndefined(widget.package.dimensions.max)) {
         if (!isNullOrUndefined(widget.package.dimensions.max.x))
            domWidget += " data-gs-max-width=\"" + widget.package.dimensions.max.x + "\"";
         if (!isNullOrUndefined(widget.package.dimensions.max.y))
            domWidget += " data-gs-max-height=\"" + widget.package.dimensions.max.y + "\"";
      }
   }

   domWidget += ">\n" +
       "<div class=\" grid-stack-item-content\">\n" +
         "<div class=\"widgetCustomizationOverlay customization-item hidden\">\n" +
            "<div class=\"customizationToolbar widgetCustomizationToolbar\">";

   if (!isNullOrUndefined(widget.package.configurationSchema)) {
      domWidget += "<div class=\"customizationButton widgetCustomizationButton btn-configure-widget\"><i class=\"fa fa-cog\"></i></div>\n";
   }

   var type = widget.type;
   if (!isNullOrUndefined(widget.downgraded)) {
      type = "dev-deactivated-widget";
   }

   domWidget +=    "<div class=\"customizationButton widgetCustomizationButton btn-delete-widget\"><i class=\"fa fa-trash-o\"></i></div>\n" +
            "</div>\n" +
       "</div>\n" +
       "<div id=\"widget-" + widget.id + "\" class=\"widgetDiv \" data-bind=\"template: { name: '" + type + "-template' }\"/>\n" +
       "</div>\n" +
   "</div>\n";

   var item = page.grid.add_widget($(domWidget), widget.positionX, widget.positionY, widget.sizeX, widget.sizeY, false);

   item.i18n();
   return item;
};


