/**
 * global viewModel object used to get the viewModelCtor of each kind of widget
 */
var widgetViewModelCtor = null;

/**
 * Array of widget during loading
 * @type {Array}
 */
var widgetArrayForLoading = [];

var loadWidgetsNotification = null;
var loadPagesNotification = null;

var startTime = null;

var widgetUpdateInterval;
var serverIsOnline;
var OfflineServerNotification = null;

var LastEventLogId = null;

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
      if (pageContainer.pages.length == 0) {
         notifyError($.t("mainPage.errors.unableToAddWidgetWithoutPage"));
         return;
      }

      modals.widgetAdd.load(function() {askWidgetPackages();});
   });
}

function initializePageEvents(page) {

   //we listen click event on tab click
   page.$tab.find("a").bind('click', function (e) {
      return tabClick($(e.currentTarget).parent().attr("page-id")); } );

   //we listen click event on rename click
   page.$tab.find('button.rename-page').bind('click', function (e) {
      createOrUpdatePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id")); } );

   //we listen click event on rename click
   page.$tab.find('button.delete-page').bind('click', function (e) {
      var pageId = $(e.currentTarget).parents("li.tabPagePills").attr("page-id");
      modals.pageDelete.load(function (pageId) {return function() {showDeletePageModal(pageId)}}(pageId));
   } );

   //we listen click event on move left click
   page.$tab.find('button.move-left-page').bind('click', function (e) {
      movePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"), "left"); } );

   //we listen click event on move right click
   page.$tab.find('button.move-right-page').bind('click', function (e) {
      movePage($(e.currentTarget).parents("li.tabPagePills").attr("page-id"), "right"); } );

   //we listen for click event with no code inside to help event transmission until exit customization
   page.$tab.bind('click', function (e) {
   } );

   //we ask for the last event to ask only those occurs after this one
   $.getJSON("/rest/eventLogger/last")
      .done(requestLastEventLoggerDone())
      .fail(function() { notifyError($.t("mainPage.errors.unableToGetEventLog")); });
}

function requestLastEventLoggerDone() {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("mainPage.errors.errorDuringRequestingLastEventLog"), JSON.stringify(data));
         return;
      }
      //we save the id of the last event
      LastEventLogId = data.data.id;

      //we can start the periodic update
      serverIsOnline = true;
      widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);
   }
}

function initializeWidgetEvents(widget) {
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
}

/**
 * Return the widget object attached to gridster element
 * @param $gridsterElement
 * @returns {Widget}
 */
function getWidgetFromGridsterElement($gridsterElement) {
   var pageId = $gridsterElement.attr("page-id");
   var page = pageContainer.getPage(pageId);
   assert(page != null, "Unable to find page Id");
   var widgetId = $gridsterElement.attr("widget-id");
   var widget = page.getWidget(widgetId);
   assert(widget != null, "Unable to find widget Id in the page");
   return widget;
}

function movePage(pageId, direction) {

   var pageToMove = pageContainer.getPage(pageId);
   assert(pageToMove != null, "Unable to find page to move");

   var nearestId = null;
   var nearestPageOrder;

   if (direction == "right") {
      //we search the nearest upper pageOrder than our
      nearestPageOrder = Infinity;

      $.each(pageContainer.pages, function (index, currentPage) {
         //if the current pageOrder is greater than our and smaller than the nearest it's the new nearest
         if ((currentPage.pageOrder > pageToMove.pageOrder) && (currentPage.pageOrder < nearestPageOrder)) {
            nearestPageOrder = currentPage.pageOrder;
            nearestId = currentPage.id;
         }
      });
   }
   else {
      //we search the nearest lower pageOrder than our
      nearestPageOrder = -Infinity;

      $.each(pageContainer.pages, function (index, currentPage) {
         //if the current pageOrder is greater than our and smaller than the nearest it's the new nearest
         if ((currentPage.pageOrder < pageToMove.pageOrder) && (currentPage.pageOrder > nearestPageOrder)) {
            nearestPageOrder = currentPage.pageOrder;
            nearestId = currentPage.id;
         }
      });
   }

   if (nearestId != null) {

      var nearestPage = pageContainer.getPage(nearestId);
      assert(nearestPage != null, "Unable to find nearest page to move");

      //we can move pageOrder
      //we move it into the array and send the complete collection to rest server
      nearestPage.pageOrder = pageToMove.pageOrder;
      pageToMove.pageOrder = nearestPageOrder;

      //we make an array of pages to send to rest server
      $.ajax({
         type: "PUT",
         url: "/rest/page",
         data: JSON.stringify(pageContainer.pages),
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
      .done(savePageMoveDone(pageId, nearestId, direction))
      .fail(function() {notifyError($.t("mainPage.errors.errorDuringSavingPagePosition"))});
   }
}

function savePageMoveDone(pageId, nearestId, direction) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("mainPage.errors.errorDuringSavingPagePosition"), JSON.stringify(data));
         console.error(data.message);
         return;
      }

      var pageToMove = pageContainer.getPage(pageId);
      assert(pageToMove != null, "Unable to find page to move");

      var nearestPage = pageContainer.getPage(nearestId);
      assert(nearestPage != null, "Unable to find nearest page to move");

      //we move the tab dynamically
      var tabDOMElement = pageToMove.$tab.detach();
      if (direction == "right") {
         //nearestPage.$tab.insertAfter(tabDOMElement);
         tabDOMElement.insertAfter(nearestPage.$tab);
      }
      else {
         //nearestPage.$tab.insertBefore(tabDOMElement);
         tabDOMElement.insertBefore(nearestPage.$tab);
      }
   };
}

