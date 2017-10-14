/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WidgetManager() { }

WidgetManager.DeactivatedWidgetPackageName = "system-deactivated-widget";

/**
 * Create a Widget instance
 * @param {Object} json The widget content
 * @returns {Widget}
 */
WidgetManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.idPage), "json.idPage must be defined");
    assert(!isNullOrUndefined(json.type), "json.name must be defined");
    assert(!isNullOrUndefined(json.title), "json.title must be defined");
    assert(!isNullOrUndefined(json.sizeX), "json.sizeX must be defined");
    assert(!isNullOrUndefined(json.sizeY), "json.sizeY must be defined");
    assert(!isNullOrUndefined(json.position), "json.position must be defined");
    assert(!isNullOrUndefined(json.configuration), "json.configuration must be defined");

    return new Widget(json.id, json.idPage, json.type, json.title, json.sizeX, json.sizeY, json.position, json.configuration);
};

/**
 * Return the widget from its id and its page id
 * @param {Number} pageId The page id
 * @param {Number} widgetId The widget id
 * @returns {Widget}
 */
WidgetManager.get = function (pageId, widgetId) {
    assert(pageId !== undefined, "page Id must be defined");
    assert(widgetId !== undefined, "Widget Id must be defined");

    var page = PageManager.getPage(pageId);
    assert(page != null, "page Id doesn't exit");
    if (page) {
        var widget = page.getWidget(widgetId);
        assert(widget != null, "widget Id doesn't exit in the page");
        return widget;
    }
    return null;
};

/**
 * Return the widget object attached to grid element
 * @param $gridElement
 * @returns {Widget}
 */
WidgetManager.getFromGridElement = function ($gridElement) {
    assert($gridElement !== undefined, "$gridElement must be defined");

    if ($gridElement) {
        var pageId = $gridElement.attr("page-id");
        var widgetId = $gridElement.attr("widget-id");
        return WidgetManager.get(pageId, widgetId);
    }
    return null;
};

/**
 * Get the list of widgets to add to a page
 * @param {Page} page The page to request widgets
 * @returns a promise
 */
WidgetManager.getWidgetOfPageFromServer = function (page) {
    assert(page !== undefined, "page must be defined");

    var d = new $.Deferred();

    //we save the information that the widgets for this page have already been asked
    if (page) {
        page.loaded = true;

        RestEngine.getJson("/rest/page/" + page.id + "/widget")
           .done(function (data) {
               var list = [];
               $.each(data.widget, function (index, value) {
                   list.push(WidgetManager.factory(value));
               });

               d.resolve(list);
           })
           .fail(function (errorMessage) {
               console.error("Fail to getWidgetOfPageFromServer from server : " + errorMessage);
               notifyError($.t("objects.widgetManager.unableToGetWidgets"), errorMessage);
               d.reject(errorMessage);
           });
    } else {
        d.reject("Page is not defined");
    }
    return d.promise();
};

/**
 * Get a widget view from the server 
 * @param {String} widgetType The widget typ eto download the view
 * @returns a promise
 * @private
 */
WidgetManager.getViewFromServer_ = function (widgetType) {
    assert(!isNullOrUndefined(widgetType), "widgetType must be defined");
    var d = new $.Deferred();

    //we try to download "widget.css" first if present
    asyncLoadCss("widgets/" + widgetType + "/widget.css")
        .always(function () {
            //wherever the result we continue
            RestEngine.get("widgets/" + widgetType + "/view.html")
            .done(function (view) {
                if (!isNullOrUndefined(view) && view.match(".*<script.*id=\"" + widgetType + "-template\">.*")) {
                    $("div#templates").append(view);
                    d.resolve();
                } else {
                    d.reject("Fail to load view.html of widget " + widgetType);
                }

            })
            .fail(function (errorMessage) {
                console.error("Fail to get view from server : " + errorMessage);
                d.reject(errorMessage);
            });
        });
    return d.promise();
};


/**
 * Get a widget viewModel from the server 
 * @param {String} widgetType The widget type to download the viewModel
 * @returns a promise
 * @private
 */
