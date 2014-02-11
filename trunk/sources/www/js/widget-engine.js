/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

function Page(id, name) {
   assert(id !== undefined, "id of a page must be defined");
   assert(name !== undefined, "name of a page must be defined");

   this.id = id;
   this.name = name;
   this.gridster;
   this.widgets = new Array();

   this.widgetsToJson = function() {
      data = new Array();
      for(widgetId in this.widgets) {
         data.push(this.widgets[widgetId]);
      }
      return JSON.stringify(data);
   }
}

function Widget(id, idPage, name, sizeX, sizeY, positionX, positionY, configuration) {
   assert(id !== undefined, "id of a widget must be defined");
   assert(idPage !== undefined, "idPage of a widget must be defined");
   assert(name !== undefined, "name of a widget must be defined");
   assert(sizeX !== undefined, "sizeX of a widget must be defined");
   assert(sizeY !== undefined, "sizeY of a widget must be defined");
   assert(positionX !== undefined, "positionX of a widget must be defined");
   assert(positionY !== undefined, "positionY of a widget must be defined");
   //configuration can be undefined

   this.id = id;
   this.idPage = idPage;
   this.name = name;
   this.sizeX = sizeX;
   this.sizeY = sizeY;
   this.positionX = positionX;
   this.positionY = positionY;
   this.configuration = configuration;

   this.viewModel;

   //gridster item
   this.$gridsterWidget;

   //div where is embed the widget
   this.$div;

   //package information of the current widget type (package.json file)
   this.package;

   /*
    {
    id: "1",
    idPage: "1",
    name: "temperature",
    sizeX: "2",
    sizeY: "2",
    positionX: "1",
    positionY: "1",
    configuration: ""
    },
    */
   this.toJSON = function () {
      return "{" +
                  "id:'" + this.id + "'," +
                  "idPage:'" + this.idPage + "'," +
                  "name:'" + this.name + "'," +
                  "sizeX:'" + this.sizeX + "'," +
                  "sizeY:'" + this.sizeY + "'," +
                  "positionX:'" + this.positionX + "'," +
                  "positionY:'" + this.positionY + "'," +
                  "configuration:'" + this.configuration + "'," +
               "}";
   };

   this.updateDataFromGridster = function() {
      this.sizeX = this.$gridsterWidget.data('coords').grid.size_x;
      this.sizeY = this.$gridsterWidget.data('coords').grid.size_y;
      this.positionX = this.$gridsterWidget.data('col');
      this.positionY = this.$gridsterWidget.data('row');
   }
}

function getWidgetFromGridsterElement($element)
{
   str = $element[0].id;
   res = str.split("-");
   assert(res.length == 3, "gridster id must be formed name-idPage-idWidget");
   return pageArray[res[1]].widgets[res[2]];
}

function requestPages()
{
   //we get pages
   $.getJSON("/rest/page")
      .done(requestPageDone())
      .fail(function() { notifyError("Unable to get Pages"); });
}

function requestPageDone()
{
    return function( data ) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during requesting pages")
         return;
      }

      $.each(data.data.page, function(index, value) {
         //foreach page
         currentPage = new Page(value.id, value.name);

         var tabIdAsText = "tab-" + value.id;
         //pill creation
         $("#tabContainer .tab-content").append(
             "<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\">" +
                 "<div class=\"gridster\">" +
                 "<ul></ul>" +
                 "</div>" +
                 "</div>");
         //page creation
         $("#pageMenu ul").append(
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
                     widgetObject = getWidgetFromGridsterElement($widget);

                     if (widgetObject.viewModel.resized !== undefined)
                        widgetObject.viewModel.resized();
                 },
                 stop: function(e, ui, $widget) {
                     widgetObject = getWidgetFromGridsterElement($widget);

                     if (widgetObject.viewModel.resized !== undefined)
                        widgetObject.viewModel.resized();
                 }
             }
         }).data('gridster');

         pageArray[currentPage.id] = currentPage;

         //we request widgets for this page
         $.getJSON("/rest/page/" + currentPage.id + "/widget")
           .done(requestWidgetsDone(currentPage))
           .fail(function(page) { return function() {notifyError("Unable to get Widgets for page " + page.name)}}(currentPage));
      });

      //we activate first page on the pills
      $("#pageMenu ul li").first().addClass("active");

      //we remove the active class of all hidden gridster
      $(".tab-content div").not($(".tab-content div").first()).removeClass("active");

      $(".widgetPage .gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

      //we deactivate the customization
      enableGridsterCustomization(false);
    };
}

function requestWidgetsDone(page)
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

function addWidgetToIHM(widget)
{
   pageArray[widget.idPage].widgets[widget.id] = widget;
   //for each widget we add view, viewModel and configuration of the widget
   $.get("widgets/" + widget.name + "/view.html")
      .done(getWidgetViewDone(pageArray[widget.idPage], widget))
      .fail(function(widget) { return function() {notifyError("Unable to get view of the widget " + widget.name) }}(widget));
}

function getWidgetViewDone(page, widget)
{
   return function( data ) {
      $("#templates").append(data);
      //we get script to execute for this widget
      //$.getScript( "widgets/" + widget.name + "/viewModel.js", getWidgetViewModelDone(page, widget));
      $.getScript("widgets/" + widget.name + "/viewModel.js")
         .done(getWidgetViewModelDone(page, widget))
         .fail(function(widget) { return function() {notifyError("Unable to get viewModel of the widget " + pageArray[pageId].widgets[widgetId].name)}}(widget));
   };
}

//Create a new widget and add it to the page
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

function getWidgetViewModelDone(page, widget)
{
   //viewModel.js has been executed
   return function(data, textStatus, jqxhr) {
      widget.viewModel = widgetViewModel;
      //we clear the widgetViewModel for the next viewModel
      widgetViewModel = null;
      widgetDivId = "widget-" + widget.id;
      gridsterWidgetId = "gridsterWidget-" + widget.id;
      widget.$gridsterWidget = createWidget(page, widget);
      widget.$div = $("#" + widgetDivId);

      //we apply binding
      ko.applyBindings(widget.viewModel, widget.$div[0]);

      $.getJSON( "widgets/" + widget.name + "/package.json")
         .done(getWidgetConfigurationDone(page, widget))
         .fail(function(widget) { return function() {notifyError("Unable to get the configuration of the widget " + widget.name)}}(widget));
   };
}

function getWidgetConfigurationDone(page, widget)
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
      if (widget.viewModel.initialize !== undefined)
         widget.viewModel.initialize(page, widget);

      if (widget.viewModel.resized !== undefined)
         widget.viewModel.resized();

      $(".widget button").tooltip({
         animated: 'fade',
         placement: 'bottom'
      });

      //we check if we are in customization we must apply customization on the new item
      if (customization)
      {
         $(".customization-item").removeClass("hidden");
         $(".widget").addClass("liWidgetCustomization");
      }
   };
}

//Return the Page object which is currently displayed
function getCurrentPage()
{
   return pageArray[$(".page-tabs .active").attr("page-id")];
}