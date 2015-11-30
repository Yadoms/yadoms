/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetManager(){}

WidgetManager.DeactivatedWidgetPackageName = "dev-deactivated-widget";

/**
 * Create a Widget instance
 * @param {Object} json The widget content
 * @returns {Widget}
 */
WidgetManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.idPage), "json.idPage must be defined");
   assert(!isNullOrUndefined(json.type), "json.name must be defined");
   assert(!isNullOrUndefined(json.title), "json.title must be defined");
   assert(!isNullOrUndefined(json.sizeX), "json.sizeX must be defined");
   assert(!isNullOrUndefined(json.sizeY), "json.sizeY must be defined");
   assert(!isNullOrUndefined(json.positionX), "json.positionX must be defined");
   assert(!isNullOrUndefined(json.positionY), "json.positionY must be defined");
   assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");
   
   return new Widget(json.id, json.idPage, json.type, json.title, json.sizeX, json.sizeY, json.positionX, json.positionY, json.configuration);
};

/**
 * Return the widget from its id and its page id
 * @param {Number} pageId The page id
 * @param {Number} widgetId The widget id
 * @returns {Widget}
 */
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

/**
 * Get the list of widgets to add to a page
 * @param {Page} page The page to request widgets
 * @returns a promise
 */
WidgetManager.getWidgetOfPageFromServer = function(page) {
   assert(page !== undefined, "page must be defined");

   //we save the information that the widgets for this page have already been asked
   page.loaded = true;

   var d = new $.Deferred();
   RestEngine.get("/rest/page/" + page.id + "/widget")
      .done(function(data) {
         var list = [];
         $.each(data.widget, function(index, value) {
            list.push(WidgetManager.factory(value));
         });
		 
         d.resolve(list);
      })
      .fail(function(errorMessage) {
         console.error("Fail to getWidgetOfPageFromServer from server : " + errorMessage);
         notifyError($.t("objects.widgetManager.unableToGetWidgets"), errorMessage);
         d.reject(errorMessage);
      });
	return d.promise();   
};

/**
 * Get a widget view from the server 
 * @param {String} widgetType The widget typ eto download the view
 * @returns a promise
 * @private
 */
WidgetManager.getViewFromServer_ = function(widgetType) {
   assert(!isNullOrUndefined(widgetType), "widgetType must be defined");
   var d = new $.Deferred();
   RestEngine.get("widgets/" + widgetType + "/view.html", undefined, "auto")
      .done(function(view) {
         if (!isNullOrUndefined(view) && view.match(".*<script.*id=\"" + widgetType + "-template\">.*")) {
            $("div#templates").append(view);
            d.resolve();
         } else {
            d.reject("Fail to load view.html of widget " + widgetType);
         }
         
      })
      .fail(function(errorMessage) {
         console.error("Fail to get view from server : " + errorMessage);
         d.reject(errorMessage);
      });
	return d.promise();

};


/**
 * Get a widget viewModel from the server 
 * @param {String} widgetType The widget type to download the viewModel
 * @returns a promise
 * @private
 */
WidgetManager.getViewModelFromServer_ = function(widgetType) {
   assert(!isNullOrUndefined(widgetType), "widgetType must be defined");

   widgetViewModelCtor = null;
   var d = new $.Deferred();
   RestEngine.get("widgets/" + widgetType + "/viewModel.js", undefined, "script")
      .done(function(viewModel) {
         if (isNullOrUndefined(widgetViewModelCtor)) {
            console.error("ViewModel of widget " + widgetType + " do not contains widgetViewModelCtor function");
            d.reject();
         } else {
            WidgetPackageManager.widgetPackages[widgetType].viewModelCtor = widgetViewModelCtor;
            //all job has been done without error
            WidgetPackageManager.widgetPackages[widgetType].viewAnViewModelHaveBeenDownloaded = true;
            
            //ensure next async call will not use this viewModel
            widgetViewModelCtor = null;
            d.resolve();
         }      
         
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
                WidgetManager.updateWidgetConfiguration_(widget);
				
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

/**
 * Fire a configurationChange event
 * @param {Widget} widget The widget 
 * @private
 */
WidgetManager.updateWidgetConfiguration_ = function(widget) {
   try
   {   
	   //Update the widget title
       widget.$gridWidget.find('span.panel-widget-title').text(widget.title);
	   
       // Update widget specific values
      if (!isNullOrUndefined(widget.viewModel.configurationChanged))
         widget.viewModel.configurationChanged();
   }
   catch (e)
   {
      notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.type, methodName : 'configurationChanged'}));
      console.warn(e);
   }
};