WidgetManager.getViewModelFromServer_ = function (widgetType) {
    assert(!isNullOrUndefined(widgetType), "widgetType must be defined");
    // ReSharper disable AssignToImplicitGlobalInFunctionScope
    widgetViewModelCtor = null;
    var d = new $.Deferred();
    RestEngine.getScript("widgets/" + widgetType + "/viewModel.js")
       .done(function () {
           if (isNullOrUndefined(widgetViewModelCtor)) {
               console.error("ViewModel of widget " + widgetType + " do not contains widgetViewModelCtor function");
               d.reject();
           } else {
               WidgetPackageManager.packageList[widgetType].viewModelCtor = widgetViewModelCtor;
               //all job has been done without error
               WidgetPackageManager.packageList[widgetType].viewAnViewModelHaveBeenDownloaded = true;

               //ensure next async call will not use this viewModel
               widgetViewModelCtor = null;
               d.resolve();
           }

       })
       .fail(function (errorMessage) {
           console.error("Fail to get viewModel from server : " + errorMessage);
           d.reject(errorMessage);
       });

    return d.promise();
    // ReSharper restore AssignToImplicitGlobalInFunctionScope
};

/**
 * Create a new widget
 * @param {Object} newWidget A widget object to create in database
 * @returns {Promise} 
 */
WidgetManager.createWidget = function (newWidget) {

    var d = new $.Deferred();

    var data = JSON.stringify({ idPage: newWidget.idPage, type: newWidget.type, title: newWidget.title, sizeX: newWidget.sizeX, sizeY: newWidget.sizeY, position: newWidget.position, configuration: newWidget.configuration });

    RestEngine.postJson("/rest/widget", { data: data })
    .done(function (widgetData) {
        var w = WidgetManager.factory(widgetData);
        d.resolve(w);
    })
    .fail(d.reject);
    return d.promise();
}

/**
 * Delete a widget
 * @param {Object} widgetToDelete The widget to delete
 * @returns {} 
 */
WidgetManager.deleteWidget = function (widgetToDelete) {
    assert(!isNullOrUndefined(widgetToDelete), "widgetToDelete must be defined");
    return RestEngine.deleteJson("/rest/widget/" + widgetToDelete.id);
}

WidgetManager.updateToServer = function (widget) {
    assert(!isNullOrUndefined(widget), "widget must be defined");

    var d = new $.Deferred();

    RestEngine.putJson("/rest/widget/" + widget.id, { data: JSON.stringify(widget) })
     .done(function () {
         //we notify that configuration has changed
         try {
             WidgetManager.updateWidgetConfiguration_(widget).always(function(){
                //we ask for a refresh of widget data
                updateWidgetPolling(widget);
                d.resolve();
             });
         }
         catch (e) {
             notifyWarning($.t("objects.widgetManager.exceptionDuringCallConfigurationChanged", { "widgetType": widget.type }));
             console.warn(e);
             d.reject(e);
         }
     })
    .fail(function (error) {
        notifyError($.t("objects.widgetManager.errorDuringModifyingWidgetNamed", { "widgetType": widget.type }), error);
        d.reject(error);
    });

    return d.promise();
};

/**
 * Fire a configurationChange event
 * @param {Widget} widget The widget 
 * @private
 */
WidgetManager.updateWidgetConfiguration_ = function (widget) {
    var d = new $.Deferred();
    try {
        //Update the widget title if displayed
        if (widget.displayTitle)
            widget.$gridWidget.find('div.panel-widget-title').text(widget.title);
        else
            widget.$gridWidget.find('div.panel-widget-title').text("");
        //we clear the listened device list before call the configuration
        widget.listenedKeywords = [];
        // Update widget specific values
        if (!isNullOrUndefined(widget.viewModel.configurationChanged)) {
            var defferedResult = widget.viewModel.configurationChanged();
            //we manage answer if it is a promise or not
            defferedResult = defferedResult || new $.Deferred().resolve();
            defferedResult.done(function () {
                //we manage the toolbar api specific icons
                widget.viewModel.widgetApi.manageBatteryConfiguration().always(function(){
					   d.resolve();
				   });
            });
        } else {
            d.resolve();
        }
    }
    catch (e) {
        notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", { widgetName: widget.type, methodName: 'configurationChanged' }));
        console.warn(e);
        d.reject();
    }
    return d.promise();
};

