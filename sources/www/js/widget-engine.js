/**
 * global viewModel object used to get the viewModelCtor of each kind of widget
 */
var widgetViewModelCtor = null;

var loadPagesNotification = null;

var widgetUpdateInterval;
var serverIsOnline;
var OfflineServerNotification = null;

var LastEventLogId = null;

var failGetEventCounter = 0;

function initializeWidgetEngine() {

    /**
     * Callback of the click on the add page button
     * Make lazy loading of the add page modal
     */
    $("#btn-add-page").click(function () {
        createOrUpdatePage();
    });

/**
     * Callback of the click on the add widget button
     * Make lazy loading of the add widget modal
     */
    $("#btn-add-widget").click(function() {
        modals.widgetAdd.load(function() { askWidgetPackages(); });
    });

    //we ask all widgets packages
    WidgetPackageManager.getAll(function () {
        console.timeStamp("entering WidgetPackageManager.getAll");
        //we show notification
        loadPagesNotification = notifyInformation($.t("mainPage.actions.loadingPages"));
        PageManager.getAll(function () {
            if (loadPagesNotification != null) {
                loadPagesNotification.close();
                if (loadPagesNotification.showing)
                    loadPagesNotification.$bar.dequeue();
                loadPagesNotification = null;
            }
            //we add all pages to the DOM
            $.each(PageManager.pages, function (index, currentPage) {
                PageManager.addToDom(currentPage);
            });

            //we deactivate the customization without launch save process
            exitCustomization(false);

            //we ensure that one page is selected
            PageManager.ensureOnePageIsSelected();

            //we ask for the last event to ask only those occurs after this one
            EventLoggerManager.getLast()
              .done(function (data) {
                  //we save the id of the last event
                  LastEventLogId = data.id;

                  //we can start the periodic update
                  serverIsOnline = true;
                  if (!WebSocketEngine.isActive())
                      widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalWithWebSocketDisabled);
                  else
                      widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);

                  //we update widget data
                  updateWidgetsPolling();
              });
        });
    });
}

function requestWidgets(page) {
    //we request widgets for the first page
    var loadWidgetsNotification = notifyInformation($.t("mainPage.actions.loadingWidgetsOfPage", { pageName: page.name }));

    //before making anything we empty the gridstack
    page.grid.remove_all();

    var d = WidgetManager.getWidgetOfPageFromServer(page)
    .done(function (list) {
        if (list != null) {
            WidgetManager.loadWidgets(list, page)
            .done(function () {
                if (!isNullOrUndefined(loadWidgetsNotification)) {
                    loadWidgetsNotification.close();
                    if (loadWidgetsNotification.showing)
                        loadWidgetsNotification.$bar.dequeue();
                    loadWidgetsNotification = null;
                }

                //we update the filter of the websocket
                updateWebSocketFilter();

            })
            .fail(function (errorMessage) {
                console.error(errorMessage);
                notifyError($.t("objects.widgetManager.loadingWidgetsError"));
            });
        }
    });
    return d.promise();
}

/**
 * Occurs when user click on a tab
 * @param pageId tab id clicked
 */
function tabClick(pageId) {

    //we check for widget loading if page is different than the current
    var currentPage = PageManager.getCurrentPage();

    if ((currentPage != null) && (currentPage.id == pageId))
        return;

    var page = PageManager.getPage(pageId);
    assert(page != null, "page Id doesn't exit");

    //and if it's not loaded for the moment
    if (!page.loaded) {
        requestWidgets(page).done(function () {
            //we poll all widget data
            updateWidgetsPolling();
        });
    } else {
        //we poll all widget data
        updateWidgetsPolling();
    }
}