/**
 * Launch page request from the REST server
 */
function requestPages()
{
   startTime = new Date();
   loadPagesNotification = notifyInformation($.t("mainPage.actions.loadingPages"));
   //we get pages
   $.getJSON("/rest/page")
      .done(requestPageDone())
      .fail(function() { notifyError($.t("mainPage.errors.unableToGetPages")); });
}

/**
 * Callback of the request page from the rest server
 * @returns {Function}
 */
function requestPageDone()
{
   return function( data ) {
      if (loadPagesNotification != null) {
         loadPagesNotification.close();
         loadPagesNotification = null;
      }

      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("mainPages.errors.errorDuringRequestingPages"), JSON.stringify(data));
         return;
      }

      data.data.page.sort(function(a, b) { return a.pageOrder > b.pageOrder; });

      $.each(data.data.page, function(index, value) {
         //foreach page
         var currentPage = new Page(value.id, decodeURIComponent(value.name), value.pageOrder);
         pageContainer.addPage(currentPage);
         addPageToIHM(currentPage);
      });

      //we deactivate the customization
      enableGridsterCustomization(false);

      ensureOnePageIsSelected();
   };
}

function addPageToIHM(page) {
   var tabIdAsText = "tab-" + page.id;
   //pill creation

   var container  = $("div#pageMenu").find("ul").append(
      "<li class=\"tabPagePills\" page-id=\"" + page.id + "\">" +
         "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
         "<span>" + page.name + "</span>" +
         "<div class=\"pageCustomizationToolbar btn-group btn-group-sm customization-item pull-right hidden\">" +
         "<button type=\"button\" class=\"btn btn-default move-left-page\" title=\"Move to left\" data-i18n=\"[title]mainPage.customization.moveToLeft\"><i class=\"glyphicon glyphicon-arrow-left\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default move-right-page\" title=\"Move to right\" data-i18n=\"[title]mainPage.customization.moveToRight\"><i class=\"glyphicon glyphicon-arrow-right\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default rename-page\" title=\"Rename\" data-i18n=\"[title]mainPage.customization.rename\"><i class=\"glyphicon glyphicon-pencil\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default delete-page\" title=\"Delete\" data-i18n=\"[title]mainPage.customization.delete\"><i class=\"glyphicon glyphicon-trash\"></i></button>" +
         "</div>" +
         "</a>" +
         "</li>");

   page.$tab = container.find("li[page-id=\"" + page.id + "\"]");

   //i18n of page tab
   page.$tab.i18n();

   //page creation
   container = $("div#tabContainer").find(".tab-content").append(
      "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\" page-id=\"" + page.id + "\">" +
         "<div class=\"gridster\">" +
         "<ul></ul>" +
         "</div>" +
         "</div>");

   //we save the content of the page dom node
   page.$content = container.find("div#" + tabIdAsText);

   //gridster creation
   page.gridster = page.$content.find("ul").gridster({
      widget_margins: [gridMargin, gridMargin],
      widget_base_dimensions: [gridWidth, gridWidth],
      min_cols: numberOfColumns,
      max_cols: numberOfColumns,
      resize: {
         enabled: true,
         resize: function(e, ui, $widget) {
            var widgetObject = getWidgetFromGridsterElement($widget);
            try
            {
               if (widgetObject.viewModel.resized !== undefined)
                  widgetObject.viewModel.resized();
            }
            catch (e)
            {
               notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'resized'}));
               console.warn(e);
            }
         },
         stop: function(e, ui, $widget) {
            var widgetObject = getWidgetFromGridsterElement($widget);
            try
            {
               if (widgetObject.viewModel.resized !== undefined)
                  widgetObject.viewModel.resized();
            }
            catch (e)
            {
               notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'resized'}));
               console.warn(e);
            }
         }
      }
   }).data('gridster');

   //we remove the active class of hidden gridster
   $("div#" + tabIdAsText).removeClass("active");

   $("div#" + tabIdAsText + " .gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

   //we check if we are in customization we must apply customization on the new page
   if (customization)
   {
      page.$tab.find(".customization-item").removeClass("hidden");
   }

   initializePageEvents(page);
}