/**
 * Instanciate a widget and configure it
 * @param {Widget} widget The widget to instanciate
 * @param {Json} widgetPackage The associated widget package
 * @private
 */
WidgetManager.consolidate_ = function (widget, widgetPackage) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    assert(!isNullOrUndefined(widgetPackage), "widgetPackage must be defined");

    //we use to construct the viewModel of the current widget
    //noinspection JSPotentiallyInvalidConstructorUsage
    // ReSharper disable once InconsistentNaming
    widget.viewModel = new widgetPackage.viewModelCtor();
    widget.package = widgetPackage.package;
};


/**
 * Load a list of widget and display them on a page
 * @param {Array of Widget} widgetList The list of widgets to load
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @return a promise
 */
WidgetManager.loadWidgets = function (widgetList, pageWhereToAdd) {
    var d = new $.Deferred();

    //make the list of distinct widget type to load
    var distinctWidgetTypes = [WidgetManager.DeactivatedWidgetPackageName];
    for (var i = 0, j = widgetList.length; i < j; i++) {
        if (distinctWidgetTypes.indexOf(widgetList[i].type) === -1)
            distinctWidgetTypes.push(widgetList[i].type);
    }

    //for each distinct type to load, download its View and ViewModel
    var arrayOfDeffered = [];
    $.each(distinctWidgetTypes, function (index, widgetType) {
        arrayOfDeffered.push(WidgetManager.downloadWidgetViewAndVieWModel_(widgetType, false /*should not reject to allow loading partially the page (with deactivated widgets)*/));
    });

    //When all view/viewModel off all requested types of widgets are loaded, 
    //then create widgets on page
    $.when.apply($, arrayOfDeffered)
       .done(function () {
           var arrayOfLoadingWidgetDeferred = [];
           $.each(widgetList, function (index, widget) {
               arrayOfLoadingWidgetDeferred.push(WidgetManager.loadWidget(widget, pageWhereToAdd));
           });

           $.when.apply($, arrayOfLoadingWidgetDeferred)
           .done(function () {
               d.resolve();
           })
           .fail(function (errorMessage) {
               d.reject(errorMessage);
           });
       })
       .fail(function (errorMessage) {
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
WidgetManager.downloadWidgetViewAndVieWModel_ = function (widgetType, canReject) {
    var d = $.Deferred();

    WidgetManager.getViewFromServer_(widgetType)
    .done(function () {
        WidgetManager.getViewModelFromServer_(widgetType)
        .done(function () {
            d.resolve();
        })
        .fail(function (error) {
            if (canReject) {
                d.reject(error);
            } else {
                console.error(error);
                d.resolve();
            }
        });
    })
    .fail(function (error) {
        if (canReject) {
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
 * @param {Boolean} ensureVisible If true ensure the item is visible (scroll if needed), else (false or undefined) do not check widget visibility
 * @return a promise
 * @private
 */
WidgetManager.loadWidget = function (widget, pageWhereToAdd, ensureVisible) {
    assert(!isNullOrUndefined(widget), "widget must be defined");
    assert(!isNullOrUndefined(pageWhereToAdd), "pageWhereToAdd must be defined");

    if (!WidgetPackageManager.packageExists(widget.type)) {
        return WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, "package do not exists", ensureVisible);
    } else {

        if (!WidgetPackageManager.packageList[widget.type].viewAnViewModelHaveBeenDownloaded) {
            WidgetManager.downloadWidgetViewAndVieWModel_(widget.type, true)
            .done(function () {
                return WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, widget.type, ensureVisible);
            })
            .fail(function (errorMessage) {
                return WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, errorMessage, ensureVisible);
            });
        } else {
            return WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, widget.type, ensureVisible);
        }
    }
};


/**
 * Instanciate a widget instance, and add it to page
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @param {Widget} widget The widget to load
 * @param {String} widgetType The widgetType to load
 * @param {Boolean} ensureVisible If true ensure the item is visible (scroll if needed), else (false or undefined) do not check widget visibility
 * @private
 */
WidgetManager.instanciateWidgetToPage_ = function (pageWhereToAdd, widget, widgetType, ensureVisible) {
   
    var d = $.Deferred();
   
    try {
        //we finalize the load of the widget
        WidgetManager.consolidate_(widget, WidgetPackageManager.packageList[widgetType]);
        WidgetManager.addToDom_(widget, ensureVisible).done(function(){
           d.resolve();
        })
        .fail(function(){
           d.reject();
        });
        //we add the widget to the collection
        pageWhereToAdd.addWidget(widget);
    }
    catch (ex) {
        if (!widget.downgraded) {
            //load widget as deactivated
            WidgetManager.instanciateDowngradedWidgetToPage_(pageWhereToAdd, widget, "Exception in loading viewModel : " + ex);
        } else {
            console.error("Fail to load deactivated widget");
        }
        d.reject();
    }
    
    return d.promise();
}

/**
 * Instanciate a DEACTIVATED widget instance, and add it to page
 * @param {Page} pageWhereToAdd The page where to add the widget
 * @param {Widget} widget The widget to load
 * @param {String} errorMessage An error message
* @param {Boolean} ensureVisible If true ensure the item is visible (scroll if needed), else (false or undefined) do not check widget visibility
  * @private
 */
WidgetManager.instanciateDowngradedWidgetToPage_ = function (pageWhereToAdd, widget, errorMessage, ensureVisible) {
    console.warn("Fail to load widget " + widget.type + " , then load deactivated model instead." + (errorMessage || ""));

    //flag the widget as downgraded
    widget.requiredType = widget.type;
    widget.downgraded = true;

    //load downgraded widget instead
    return WidgetManager.instanciateWidgetToPage_(pageWhereToAdd, widget, WidgetManager.DeactivatedWidgetPackageName, ensureVisible);
}

/**
 * Add a widget to page
 * @param {Widget} widget The widget to add
  * @param {Boolean} ensureVisible If true ensure the item is visible (scroll if needed), else (false or undefined) do not check widget visibility
* @private
 */
WidgetManager.addToDom_ = function (widget, ensureVisible) {
    assert(!isNullOrUndefined(widget), "widget must be defined");

    var d = new $.Deferred();
    
    WidgetManager.createGridWidget(widget);

    //we check if we are in customization we must apply customization on the new item
    WidgetManager.enableCustomization(widget, customization);
    if (customization) {
        widget.$gridWidget.find(".customization-item").removeClass("hidden");
    }

    var page = PageManager.getPage(widget.idPage);
    assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

    //we apply binding to the view
    ko.applyBindings(widget.viewModel, widget.$gridWidget[0]);

    //we listen click event on configure click
    widget.$gridWidget.find('div.btn-configure-widget').bind('click', function (e) {
        var widgetDomElement = $(e.currentTarget).parents(".widget");
        var pageId = widgetDomElement.attr("page-id");
        var widgetId = widgetDomElement.attr("widget-id");
        Yadoms.modals.widgetConfiguration.load(function (pageId, widgetId) {
            return function () {
                var widgetToConfigure = WidgetManager.get(pageId, widgetId);
                Yadoms.configureWidget(widgetToConfigure, function () {
                    WidgetManager.updateToServer(widgetToConfigure);
                });
            }
        }(pageId, widgetId));
    });

    //we listen click event on delete click
    widget.$gridWidget.find('div.btn-delete-widget').bind('click', function (e) {
        var widgetDomElement = $(e.currentTarget).parents(".widget");
        var pageId = widgetDomElement.attr("page-id");
        var widgetId = widgetDomElement.attr("widget-id");
        Yadoms.modals.widgetDelete.load(function () {
            Yadoms.showDeleteWidgetModal(pageId, widgetId);
        });
    });

    //we initialize the widget
    try {
        if (widget.viewModel.initialize !== undefined) {
            //we save the widget object into the viewModel
            widget.viewModel.widget = widget;
            widget.viewModel.widgetApi = new WidgetApi(widget);
            var defferedResult = widget.viewModel.initialize();
            //we manage answer if it is a promise or not
            defferedResult = defferedResult || new $.Deferred().resolve();
            defferedResult.done(function () {
                //we notify that configuration has changed
                WidgetManager.updateWidgetConfiguration_(widget).done(function () {
                    //we notify that widget has been resized
                    var defferedResized;
                    try {
                        if (widget.viewModel.resized !== undefined) {
                            defferedResized = widget.viewModel.resized();
                        }
                    }
                    catch (e) {
                        notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", { widgetName: widget.type, methodName: 'resized' }));
                        console.warn(e);
                    }

                    //we manage answer if it is a promise or not
                    defferedResized = defferedResized || new $.Deferred().resolve();
                    defferedResized.done(function () {
                        widget.$gridWidget.i18n();

                        if (ensureVisible === true) {
                            //ensure the item is completly visible
                            widget.$gridWidget.ensureVisible(true);
                        }
						
                        widget.$gridWidget.find(".textfit").fitText();
						
                        //we ask for widget refresh data
                        updateWidgetPolling(widget).always(function() {
                           widget.viewModel.widgetApi.manageRollingTitle();
                           d.resolve();                           
                        });
                    });
                });
            });
        }
    }
    catch (e) {
        notifyWarning($.t("objects.widgetManager.widgetHasGeneratedAnExceptionDuringCallingMethod", { widgetName: widget.type, methodName: 'initialize' }));
        console.warn(e);
        d.reject();
    }
    
    return d.promise();
};

/**
 * Enable or disable customization on widget
 * @param {Widget} widget The widget to update the mode
 * @param {Boolean} enable If true configure the widget in customization mode, else in normal mode
 */
WidgetManager.enableCustomization = function (widget, enable) {
    var page = PageManager.getPage(widget.idPage);
    assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

    if (enable) {
        widget.$gridWidget.draggable("enable");
    } else {
        widget.$gridWidget.draggable("disable");
    }
    page.$grid.packery('layout');

    if ((enable) && (widget.resizable))
        widget.$gridWidget.resizable("enable");
    else
        widget.$gridWidget.resizable("disable");
};

/**
 * Create a new graphic Widget and add it to the grid
 * @param {Widget} widget The widget to add
 * @returns {object} The dom $(item)
 */
WidgetManager.createGridWidget = function (widget) {
    assert(widget !== undefined, "widget must be defined");

    var page = PageManager.getPage(widget.idPage);
    assert(!isNullOrUndefined(page), "page doesn't exist in PageManager");

    var domWidget = "<div class=\"widget\" page-id=\"" + widget.idPage + "\" widget-id=\"" + widget.id + "\"";

    var minX = 1;
    var maxX = Number.MAX_VALUE;
    var minY = 1;
    var maxY = Number.MAX_VALUE;

    if (!isNullOrUndefined(widget.package.dimensions)) {
        if (!isNullOrUndefined(widget.package.dimensions.min)) {
            if (!isNullOrUndefined(widget.package.dimensions.min.x))
                minX = widget.package.dimensions.min.x;
            if (!isNullOrUndefined(widget.package.dimensions.min.y))
                minY = widget.package.dimensions.min.y;
        }
        if (!isNullOrUndefined(widget.package.dimensions.max)) {
            if (!isNullOrUndefined(widget.package.dimensions.max.x))
                maxX = widget.package.dimensions.max.x;
            if (!isNullOrUndefined(widget.package.dimensions.max.y))
                maxY = widget.package.dimensions.max.y;
        }
    }

    minX *= Yadoms.gridWidth;
    maxX *= Yadoms.gridWidth;
    minY *= Yadoms.gridHeight;
    maxY *= Yadoms.gridHeight;

    //we save the information that says if we can resize the widget
    widget.resizable = ((minX !== maxX) || (minY !== maxY));

    domWidget += ">\n" +
        "<div class=\"grid-item-content\">\n" +
          "<div class=\"panel-widget-customization-overlay customization-item hidden\">\n" +
             "<div class=\"customizationToolbar widgetCustomizationToolbar\">";

    //the configuration button is visible only if there is a custom title or a confgiuration for this widget
    if ((parseBool(widget.package.hasTitle)) || (widget.package.configurationSchema)) {
        domWidget += "<div class=\"customizationButton widgetCustomizationButton btn-configure-widget\"><i class=\"fa fa-lg fa-cog\"></i></div>\n";
    }

    var type = widget.type;
    if (!isNullOrUndefined(widget.downgraded)) {
        type = WidgetManager.DeactivatedWidgetPackageName;
    }

    domWidget += "<div class=\"customizationButton widgetCustomizationButton btn-delete-widget\"><i class=\"fa fa-lg fa-trash-o\"></i></div>\n" +
             "</div>\n" +
        "</div>\n" +
        "<div class=\"panel panel-primary panel-widget widget-" + type + "\" >" +
            "<div class=\"panel-heading panel-widget-header\">" +
                "<div class=\"panel-widget-title-toolbar\" ></div>" +
            "<div class=\"panel-widget-title\" >" + widget.title + "</div>\n" +
            "</div>" +
            "<div class=\"panel-widget-body\" id=\"widget-" + widget.id + "\"  data-bind=\"template: { name: '" + type + "-template' }\"/>\n" +
        "</div>\n" +
    "</div>\n";
    
    var $domWidget = $(domWidget);
    page.$grid.append($domWidget).packery("appended", $domWidget);

    //we save the $dom infos into the object
    widget.$gridWidget = page.$grid.find(".widget[widget-id=" + widget.id + "]");
    widget.$header = widget.$gridWidget.find("div.panel-widget-header");
    widget.$toolbar = widget.$gridWidget.find("div.panel-widget-title-toolbar");
    widget.$content = widget.$gridWidget.find("div.panel-widget-body");

    //we check that the actual size fit allowed size
    widget.setHeight(Math.min(maxY, Math.max(widget.initialValues.sizeY, minY)));
    widget.setWidth(Math.min(maxX, Math.max(widget.initialValues.sizeX, minX)));

    //we manage draggablility and resizability

    widget.$gridWidget.draggable({
        handle: ".panel-widget-customization-overlay",
        stop: function () {
            page.$grid.packery("layout");
            /*// debounce
            setTimeout(function () {
                page.$grid.packery("layout");
            }, 10);*/
        }
    });

    page.$grid.packery('bindUIDraggableEvents', widget.$gridWidget);

    var resizeTimeout;
    widget.$gridWidget.resizable({
        grid: [Yadoms.gridWidth, Yadoms.gridHeight],
        minHeight: minY,
        minWidth: minX,
        maxHeight: maxY,
        maxWidth: maxX,
        disabled: true,
        resize: function () {
            // debounce
            if (resizeTimeout) {
                clearTimeout(resizeTimeout);
            }

            resizeTimeout = setTimeout(function () {
                try {
                    if (widget.viewModel.resized !== undefined)
					{
                        var defferedResult = widget.viewModel.resized();
                        //we manage answer if it is a promise or not
                        defferedResult = defferedResult || new $.Deferred().resolve();
                        defferedResult.always(function() {
                           widget.viewModel.widgetApi.manageRollingTitle();
                        });
					}
                }
                catch (e) {
                    notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", { widgetName: widget.type, methodName: 'resized' }));
                    console.warn(e);
                }
                page.$grid.packery("layout");
            }, 10);
        }
    });

    widget.$gridWidget.i18n();
};

/**
 * Update the layout of the widget
 */
WidgetManager.updateWidgetLayout = function (widget) {
    widget.$gridWidget.find(".textfit").fitText();
};

/**
 * Refresh widgets
 */
WidgetManager.refreshWidgets = function (widget) {
	if (widget.viewModel.configurationChanged !== undefined) {
	   widget.viewModel.configurationChanged();
	}	
};