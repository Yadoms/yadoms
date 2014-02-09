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

function exitCustomization() {
    $("#customizeButton").removeClass('btn-primary').addClass('btn-inverse');
    enableGridsterCustomization(false);
    $(".customization-item").addClass("hidden");
    $(".widget").removeClass("liWidgetCustomization");
}

$("#btn-add-widget").click(function() {
    //we list all widgets in the modal

    //TODO interrogation en REST pour recuperer la liste des widgets disponibles
    //pour l'instant on le prend dansun fichier json
    $.getJSON( "/widgets/widgets.json", requestWidgetsTypeDone());

    //TODO afficher une popup d'infomation

});

function requestWidgetsTypeDone()
{
    return function( data ) {
        //we parse the json answer
        $("#new-widget-carousel .carousel-inner > div").remove();
        $.each(data.widgetType, function(index, value) {
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

$( document ).ready(function() {
    $("#btn-confirm-add-widget").click(function () {

        var creatingWidgetNoty = noty({text: 'Creating widget ...', timeout:2000,  layout:'topLeft'});
        //ask for widget creation to the server
        $.ajax({
            url: '/rest/widget',
            type: 'PUT'
            })
            .done(function() {
                //TODO : check the answer
                creatingWidgetNoty.close();
                noty({text: 'Widget sucesfully created', timeout:2000,  layout:'topLeft', type: 'success'});
            })
            .fail(function() {
                noty({text: 'Unable to create widget', timeout:2000,  layout:'topLeft', type: 'alert'});
            })
            .always(function() {
            });
    });
});