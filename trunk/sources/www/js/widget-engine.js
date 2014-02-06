/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

function Page() {
   this.pageData;
   this.gridster;
}

function getDatabaseIdFromElement($element)
{
   str = $element[0].id;
   res = str.split("-");
   return res[res.length - 1];
}

function requestPageDone()
{
   return function( data ) {
      //we parse the json answer
      $.each(data.page, function(index, value) {
         //foreach page
         pageArray[value.id] = new Page();
         pageArray[value.id].pageData = value;
         var tabIdAsText = "tab-" + value.id;
         //pill creation
         $("#tabContainer .tab-content").append("<div class=\"widgetPage tab-pane active\" id=\"" + tabIdAsText + "\"><div class=\"gridster\"><ul></ul></div></div>")
         //page creation
         $("#pageMenu ul").append("<li><a href=\"#" + tabIdAsText + "\" data-toggle=\"tab\">" + value.name + "</a></li>");
         //gridster creation
         pageArray[value.id].gridster = $("#" + tabIdAsText + " ul").gridster({
            widget_margins: [gridMargin, gridMargin],
            widget_base_dimensions: [gridWidth, gridWidth],
            min_cols: numberOfColumns,
            max_cols: numberOfColumns,
            resize: {
               enabled: true,
               resize: function(e, ui, $widget) {

                     if ($widget.width() <= $widget.data('coords').grid.min_size_x * gridWidth) {
                        $widget.width($widget.data('coords').grid.min_size_x * gridWidth);
                     }
                     if ($widget.height() <= $widget.data('coords').grid.min_size_y * gridWidth) {
                        $widget.height($widget.data('coords').grid.min_size_y * gridWidth);
                     }

                  id = getDatabaseIdFromElement($widget);
                  if (widgetArray[id].viewModel.resized !== undefined)
                     widgetArray[id].viewModel.resized()
               },
               stop: function(e, ui, $widget) {
                  id = getDatabaseIdFromElement($widget);
                  if (widgetArray[id].viewModel.resized !== undefined)
                     widgetArray[id].viewModel.resized()
               }
            }
         }).data('gridster');
      });

      //we activate first page on the pills
      $("#pageMenu ul li").first().addClass("active");

      //we remove the active class of all hidden gridster
      $(".tab-content div").not($(".tab-content div").first()).removeClass("active");

      $(".widgetPage .gridster").width(numberOfColumns * (gridWidth + gridMargin * 2));

      //we deactivate the customization
      enableGridsterCustomization(false);

      //we get widgets from REST
      $.getJSON( "/rest/widget", requestWidgetsDone());
   };
}

function requestWidgetsDone()
{
   return function(data) {
      //we parse the json answer
      $.each(data.widget, function(index, value) {
         widgetArray[value.id] = value;
         //for each widget we add view, viewModel and configuration of the widget
         $.get( "widgets/" + value.name + "/view.html", getWidgetViewDone(value));
      });
   };
}

function getWidgetViewDone(widget)
{
   return function( data ) {
      $("#templates").append(data);
      //we get script to execute for this widget
      $.getScript( "widgets/" + widget.name + "/viewModel.js", getWidgetViewModelDone(widget));
   };
}

function getWidgetViewModelDone(widget)
{
   //viewModel.js has been executed
   return function(data, textStatus, jqxhr) {
      widget.viewModel = widgetViewModel;
      widgetViewModel = null;
      widgetDivId = "widget-" + widget.id;
      gridsterWidgetId = "gridsterWidget-" + widget.id;
      $gridsterWidget = pageArray[widget.idPage].gridster.add_widget(
         "<li class=\"widget\" id=\"gridsterWidget-" + widget.id +"\">" +
            "<div class=\"widgetCustomizationToolbar hidden\">" +
            "<div class=\"btn-group btn-group-sm\">" +
            "<button type=\"button\" class=\"btn btn-default\" title=\"Configure\"><i class=\"fa fa-cog\"></i></button>" +
            "<button type=\"button\" class=\"btn btn-default\" title=\"Delete\"><i class=\"fa fa-times\"></i></button>" +
            "</div>" +
            "</div>" +
            "<div id=\"" + widgetDivId + "\" data-bind=\"template: { name: '" + widget.name + "-template', data: data }\"/>" +
            "</li>", widget.sizeX, widget.sizeY, widget.positionX, widget.positionY);

      $createdObject = $("#" + widgetDivId);

      //we apply binding
      ko.applyBindings(widget.viewModel, $createdObject[0]);

      $.getJSON( "widgets/" + widget.name + "/configuration.json", getWidgetConfigurationDone(widget, $createdObject, $gridsterWidget));
   };
}

function getWidgetConfigurationDone(widget, $widgetNode, $gridsterWidgetNode)
{
   return function( data ) {
      //we set the min and max size if they are defined
      assert(data !== undefined, "Configuration of widget must be defined");
      if (data.dimensions.min !== undefined)
      {
         //min dimension is set
         assert((data.dimensions.min.x !== undefined) && (data.dimensions.min.y !== undefined), "You can't set only one axe of the widget minimum dimension");
         $gridsterWidgetNode.data('coords').grid.min_size_x = data.dimensions.min.x;
         $gridsterWidgetNode.data('coords').grid.min_size_y = data.dimensions.min.y;
         //pageArray[widget.idPage].gridster.set_widget_min_size($gridsterWidgetNode, [data.dimensions.max.x, data.dimensions.max.y])
      }

      if (data.dimensions.max !== undefined)
      {
         //min dimension is set
         assert((data.dimensions.max.x !== undefined) && (data.dimensions.max.y !== undefined), "You can't set only one axe of the widget maximum dimension");
         pageArray[widget.idPage].gridster.set_widget_max_size($gridsterWidgetNode, [data.dimensions.max.x, data.dimensions.max.y])
      }

      //we initialize the widget
      if (widget.viewModel.initialize !== undefined)
         widget.viewModel.initialize($widgetNode, $gridsterWidgetNode, data);

      if (widget.viewModel.resized !== undefined)
         widget.viewModel.resized();

      $(".widget button").tooltip({
         animated: 'fade',
         placement: 'bottom'
      });
   };
}