function requestWidgets(page) {
   //we request widgets for the first page
   loadWidgetsNotification = notifyInformation($.t("mainPage.actions.loadingWidgetsOfPage", {pageName : page.name}));

   //we save the information that the widgets for this page have already been asked
   page.loaded = true;

   $.getJSON("/rest/page/" + page.id + "/widget")
      .done(requestWidgetsDone())
      .fail(function() {notifyError($.t("mainPage.errors.unableToGetWidgets"))});
}

/**
 * Callback of the request widgets from the rest server
 * @returns {Function}
 */
function requestWidgetsDone() {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("mainPage.errors.errorDuringRequestingWidgets"), JSON.stringify(data));
         return;
      }

      //if there is no widget it's finished
      if (data.data.widget.length == 0) {
         //we close the noty
         if (loadWidgetsNotification != null) {
            loadWidgetsNotification.close();
            loadWidgetsNotification = null;
         }
         return;
      }

      //we list all kind of widget we have
      var widgetTypes = [];
      $.each(data.data.widget, function(index, value) {
         var w = WidgetManager.factory(value);
         widgetArrayForLoading.push(w);
         if (widgetTypes.indexOf(value.name) == -1)
            widgetTypes.push(value.name);
      });

      //we've got the list of widget type to ask
      $.each(widgetTypes, function(index, value) {
         askForWidgetPackage(value);
      });
   };
}

function askForWidgetPackage(packageName) {
   widgetPackages[packageName] = new WidgetPackage();
   //we ask for the view
   $.get("widgets/" + packageName + "/view.html")
      .done(getWidgetViewDone(packageName))
      .fail(function(pkg) { return function() {askForWidgetDelete(pkg, $.t("mainPage.errors.partOfWidgetIsMissing", {widgetName : pkg}));};}(packageName));
}

/**
 * Callback of the request for the view of the widget from the web server
 * @param packageName package concerned
 * @returns {Function}
 */
function getWidgetViewDone(packageName) {
   return function( data ) {
      $("div#templates").append(data);
      //we indicate that the view for this king of widget has been lazy loaded
      widgetPackages[packageName].viewHasBeenDownloaded = true;

      i18n.options.resGetPath = 'widgets/__ns__/locales/__lng__.json';
      i18n.loadNamespace(packageName);

      //we get script to execute for this widget
      askViewModelCtor(packageName);
   };
}

function askViewModelCtor(packageName) {
   widgetViewModelCtor = null;
   $.getScript("widgets/" + packageName + "/viewModel.js")
         .done(getWidgetViewModelConstructorDone(packageName))
         .fail(function(pkg) { return function() {askForWidgetDelete(pkg, $.t("mainPage.errors.partOfWidgetIsMissing", {widgetName : pkg}));};}(packageName));
}

/**
 * Callback for the request of the viewModel of a widget from the web server
 * @param packageName package concerned
 * @returns {Function}
 */
function getWidgetViewModelConstructorDone(packageName) {
   //viewModel.js has been executed
   //noinspection JSUnusedLocalSymbols
   return function(data, textStatus, jqxhr) {
      //we save the ctor in the map
      widgetPackages[packageName].viewModelCtor = widgetViewModelCtor;

      //we ask to add package information to the current widget package
      askForPackageInformation(packageName);
   };
}

function askForPackageInformation(packageName)
{
   $.getJSON( "widgets/" + packageName + "/package.json")
      .done(getWidgetPackageInformationDone(packageName))
      .fail(function(pkg) { return function() {askForWidgetDelete(pkg, $.t("mainPage.errors.partOfWidgetIsMissing", {widgetName : pkg}));};}(packageName));
}

/**
 * Callback of the request Package information of a widget
 * @param packageName package concerned
 * @returns {Function}
 */