/**
 * Instanciate a widget and configure it
 * @param {Widget} widget The widget to instanciate
 * @param {Json} widgetPackage The associated widget package
 * @private
 */
WidgetManager.consolidate_ = function(widget, widgetPackage) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(widgetPackage), "widgetPackage must be defined");

   //we use to construct the viewModel of the current widget
   //noinspection JSPotentiallyInvalidConstructorUsage
   widget.viewModel = new widgetPackage.viewModelCtor();
   widget.package = widgetPackage.package;
};


/**
 * Load a list of widget and display them on a page
 * @param {Array of Widget} widgetList The list of widgets to load
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @return a promise
 */
WidgetManager.loadWidgets = function(widgetList, pageWhereToAdd) {
   var d = new $.Deferred();
   
   //make the list of distinct widget type to load
   var distinctWidgetTypes = [WidgetManager.DeactivatedWidgetPackageName];
   for(var i=0, j=widgetList.length; i<j; i++){
      if(distinctWidgetTypes.indexOf(widgetList[i].type) == -1)
         distinctWidgetTypes.push(widgetList[i].type);  
   }
   
   //for each distinct type to load, download its View and ViewModel
   var arrayOfDeffered = [];
   $.each(distinctWidgetTypes, function(index, widgetType) {
      arrayOfDeffered.push(WidgetManager.downloadWidgetViewAndVieWModel_(widgetType, false /*should not reject to allow loading partially the page (with deactivated widgets)*/));
   });
   
   //When all view/viewModel off all requested types of widgets are loaded, 
   //then create widgets on page
   $.when.apply($, arrayOfDeffered)
      .done(function() {
         d.resolve();
         var arrayOfLoadingWidgetDeferred = [];
         $.each(widgetList, function(index, widget) {
            arrayOfLoadingWidgetDeferred.push(WidgetManager.loadWidget(widget, pageWhereToAdd));
         });
         
         $.when.apply($, arrayOfLoadingWidgetDeferred)
         .done(function() {
            d.resolve();
         })
         .fail(function(errorMessage) {
            d.reject(errorMessage);
         });
      })
      .fail(function(errorMessage) {
         d.reject(errorMessage);
      });
   
  return d.promise();
};

/**
 * Download the view and the viewModel of a widget
 * @param {String} widgetType The widgetType to load
 * @param {Boolean} canReject If true then the promise reject on error, else (false or undefined) then the promise is resolved but an error is consoled
 * @return a promise
 * @private
 */
WidgetManager.downloadWidgetViewAndVieWModel_ = function(widgetType, canReject) {
   var d = $.Deferred();
   
   WidgetManager.getViewFromServer_(widgetType)
   .done(function() {
      WidgetManager.getViewModelFromServer_(widgetType)
      .done(function() {
         d.resolve();
      })
      .fail(function(error) {
         if(canReject) {
            d.reject(error);
         } else {
            console.error(error);
            d.resolve();
         }
      });
   })
   .fail(function(error) {
      if(canReject) {
         d.reject(error);
      } else {
         console.error(error);
         d.resolve();
      }
   });
      
   return d.promise();
};


