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

var webSocket = null;

function initializeWebSocketEngine() {

   if (!isNullOrUndefined(webSocket))
      webSocket.close();

   //we check if websocket are handled
   if(window.MozWebSocket) {
      window.WebSocket = window.MozWebSocket;
   }
   if(window.WebSocket) {
      webSocket = new WebSocket('ws://' + window.location.host + '/ws');

      webSocket.onopen = function() {
         console.debug('Web socket opened');
      };

      webSocket.onmessage = function(e) {
         if (!isNullOrUndefined(e)) {
            var websocketData = JSON.parse(e.data);
            if (!isNullOrUndefined(websocketData)) {
               var acq = AcquisitionManager.factory(websocketData.data);
               dispatchToWidgets(acq);
            }
         }
      };

      webSocket.onclose = function() {
         console.debug('Web socket closed');
      };
   }
   else {
      console.debug("Web socket unhandled");
   }
}

function initializeWidgetEngine() {

   /**
    * Callback of the click on the add widget button
    * Make lazy loading of the add widget modal
    */
   $("#btn-add-page").click(function() {
      createOrUpdatePage();
   });

   /**
    * Callback of the click on the add widget button
    * Make lazy loading of the add widget modal
    */
   $("#btn-add-widget").click(function() {
      //we make something only if there is some pages
      if (PageManager.pages.length == 0) {
         notifyError($.t("mainPage.errors.unableToAddWidgetWithoutPage"));
         return;
      }

      modals.widgetAdd.load(function() {askWidgetPackages();});
   });

   //we ask all widgets packages
   WidgetPackageManager.getAll(function() {
      //we show notification
      loadPagesNotification = notifyInformation($.t("mainPage.actions.loadingPages"));
      PageManager.getAll(function() {
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

         //we deactivate the customization
         enableGridsterCustomization(false);

         //we ensure that one page is selected
         PageManager.ensureOnePageIsSelected();

         //we ask for the last event to ask only those occurs after this one
         $.getJSON("/rest/eventLogger/last")
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("mainPage.errors.unableToGetEventLog"), JSON.stringify(data));
                  return;
               }
               //we save the id of the last event
               LastEventLogId = data.data.id;

               //we can start the periodic update
               serverIsOnline = true;
               if (isNullOrUndefined(webSocket))
                  widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalWithWebSocketDisabled);
               else
                  widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);

               //we update widget data
               updateWidgetsPolling();
            })
            .fail(function() { notifyError($.t("mainPage.errors.unableToGetEventLog")); });
      });
   });
}

function requestWidgets(page) {
   //we request widgets for the first page
   var loadWidgetsNotification = notifyInformation($.t("mainPage.actions.loadingWidgetsOfPage", {pageName : page.name}));

   WidgetManager.getWidgetOfPageFromServer(page, function(list) {
      if (list != null) {
         //we've got the list of widget
         $.each(list, function(index, widget) {
            WidgetManager.loadWidget(widget, page);
         });

         if (!isNullOrUndefined(loadWidgetsNotification)) {
            loadWidgetsNotification.close();
            if (loadWidgetsNotification.showing)
               loadWidgetsNotification.$bar.dequeue();
            loadWidgetsNotification = null;
         }

         //we update the filter of the websocket
         updateWebSocketFilter();
      }
   });
}
/*
function askForWidgetDelete(packageName, errorMessage) {
   assert(!isNullOrUndefined(packageName), "packageName must be defined in askForWidgetDelete()");
   assert(!isNullOrUndefined(errorMessage), "errorMessage must be defined in askForWidgetDelete()");

   notifyConfirm(errorMessage, "error",
      function($noty) {
         // this = button element
         // $noty = $noty element
         $noty.close();
         if ($noty.showing)
            $noty.$bar.dequeue();

         //we look for all widget of that type


         //we ask for deletion of the widget
         $.ajax({
            type: "DELETE",
            url: "/rest/widget/" + widget.id,
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("modals.delete-widget.errorDuringDeletingWidget"), JSON.stringify(data));
                  return;
               }
            })
            .fail(function(widgetName) { return function() {notifyError($.t("modals.delete-page.errorDuringDeletingWidgetNamed", {"widgetType" : widgetName}));};}(widget.type));
      },
      function($noty) {
         // this = button element
         // $noty = $noty element
         $noty.close();
         if ($noty.showing)
            $noty.$bar.dequeue();
      });
}
*/
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
   if (!page.loaded)
   {
      requestWidgets(page);
   }

   //we poll all widget data
   updateWidgetsPolling();
}