function getWidgetPackageInformationDone(packageName)
{
   return function( data ) {
      //we set the min and max size if they are defined
      assert(data !== undefined, "Configuration of widget must be defined");

      //we save the package information in the map
      widgetPackages[packageName].packageInformation = data;

      //all data for this package have been downloaded
      //we look for all widgets of this kind
      var i = widgetArrayForLoading.length - 1;
      while (i >= 0) {
         var widget = widgetArrayForLoading[i];
         if (widget.name == packageName) {
            widgetArrayForLoading.splice(i, 1);
            addWidgetToIHM(widget);
         }
         i--;
      }
      //we have finished to load every widgets on this page
      if (widgetArrayForLoading.length == 0) {

         //we close the noty
         if (loadWidgetsNotification != null) {
            loadWidgetsNotification.close();
            loadWidgetsNotification = null;
         }

         console.log('Widgets loaded in ' + (new Date() - startTime) + " ms");
      }
   };
}

function askForWidgetDelete(packageName, errorMessage) {
   assert(!isNullOrUndefined(packageName), "packageName must be defined in askForWidgetDelete()");
   assert(!isNullOrUndefined(errorMessage), "errorMessage must be defined in askForWidgetDelete()");

   notifyConfirm(errorMessage, "error",
      function($noty) {
         // this = button element
         // $noty = $noty element
         $noty.close();

         //we must retrieve which widgets are concerned by this package and we ask for widget deletion
         var i = widgetArrayForLoading.length - 1;
         while (i >= 0) {
            var widget = widgetArrayForLoading[i];
            if (widget.name == packageName) {
               widgetArrayForLoading.splice(i, 1);
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
                  .fail(function(widgetName) { return function() {notifyError($.t("modals.delete-page.errorDuringDeletingWidgetNamed", {"widgetType" : widgetName}));};}(widget.name));
            }
            i--;
         }
      },
      function($noty) {
         // this = button element
         // $noty = $noty element
         $noty.close();
      });
}

/**
 * Graphically add the widget to the correct tab
 * @param widget widget to add
 */
function addWidgetToIHM(widget) {
   if (widgetPackages[widget.name] === undefined)
      widgetPackages[widget.name] = new WidgetPackage();

   //if we haven't yet downloaded the view we ask for lazy load
   if (!widgetPackages[widget.name].viewHasBeenDownloaded) {
      //we indicate that we have this widget to load
      widgetArrayForLoading.push(widget);
      $.get("widgets/" + widget.name + "/view.html")
         .done(getWidgetViewDone(widget.name))
         .fail(function(pkg) { return function() {askForWidgetDelete(pkg, $.t("mainPage.errors.partOfWidgetIsMissing", {widgetName : pkg}));};}(widget.name));
   }
   else {
      //if we haven't yet downloaded the viewModelCtor we ask for lazy load
      if ((widgetPackages[widget.name] === undefined) || (widgetPackages[widget.name].viewModelCtor == null)) {
         //we indicate that we have this widget to load
         widgetArrayForLoading.push(widget);
         askViewModelCtor(widget.name);
      }
      else {
         //if we haven't yet downloaded the package.json file we ask for lazy load
         if ((widgetPackages[widget.name] === undefined) || (widgetPackages[widget.name].packageInformation == null)) {
            //we indicate that we have this widget to load
            widgetArrayForLoading.push(widget);
            askForPackageInformation(widget.name);
         }
         else
         {
            //we have all information about the package we can create the widget
            finalizeWidgetCreation(widget);
         }
      }
   }
}

/**
 * Create a new graphic Widget and add it to the corresponding gridster
 * @param widget widget to add
 * @returns {gridster}
 */
function createGridsterWidget(widget) {
   assert(widget !== undefined, "widget must be defined");
   var page = pageContainer.getPage(widget.idPage);
   assert(page != null, "page doesn't exist in pageContainer");

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

/**
 * This function end the widget creation process
 * @param widget
 */
function finalizeWidgetCreation(widget) {
   var page = pageContainer.getPage(widget.idPage);
   assert(page != null, "page doesn't exist in pageContainer");

   //we use to construct the viewModel of the current widget
   //noinspection JSPotentiallyInvalidConstructorUsage
   widget.viewModel = new widgetPackages[widget.name].viewModelCtor();
   widget.package = widgetPackages[widget.name].packageInformation;

   //last configuration of properties
   var widgetDivId = "widget-" + widget.id;
   widget.$gridsterWidget = createGridsterWidget(widget);
   widget.$div = $("div#" + widgetDivId);

   //we check if we are in customization we must apply customization on the new item
   if (customization)
   {
      widget.$gridsterWidget.find(".customization-item").removeClass("hidden");
      widget.$gridsterWidget.addClass("liWidgetCustomization");
   }

   //we apply binding to the view
   ko.applyBindings(widget.viewModel, widget.$div[0]);
   //we add minimum dimension constraint to the gridster widget
   if (widget.package.dimensions.min !== undefined)
   {
      //min dimension is set
      assert((widget.package.dimensions.min.x !== undefined) && (widget.package.dimensions.min.y !== undefined), "You can't set only one axe of the widget minimum dimension");
      widget.$gridsterWidget.data('coords').grid.min_size_x = widget.package.dimensions.min.x;
      widget.$gridsterWidget.data('coords').grid.min_size_y = widget.package.dimensions.min.y;
   }

   if (widget.package.dimensions.max !== undefined)
   {
      //min dimension is set
      assert((widget.package.dimensions.max.x !== undefined) && (widget.package.dimensions.max.y !== undefined), "You can't set only one axe of the widget maximum dimension");
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
      notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'initialize'}));
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
      notifyWarning($.t("widgets.errors.widgetHasGeneratedAnExceptionDuringCallingMethod", {widgetName : widget.name, methodName : 'configurationChanged'}));
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

   initializeWidgetEvents(widget);

   widget.$div.i18n();

   //we add the widget to the collection
   page.addWidget(widget);
}