/**
 * Load a widget and add it to the page
 * @param {Widget} widget The widgetType to load
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @return a promise
 * @private
 */   
WidgetManager.loadWidget = function(widget, pageWhereToAdd) {
   assert(!isNullOrUndefined(widget), "widget must be defined");
   assert(!isNullOrUndefined(pageWhereToAdd), "pageWhereToAdd must be defined");
   
   var d = $.Deferred();
   
   if (!WidgetPackageManager.packageExists(widget.type)) {
      WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, "package do not exists"); 
      d.reject();
      
   } else { 
            
      if(!WidgetPackageManager.widgetPackages[widget.type].viewAnViewModelHaveBeenDownloaded) {
         WidgetManager.downloadWidgetViewAndVieWModel_(widget.type, true)
         .done(function() {
            WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, widget.type);
            d.resolve();
         })
         .fail(function(errorMessage) {
            WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, errorMessage);      
            d.reject(errorMessage);
         });
      } else {
         WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, widget.type);
         d.resolve();
      }
   
   }
   return d.promise();
};
  

/**
 * Instanciate a widget instance, and add it to page
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @param {Widget} widget The widget to load
 * @param {String} widgetType The widgetType to load
 * @private
 */
WidgetManager.instanciateWidgetToPage_ = function(pageWhereToAdd, widget, widgetType) {
   try
   {
      //we finalize the load of the widget
      WidgetManager.consolidate_(widget, WidgetPackageManager.widgetPackages[widgetType]);
      WidgetManager.addToDom_(widget);
      //we add the widget to the collection
      pageWhereToAdd.addWidget(widget);
   }
   catch(ex)
   {
      if(!widget.downgraded) {
         //load widget as deactivated
         WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, "Exception in loading viewModel : " + ex);
      } else {
         console.error("Fail to load deactivated widget");
      }
   }
}

/**
 * Instanciate a DEACTIVATED widget instance, and add it to page
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @param {Widget} widget The widget to load
 * @param {String} errorMessage An error message
 * @private
 */
WidgetManager.instanciateDowngradedWidgetToPage_ = function(pageWhereToAdd, widget, errorMessage) {
   console.warn("Fail to load widget " + widget.type + " , then load deactivated model instead." + (errorMessage||""));
   
   //flag the widget as downgraded
   widget.requiredType = widget.type;
   widget.downgraded = true;
   
   //load downgraded widget instead
   WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, WidgetManager.DeactivatedWidgetPackageName);         
}

/**
 * Add a widget to page
 * @param {Widget} widget The widget to add
 * @private
 */
WidgetManager.addToDom_ = function(widget) {
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
   WidgetManager.updateWidgetConfiguration_(widget);

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

    
   widget.$gridWidget.i18n();

   //we ask for widget refresh data
   updateWidgetPolling(widget);
};

/**
 * Enable or disable customization on widget
 * @param {Widget} widget The widget to update the mode
 * @param {Boolean} enable If true configure the widget in customization mode, else in normal mode
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
 * @param {Widget} widget The widget to add
 * @returns {object} The gridstack item
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
       "<div class=\"grid-stack-item-content\">\n" +
         "<div class=\"panel-widget-customization-overlay customization-item hidden\">\n" +
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
	   "<div class=\"panel panel-primary panel-widget\" >" +
	       "<div class=\"panel-heading panel-widget-header\">" +
               "<div class=\"panel-widget-title-toolbar\" ></div>" +
               "<span class=\"panel-widget-title\">" + widget.title + "</span>\n" +
           "</div>" +
           "<div class=\"panel-widget-body\" id=\"widget-" + widget.id + "\"  data-bind=\"template: { name: '" + type + "-template' }\"/>\n" +
       "</div>\n" +
   "</div>\n";

   var item = page.grid.add_widget($(domWidget), widget.positionX, widget.positionY, widget.sizeX, widget.sizeY, false);

   item.i18n();
   return item;
};


