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
         notifyError("You must have at least one page to add widget");
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
}

function initializeWidgetEvents(widget) {
   //we listen click event on configure click
   widget.$gridsterWidget.find('button.configure-widget').bind('click', function (e) {
      var widgetDOMElement = $(e.currentTarget).parents("li.widget");
      var pageId = widgetDOMElement.attr("page-id");
      var widgetId = widgetDOMElement.attr("widget-id");
      modals.widgetConfiguration.load(function (pageId, widgetId) {return function() {showWidgetConfigurationModal(pageId, widgetId)}}(pageId, widgetId));
   });

   //we listen click event on delete click
   widget.$gridsterWidget.find('button.delete-widget').bind('click', function (e) {
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
      .fail(function() {notifyError("Unable to save page position")});
   }
}

function savePageMoveDone(pageId, nearestId, direction) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during saving page position");
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
   loadPagesNotification = notify("Loading pages ...", "information", 0);
   //we get pages
   $.getJSON("/rest/page")
      .done(requestPageDone())
      .fail(function() { notifyError("Unable to get Pages"); });
}

/**
 * Callback of the request page from the rest server
 * @returns {Function}
 */
function requestPageDone()
{
   return function( data ) {
       //console.log('requestPageDone()' + (new Date() - startTime));

      if (loadPagesNotification != null) {
         loadPagesNotification.close();
         loadPagesNotification = null;
      }

      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during requesting pages");
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
         "<button type=\"button\" class=\"btn btn-default move-left-page\" title=\"Move to left\"><i class=\"glyphicon glyphicon-arrow-left\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default move-right-page\" title=\"Move to right\"><i class=\"glyphicon glyphicon-arrow-right\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default rename-page\" title=\"Rename\"><i class=\"fa fa-pencil\"></i></button>" +
         "<button type=\"button\" class=\"btn btn-default delete-page\" title=\"Delete\"><i class=\"fa fa-times\"></i></button>" +
         "</div>" +
         "</a>" +
         "</li>");

   page.$tab = container.find("li[page-id=\"" + page.id + "\"]");

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
               console.log("the widget " + widget.name + " has generated an exception :");
               console.log(e);
               notifyWarning("The widget " + widget.name + " has generated an exception");
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
               notifyWarning("The widget " + widget.name + " has generated an exception");
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
   loadWidgetsNotification = notify("Loading widgets for page " + page.name + " ...", "information", 0);

   //we save the information that the widgets for this page have already been asked
   page.loaded = true;

   $.getJSON("/rest/page/" + page.id + "/widget")
      .done(requestWidgetsDone())
      .fail(function() {notifyError("Unable to get Widgets")});
}

/**
 * Callback of the request widgets from the rest server
 * @returns {Function}
 */