function periodicUpdateTask() {
    //we first check if the server is online and only if it answer to the eventLog new messages
    //to do that we ask event message
    EventLoggerManager.getFrom(LastEventLogId).done(function (data) {
        //we reset the fail event counter
        failGetEventCounter = 0;

        //if we were offline we go back to online status
        if (!serverIsOnline) {
            serverIsOnline = true;
            //we signal that server has been back
            notifyInformation($.t("mainPage.notifications.connectionToServerHasBeenRestored"));
            //if the errorNotification is always visible we close it
            if (OfflineServerNotification != null) {
                OfflineServerNotification.close();
                if (OfflineServerNotification.showing)
                    OfflineServerNotification.$bar.dequeue();
                OfflineServerNotification = null;
            }
            //we change the interval period to the normal one
            clearInterval(widgetUpdateInterval);
            if (!WebSocketEngine.isActive())
                widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalWithWebSocketDisabled);
            else
                widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);

            //we reinitialize the websocket
            WebSocketEngine.initializeWebSocketEngine(function () {
                //web socket opened
                //we listen acquisitionupdate event
                $(document).on("acquisitionupdate", function (e, websocketData) {
                    var acq = AcquisitionManager.factory(websocketData.data);
                    dispatchToWidgets(acq);
                });

                //Maybe there is a lot of time between the turn off of the server and the turn on, so we must ask all widget
                //data to be sure that all information displayed are fresh
                updateWidgetsPolling();

                //we update the filter of the websockets to receive only wanted data
                updateWebSocketFilter();
            });
        }

        $.each(data.EventLogger, function (index, value) {
            console.debug("incoming event: " + JSON.stringify(value));
            var gravity;
            //the gravity of the noty depend on the code
            if ((value.code.toLowerCase() == "started") || (value.code.toLowerCase() == "stopped") || (value.code.toLowerCase() == "updated")) {
                gravity = "information";
            }
            else {
                gravity = "error";
            }

            var translation = "eventLogger." + value.code;
            notify(DateTimeFormatter.isoDateToString(value.date) + " " +
               $.t(translation, { "who": value.who, "what": value.what }), gravity);

            //we update the lastEvent Id Read
            LastEventLogId = value.id;
        });

        //we ask for widget's devices if web sockets are unsupported
        if (!WebSocketEngine.isActive())
            updateWidgetsPolling();
    })
    .fail(function () {
        if (serverIsOnline) {
            failGetEventCounter++;
            if (failGetEventCounter >= 3) {
                //we indicate that *server has passed offline
                serverIsOnline = false;
                OfflineServerNotification = notifyError($.t("mainPage.errors.youHaveBeenDisconnectedFromTheServerOrItHasGoneOffline"), "", false);
                //we change the interval period
                clearInterval(widgetUpdateInterval);
                widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalInOfflineMode);
                failGetEventCounter = 0;
                //we close the dashboard if shown
                $('#main-dashboard-modal').modal('hide');
                //we stop refresh timer of the dashboard if set
                if (periodicDashboardTask)
                    clearInterval(periodicDashboardTask);
            }
        }
        //if we are again offline there is nothing to do
    });
}

function dispatchToWidgets(acq) {
    assert(!isNullOrUndefined(acq), "data must be defined");

    var page = PageManager.getCurrentPage();
    if (page == null)
        return;

    $.each(page.widgets, function (widgetIndex, widget) {
        //we ask which devices are needed for this widget instance
        if (!isNullOrUndefined(widget.listenedKeywords)) {
            $.each(widget.listenedKeywords, function (keywordIndex, keywordId) {
                if (!isNullOrUndefined(keywordId)) {
                    //foreach device we ask for last values
                    if (keywordId == acq.keywordId) {
                        console.debug("onNewAcquisition : " + JSON.stringify(acq));
                        try {
                            //we signal the new acquisition to the widget if the widget support the method
                            if (widget.viewModel.onNewAcquisition !== undefined)
                                widget.viewModel.onNewAcquisition(keywordId, acq);
                        }
                        catch (e) {
                            console.error(widget.type + " has encouter an error in onNewAcquisition() method:" + e.message);
                        }

                        //we manage battery api toolbar
                        var $battery = widget.$toolbar.find(".widget-toolbar-battery");
                        if ($battery) {
                            if ($battery.attr("keywordId") == acq.keywordId) {
                                ToolbarApi.updateBatteryLevel(widget, acq.value);
                            }
                        }
                    }
                }
            });
        }
    });
}

function updateWebSocketFilter() {
    console.log("updateWebSocketFilter()");
    if (WebSocketEngine.isActive()) {
        var page = PageManager.getCurrentPage();
        if (page == null)
            return;

        var collection = [];

        //we build the collection of keywordId to ask
        $.each(page.widgets, function (widgetIndex, widget) {
            //we ask which devices are needed for this widget instance
            if (!isNullOrUndefined(widget.listenedKeywords)) {
                $.each(widget.listenedKeywords, function (keywordIndex, keywordId) {
                    if (!isNullOrUndefined(keywordId)) {
                        collection.push(keywordId);
                    }
                });
            }
        });

        WebSocketEngine.updateAcquisitionFilter(duplicateRemoval(collection));
    }
}

function updateWidgetsPolling() {
    //we browse each widget instance
    var page = PageManager.getCurrentPage();
    if (page == null)
        return;

    $.each(page.widgets, function (widgetIndex, widget) {
        //we ask which devices are needed for this widget instance
        updateWidgetPolling(widget);
    });
}

function updateWidgetPolling(widget) {
    if (!isNullOrUndefined(widget.listenedKeywords)) {
        $.each(widget.listenedKeywords, function (keywordIndex, keywordId) {
            if (!isNullOrUndefined(keywordId)) {
                //foreach device we ask for last values
                AcquisitionManager.getLastValue(keywordId, function (acquisition) {
                    //we signal the new acquisition to the widget if the widget support the method
                    if (widget.viewModel.onNewAcquisition !== undefined)
                        widget.viewModel.onNewAcquisition(keywordId, acquisition);
                });
            }
        });
    }
}