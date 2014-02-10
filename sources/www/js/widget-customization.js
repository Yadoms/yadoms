/**
 * Created by nicolasHILAIRE on 06/02/14.
 */
//var declaration
var customization = false;

function enableGridsterCustomization(enable) {
   $.each(pageArray, function (index, value) {
      if (enable)
      {
         value.gridster.enable();
         value.gridster.enable_resize();
      }
      else
      {
         value.gridster.disable();
         value.gridster.disable_resize();
      }
   });
}

function animateCustomizeButton() {
   $("#customizeButton i").transition({
      rotate: '+=30deg',
      duration: 500,
      easing: 'linear',
      complete: function() {
         if (customization)
            animateCustomizeButton();
         else
            $("#customizeButton i").transition({
               rotate: '0deg',
               duration: 100
            });
      }
   });
}

$("#customizeButton").click(function() {
   customization = !customization;

   if (customization)
   {
      $("#customizeButton").removeClass('btn-inverse').addClass('btn-primary');
      enableGridsterCustomization(true);
      animateCustomizeButton();
      $(".customization-item").removeClass("hidden");
      $(".widget").addClass("liWidgetCustomization");
   }
   else
       exitCustomization();
});

$("#btn-exit-customization").click(function() {
    exitCustomization();
});

$("#tabContainer").click(function() {
   //TODO : ne fonctionne pas pour l'instant il faut pouvoir quitter la customization en cliquant sur le fond et non un item
   //exitCustomization();
});

function exitCustomization() {
    $("#customizeButton").removeClass('btn-primary').addClass('btn-inverse');
    enableGridsterCustomization(false);
    $(".customization-item").addClass("hidden");
    $(".widget").removeClass("liWidgetCustomization");
}

$("#btn-add-widget").click(function() {
    if (addWidgetHasBeenLoaded)
    {
        //we ask the package to display the modal
        askWidgetPackages();
    }
    else
    {
        $.ajax( "add_widget.html" )
            .done(function(data) {
                //we append it to the index
                $('body').append(data);
                //we ask the package to display the modal
                askWidgetPackages();
            })
            .fail(function() {
                notifyError("Unable to add a widget");
            });
    }
});

function askWidgetPackages()
{
    $.getJSON("rest/widget/packages")
        .done(requestWidgetsTypeDone())
        .fail(function() {notifyError("Unable to get widget packages")});
}

function requestWidgetsTypeDone()
{
    return function( data ) {
        //we parse the json answer
        if (data.result != "true")
        {
            notifyError("Error during requesting widget packages");
            return;
        }
        $("#new-widget-carousel .carousel-inner > div").remove();
        $.each(data.data.packages, function(index, value) {
            //foreach widget-type
            //carousel item creation
            $("#new-widget-carousel .carousel-inner").append(
                "<div class=\"item\">" +
                    "<img class=\"carousel-widget-preview\" src=\"/widgets/" + value.name + "/preview.png\">" +
                    "<div class=\"carousel-caption\">" + value.description + "</div>" +
                "</div>"
            );
        });

        //we activate first item of carousel
        $("#new-widget-carousel .item").first().addClass("active");

        $('.carousel').carousel('pause');

        //we display the modal
        $('#new-widget-modal').modal();
    };
}