function requestWidgetsDone()
{
   return function(data) {
      //console.log('requestWidgetsDone()' + (new Date() - startTime));
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during requesting widgets");
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
      var widgetTypes = new Array();
      $.each(data.data.widget, function(index, value) {
         var w = new Widget(value.id, value.idPage, value.name, value.sizeX, value.sizeY, value.positionX, value.positionY, value.configuration);
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

function askForWidgetPackage(packageName)
{
   //console.log('askForWidgetPackage(' + packageName + ')' + (new Date() - startTime));
   widgetPackages[packageName] = new WidgetPackage();
   //we ask for the view
   $.get("widgets/" + packageName + "/view.html")
      .done(getWidgetViewDone(packageName))
      .fail(function(packageName) { return function() {notifyError("Unable to get view of the widget package " + packageName) }}(packageName));
}

/**
 * Callback of the request for the view of the widget from the web server
 * @param packageName package concerned
 * @returns {Function}
 */
function getWidgetViewDone(packageName)
{
   return function( data ) {
      //console.log('getWidgetViewDone(' + packageName + ')' + (new Date() - startTime));
      $("div#templates").append(data);
      //we indicate that the view for this king of widget has been lazy loaded
      widgetPackages[packageName].viewHasBeenDownloaded = true;
      //we get script to execute for this widget
      askViewModelCtor(packageName);
   };
}

function askViewModelCtor(packageName)
{
   //console.log('askViewModelCtor(' + packageName + ')' + (new Date() - startTime));
   widgetViewModelCtor = null;
   try
   {
      $.getScript("widgets/" + packageName + "/viewModel.js")
         .done(getWidgetViewModelConstructorDone(packageName))
         .fail(function(packageName) { return function() {notifyError("Unable to get viewModel of the widget package " + packageName)}}(packageName));
   }
   catch (e)
   {
      notifyWarning("The widget " + packageName + " has generated an exception");
      console.warn(e);
   }
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
      //console.log('getWidgetViewModelConstructorDone(' + packageName + ')' + (new Date() - startTime));
      //we save the ctor in the map
      widgetPackages[packageName].viewModelCtor = widgetViewModelCtor;

      //we ask to add package information to the current widget package
      askForPackageInformation(packageName);
   };
}

function askForPackageInformation(packageName)
{
   //console.log('askForPackageInformation(' + packageName + ')' + (new Date() - startTime));
   $.getJSON( "widgets/" + packageName + "/package.json")
      .done(getWidgetPackageInformationDone(packageName))
      .fail(function(packageName) { return function() {notifyError("Unable to get the configuration of the widget package " + packageName)}}(packageName));
}

/**
 * Callback of the request Package information of a widget
 * @param packageName package concerned
 * @returns {Function}
 */
function getWidgetPackageInformationDone(packageName)
{
   return function( data ) {
      //console.log('getWidgetPackageInformationDone(' + packageName + ')' + (new Date() - startTime));
      //we set the min and max size if they are defined
      assert(data !== undefined, "Configuration of widget must be defined");

      //we save the package information in the map
      widgetPackages[packageName].packageInformation = data;

      //all data for this package have been downloaded
      //we look for all widgets of this kind
      i = widgetArrayForLoading.length - 1;
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

         //we prevent from click on widget to be propagated on the rest of the window
         $(".widget").click(function(e) {
            e.stopPropagation();
         });

         $(".delete-page").click(function(e) {
            e.stopPropagation();
         });

         //we close the noty
         if (loadWidgetsNotification != null) {
            loadWidgetsNotification.close();
            loadWidgetsNotification = null;
         }

         console.log('Widgets loaded in ' + (new Date() - startTime) + " ms");
      }

   };
}

/**
 * Graphically add the widget to the correct tab
 * @param widget widget to add
 */
function addWidgetToIHM(widget) {
   //console.log('addWidgetToIHM(' + widget.id + ')' + (new Date() - startTime));
   if (widgetPackages[widget.name] === undefined)
      widgetPackages[widget.name] = new WidgetPackage();

   //if we haven't yet downloaded the view we ask for lazy load
   if (!widgetPackages[widget.name].viewHasBeenDownloaded) {
      //we indicate that we have this widget to load
      widgetArrayForLoading.push(widget);
      $.get("widgets/" + widget.name + "/view.html")
         .done(getWidgetViewDone(widget.name))
         .fail(function(widget) { return function() {notifyError("Unable to get view of the widget " + widget.name) }}(widget));
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

   return  page.gridster.add_widget(
      "<li class=\"widget\" page-id=\"" + widget.idPage + "\" widget-id=\"" + widget.id +"\">" +
         "<div class=\"widgetCustomizationToolbar customization-item hidden\">" +
            "<div class=\"btn-group btn-group-sm\">" +
               "<button type=\"button\" class=\"btn btn-default configure-widget\" title=\"Configure\"><i class=\"fa fa-cog\"></i></button>" +
               "<button type=\"button\" class=\"btn btn-default delete-widget\" title=\"Delete\"><i class=\"fa fa-times\"></i></button>" +
            "</div>" +
         "</div>" +
         "<div id=\"widget-" + widget.id + "\" class=\"widgetDiv\" data-bind=\"template: { name: '" + widget.name + "-template', data: data }\"/>" +
         "</li>", widget.sizeX, widget.sizeY, widget.positionX, widget.positionY);
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

   //console.log('before binding(' + widget.id + ')' + (new Date() - startTime));
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
      var page = pageContainer.getPage(widget.idPage);
      assert(page != null, "PageId not found");
      page.gridster.set_widget_max_size(widget.$gridsterWidget, [widget.package.dimensions.max.x, widget.package.dimensions.max.y]);
   }

   //we initialize the widget
   try
   {
      if (widget.viewModel.initialize !== undefined)
         widget.viewModel.initialize(widget);

      if (widget.viewModel.resized !== undefined)
         widget.viewModel.resized();
   }
   catch (e)
   {
      notifyWarning("The widget " + widget.name + " has generated an exception");
      console.warn(e);
   }
   initializeWidgetEvents(widget);

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