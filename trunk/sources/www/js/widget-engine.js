/**
 * global viewModel object used to get the viewModel of each widget
 */
var widgetViewModel = null;

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
                 "<a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" + value.name + "</a>" +
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

         //we request widgets for this page
         $.getJSON("/rest/page/" + currentPage.id + "/widget")
           .done(requestWidgetsDone())
           .fail(function(page) { return function() {notifyError("Unable to get Widgets for page " + page.name)}}(currentPage));
      });

      //we activate first page on the pills
      $("div#pageMenu").find("ul li").first().addClass("active");

      //we remove the active class of all hidden gridster
      var $tabs = $("div.tab-content div");
      $tabs.not($tabs.first()).removeClass("active");

      $("div.widgetPage div.gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

      //we deactivate the customization
      enableGridsterCustomization(false);
    };
}

/**
 * Callback of the request widgets from the rest server
 * @returns {Function}
 */
function requestWidgetsDone()
{
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during requesting widgets");
         return;
      }
      $.each(data.data.widget, function(index, value) {
         var w = new Widget(value.id, value.idPage, value.name, value.sizeX, value.sizeY, value.positionX, value.positionY, value.configuration);
         addWidgetToIHM(w);
      });
   };
}

/**
 * Graphically add the widget to the correct tab
 * @param widget widget to add
 */
function addWidgetToIHM(widget)
{
   pageArray[widget.idPage].widgets[widget.id] = widget;
   //for each widget we add view, viewModel and configuration of the widget
   $.get("widgets/" + widget.name + "/view.html")
      .done(getWidgetViewDone(pageArray[widget.idPage], widget))
      .fail(function(widget) { return function() {notifyError("Unable to get view of the widget " + widget.name) }}(widget));
}

/**
 * Callback of the request for the view of the widget from the web server
 * @param page page concerned
 * @param widget widget concerned
 * @returns {Function}
 */
function getWidgetViewDone(page, widget)
{
   return function( data ) {
      $("div#templates").append(data);
      //we get script to execute for this widget
      //$.getScript( "widgets/" + widget.name + "/viewModel.js", getWidgetViewModelDone(page, widget));
      $.getScript("widgets/" + widget.name + "/viewModel.js")
         .done(getWidgetViewModelDone(page, widget))
         .fail(function(widget) { return function() {notifyError("Unable to get viewModel of the widget " + pageArray[pageId].widgets[widgetId].name)}}(widget));
   };
}

/**
 * Create a new graphic Widget and add it to the corresponding gridster
 * @param page page concerned
 * @param widget widget to add
 * @returns {gridster}
 */
function createWidget(page, widget) {
    assert(page !== undefined, "createWidget function invalid page argument");
    assert(widget !== undefined, "createWidget function widget must be defined");

    return page.gridster.add_widget(
        "<li class=\"widget\" id=\"gridsterWidget-" + page.id + "-" + widget.id +"\">" +
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
 * Callback for the request of the viewModel of a widget from the web server
 * @param page page concerned
 * @param widget widget concerned
 * @returns {Function}
 */
function getWidgetViewModelDone(page, widget)
{
   //viewModel.js has been executed
   //noinspection JSUnusedLocalSymbols
   return function(data, textStatus, jqxhr) {
      widget.viewModel = widgetViewModel;
      //we clear the widgetViewModel for the next viewModel
      widgetViewModel = null;
      var widgetDivId = "widget-" + widget.id;
      widget.$gridsterWidget = createWidget(page, widget);
      widget.$div = $("div#" + widgetDivId);

      //we apply binding
      ko.applyBindings(widget.viewModel, widget.$div[0]);

      $.getJSON( "widgets/" + widget.name + "/package.json")
         .done(getWidgetPackageDone(page, widget))
         .fail(function(widget) { return function() {notifyError("Unable to get the configuration of the widget " + widget.name)}}(widget));
   };
}

/**
 * Callback of the request Package information of a widget
 * @param page page concerned
 * @param widget widget concerned
 * @returns {Function}
 */
function getWidgetPackageDone(page, widget)
{
   return function( data ) {
      //we set the min and max size if they are defined
      assert(data !== undefined, "Configuration of widget must be defined");

      widget.package = data;

      if (data.dimensions.min !== undefined)
      {
         //min dimension is set
         assert((data.dimensions.min.x !== undefined) && (data.dimensions.min.y !== undefined), "You can't set only one axe of the widget minimum dimension");
         widget.$gridsterWidget.data('coords').grid.min_size_x = data.dimensions.min.x;
         widget.$gridsterWidget.data('coords').grid.min_size_y = data.dimensions.min.y;
      }

      if (data.dimensions.max !== undefined)
      {
         //min dimension is set
         assert((data.dimensions.max.x !== undefined) && (data.dimensions.max.y !== undefined), "You can't set only one axe of the widget maximum dimension");
         page.gridster.set_widget_max_size(widget.$gridsterWidget, [data.dimensions.max.x, data.dimensions.max.y])
      }

      //we initialize the widget
      try
      {
         if (widget.viewModel.initialize !== undefined)
            widget.viewModel.initialize(page, widget);

         if (widget.viewModel.resized !== undefined)
            widget.viewModel.resized();
      }
      catch (e)
      {
         notifyWarning("The widget " + widget.name + " has generated an exception");
         console.warn(e);
      }

      $("li.widget button").tooltip({
         animated: 'fade',
         placement: 'bottom'
      });

      //we check if we are in customization we must apply customization on the new item
      if (customization)
      {
         $(".customization-item").removeClass("hidden");
         $("li.widget").addClass("liWidgetCustomization");
      }
   };
}

/**
 * Return the current displayed page
 * @returns {Page}
 */
function getCurrentPage()
{
   return pageArray[$("ul.page-tabs li.active").attr("page-id")];
}