function periodicUpdateTask() {
   //we first check if the server is online and only if it answer to the eventLog new messages
   //to do that we ask event message
   $.ajax({
      url:"/rest/eventLogger/from/" + LastEventLogId,
      dataType:"json",
      timeout: 3000
   }).done(function(data) {
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
            if (isNullOrUndefined(webSocket))
               widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalWithWebSocketDisabled);
            else
               widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);

            //we reinitialize the websocket
            initializeWebSocketEngine();

            //Maybe there is a lot of time between the turn off of the server and the turn on, so we must ask all widget
            //data to be sure that all information displayed are fresh
            updateWidgetsPolling();

            //we update the filter of the websockets to receive only wanted data
            updateWebSocketFilter();
         }

         //if there is new messages we display them
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("mainPage.errors.errorDuringRequestingNewLogEvents"), JSON.stringify(data));
            return;
         }

         $.each(data.data.EventLogger, function(index, value) {
            var eventLogger = new EventLogger(value.id, value.date, value.code, value.who, value.what);
            if (eventLogger.code > 0) {
               notifyInformation(EventLoggerHelper.toString(eventLogger));
            }
            else {
               notifyError(EventLoggerHelper.toString(eventLogger), value.what, false);
            }

            //we update the lastEvent Id Read
            LastEventLogId = value.id;
         });

         //we ask for widget's devices if web sockets are unsupported
         if (isNullOrUndefined(webSocket))
            updateWidgetsPolling();
     })
     .fail(function() {
         if (serverIsOnline)
         {
            failGetEventCounter++;
            if (failGetEventCounter >= 3) {
               //we indicate that server has passed offline
               serverIsOnline = false;
               OfflineServerNotification = notifyError($.t("mainPage.errors.youHaveBeenDisconnectedFromTheServerOrItHasGoneOffline"), "", false);
               //we change the interval period
               clearInterval(widgetUpdateInterval);
               widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalInOfflineMode);
               failGetEventCounter = 0;
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

   $.each(page.widgets, function(widgetIndex, widget) {
      //we ask which devices are needed for this widget instance
      if (!isNullOrUndefined(widget.viewModel.getDevicesToListen)) {
         var list = widget.viewModel.getDevicesToListen();
         $.each(list, function(deviceIndex, device) {
            if (!isNullOrUndefined(device.keywordId)) {
               //foreach device we ask for last values
               if (device.keywordId == acq.keywordId) {
                  console.debug("Dispatch : " + JSON.stringify(acq));

                  //we dispatch the device to the widget if the widget support the method
                  if (widget.viewModel.dispatch !== undefined)
                        widget.viewModel.dispatch(device, acq);
               }
            }
         });
      }
   });
}

function updateWebSocketFilter() {
   console.log("updateWebSocketFilter()");
   if (!isNullOrUndefined(webSocket)) {
      var page = PageManager.getCurrentPage();
      if (page == null)
         return;

      var collection = [];

      //we build the collection of keywordId to ask
      $.each(page.widgets, function(widgetIndex, widget) {
         //we ask which devices are needed for this widget instance
         if (!isNullOrUndefined(widget.viewModel.getDevicesToListen)) {
            var list = widget.viewModel.getDevicesToListen();
            $.each(list, function(deviceIndex, device) {
               if (!isNullOrUndefined(device.keywordId)) {
                  collection.push(device.keywordId);
               }
            });
         }
      });

      webSocket.send(JSON.stringify({"type" : "acquisitionFilter", "data" : collection}));
   }
}

function updateWidgetsPolling() {
   //we browse each widget instance
   var page = PageManager.getCurrentPage();
   if (page == null)
      return;

   $.each(page.widgets, function(widgetIndex, widget) {
      //we ask which devices are needed for this widget instance
      updateWidgetPolling(widget);
   });
}

function updateWidgetPolling(widget) {
   if (!isNullOrUndefined(widget.viewModel.getDevicesToListen)) {
      var list = widget.viewModel.getDevicesToListen();
      $.each(list, function(deviceIndex, device) {
         if ((!isNullOrUndefined(device.deviceId)) && (!isNullOrUndefined(device.keywordId))) {
            //foreach device we ask for last values
            $.getJSON("/rest/acquisition/keyword/" + device.keywordId  + "/lastdata")
               .done(function(data) {
                  //we parse the json answer
                  if (data.result != "true")
                  {
                     notifyError($.t("mainPage.errors.errorDuringRequestingDeviceLastData"), JSON.stringify(data));
                     return;
                  }

                  console.debug("Dispatch : " + JSON.stringify(data.data));

                  var acq = AcquisitionManager.factory(data.data);

                  //we dispatch the device to the widget if the widget support the method
                  if (widget.viewModel.dispatch !== undefined)
                     widget.viewModel.dispatch(device, acq);
               });
            //we don't need to manage the fail because the server is online
            //it happens that server is offline but it will be shown next time by the first check
         }
      });
   }
}