function ensureOnePageIsSelected() {
   //if there is no page selected we select the first one
   if (getCurrentPage() == null) {
      //we selected the first page
      $.each(pageContainer.pages, function (index, currentPage) {
         currentPage.$tab.find("a").trigger("click");
         return false;
      });
   }
}

/**
 * Return the current displayed page
 * @returns {Page}
 */
function getCurrentPage()
{
   var pageId = $("ul.page-tabs li.active").attr("page-id");
   return pageContainer.getPage(pageId);
}

/**
 * Occurs when user click on a tab
 * @param pageId tab id clicked
 */
function tabClick(pageId) {

   //we check for widget loading if page is different than the current
   var currentPage = getCurrentPage();

   if ((currentPage != null) && (currentPage.id == pageId))
      return;

   var page = pageContainer.getPage(pageId);
   assert(page != null, "page Id doesn't exit");

   //and if it's not loaded for the moment
   if (!page.loaded)
   {
      requestWidgets(page);
   }
}

function periodicUpdateTask() {
   //we first check if the server is online and only if it answer to the eventLog new messages
   //to do that we ask event message
   $.ajax({
      url:"/rest/eventLogger/from/" + LastEventLogId,
      dataType:"json",
      timeout: 3000
   }).done(function(data) {
         //if we were offline we go back to online status
         if (!serverIsOnline) {
            serverIsOnline = true;
            //we signal that server has been back
            notifyInformation($.t("mainPage.notifications.connectionToServerHasBeenRestored"));
            //if the errorNotification is always visible we close it
            if (OfflineServerNotification != null) {
               OfflineServerNotification.close();
               OfflineServerNotification = null;
            }
            //we change the interval period to the normal one
            clearInterval(widgetUpdateInterval);
            widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateInterval);
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

         //we ask for widget's devices
         updateWidgets();
     })
     .fail(function() {
         if (serverIsOnline)
         {
            //we indicate that server has passed offline
            serverIsOnline = false;
            OfflineServerNotification = notifyError($.t("mainPage.errors.youHaveBeenDisconnectedFromTheServerOrItHasGoneOffline"), "", false);
            //we change the interval period
            clearInterval(widgetUpdateInterval);
            widgetUpdateInterval = setInterval(periodicUpdateTask, UpdateIntervalInOfflineMode);
         }
         //if we are again offline there is nothing to do
     });
}

function updateWidgets() {
   //we browse each widget instance
   var page = getCurrentPage();
   if (page == null)
      return;

   $.each(page.widgets, function(widgetIndex, widget) {
      //we ask which devices are needed for this widget instance
      if (!isNullOrUndefined(widget.viewModel.getDevicesToListen)) {
         var list = widget.viewModel.getDevicesToListen();
         $.each(list, function(deviceIndex, device) {
            if ((!isNullOrUndefined(device.deviceId)) && (!isNullOrUndefined(device.keywordId))) {
               //foreach device we ask for last values
               $.getJSON("/rest/acquisition/keyword/" + device.keywordId  + "/lastdata")
                  .done(dispatchDeviceDataToWidget(device, widget));
                  //we don't need to manage the fail because the server is online
                  //it happens that server is offline but it will be shown next time by the first check
            }
         });
      }
   });
}

function dispatchDeviceDataToWidget(device, widget) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("mainPage.errors.errorDuringRequestingDeviceLastData"), JSON.stringify(data));
         return;
      }

      console.debug("Dispatch : " + JSON.stringify(data.data));

      //we dispatch the device to the widget if the widget support the method
      if (widget.viewModel.dispatch !== undefined)
         widget.viewModel.dispatch(device, data.data);
   };
}