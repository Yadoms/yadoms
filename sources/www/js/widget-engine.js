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

var pagesLoaded = [];

var startTime = null;

/**
 * Return a Widget object from the gridster DOM object
 * @param tabElement tab hash concerned
 * @returns {Page}
 */
function getPageFromTabElement(tabElement)
{
   var res = tabElement.split("-");
   assert(res.length == 2, "tabElement must be formed #tab-idPage");
   return pageArray[res[1]];
}

/**
 * Return a Widget object from the gridster DOM object
 * @param $element gridster DOM object concerned
 * @returns {Widget}
 */
function getWidgetFromGridsterElement($element)
{
   var str = $element[0].id;
   var res = str.split("-");
   assert(res.length >= 3, "gridster id must be formed name-idPage-idWidget");
   return pageArray[res[res.length - 2]].widgets[res[res.length - 1]];
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

      $.each(data.data.page, function(index, value) {
         //foreach page
         var currentPage = new Page(value.id, value.name);

         var tabIdAsText = "tab-" + value.id;
         //pill creation
         $("div#tabContainer").find(".tab-content").append(
             "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\">" +
                 "<div class=\"gridster\">" +
                 "<ul></ul>" +
                 "</div>" +
                 "</div>");
         //page creation
         $("div#pageMenu").find("ul").append(
             "<li class=\"tabPagePills\" page-id=\"" + value.id + "\">" +
                 "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" +
                     value.name +
                     "<div class=\"pageCustomizationToolbar btn-group btn-group-sm customization-item pull-right hidden\">" +
                        "<button type=\"button\" class=\"btn btn-default\" title=\"Move to left\"><i class=\"glyphicon glyphicon-arrow-left\"></i></button>" +
                        "<button type=\"button\" class=\"btn btn-default\" title=\"Move to right\"><i class=\"glyphicon glyphicon-arrow-right\"></i></button>" +
                        "<button type=\"button\" class=\"btn btn-default\" title=\"Rename\"><i class=\"fa fa-pencil\"></i></button>" +
                        "<button type=\"button\" class=\"btn btn-default delete-page\" title=\"Delete\"><i class=\"fa fa-times\"></i></button>" +
                     "</div>" +
                 "</a>" +

             "</li>");

         //gridster creation
         currentPage.gridster = $("#" + tabIdAsText + " ul").gridster({
             page_object: pageArray[value.id],
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

         pageArray[currentPage.id] = currentPage;
      });

      //we activate first page on the pills
      $("div#pageMenu").find("ul li").first().addClass("active");

      //we remove the active class of all hidden gridster
      var $tabs = $("div.tab-content div");
      $tabs.not($tabs.first()).removeClass("active");

      $("div.widgetPage div.gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

      //we deactivate the customization
      enableGridsterCustomization(false);

      //we listen click event on tab click
      $('.page-tabs').bind('click', function (e) { return tabClick(getPageFromTabElement(e.target.hash)); } );
      requestWidgets(getCurrentPage());

    };
}

function requestWidgets(page) {
   //we request widgets for the first page
   loadWidgetsNotification = notify("Loading widgets for page " + page.name + " ...", "information", 0);

   //we save the information that the widgets for this page have already been asked
   pagesLoaded[page.id] = true;

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

         //we add tooltip to customize buttons
         $("li.widget button, li.tabPagePills button").tooltip({
            animated: 'fade',
            placement: 'bottom'
         });

         //we prevent from click on widget to be propagated on the rest of the window
         /*$(".widget").click(function(e) {
            e.stopPropagation();
         });*/

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
   assert(widget !== undefined, "createWidget function widget must be defined");

   return pageArray[widget.idPage].gridster.add_widget(
      "<li class=\"widget\" id=\"gridsterWidget-" + widget.idPage + "-" + widget.id +"\">" +
         "<div class=\"widgetCustomizationToolbar customization-item hidden\">" +
            "<div class=\"btn-group btn-group-sm\">" +
               "<button type=\"button\" class=\"btn btn-default\" title=\"Configure\"><i class=\"fa fa-cog\"></i></button>" +
               "<button type=\"button\" class=\"btn btn-default\" title=\"Delete\"><i class=\"fa fa-times\"></i></button>" +
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
   //we use to construct the viewModel of the current widget
   //noinspection JSPotentiallyInvalidConstructorUsage
   widget.viewModel = new widgetPackages[widget.name].viewModelCtor();
   widget.package = widgetPackages[widget.name].packageInformation;

   pageArray[widget.idPage].widgets[widget.id] = widget;

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
      pageArray[widget.idPage].gridster.set_widget_max_size(widget.$gridsterWidget, [widget.package.dimensions.max.x, widget.package.dimensions.max.y]);
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
}

/**
 * Return the current displayed page
 * @returns {Page}
 */
function getCurrentPage()
{
   return pageArray[$("ul.page-tabs li.active").attr("page-id")];
}

/**
 * Occurs when user click on a tab
 * @param page tab clicked
 */
function tabClick(page) {

   //we check for widget loading if page is different than the current
   if (getCurrentPage() == page)
      return;

   //and if it's not loaded for the moment
   if (!pagesLoaded[page.id])
   {
      requestWidgets(page);
   }
}