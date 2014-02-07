/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

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
      $(".widgetCustomizationToolbar").removeClass("hidden");
      $(".widget").addClass("liWidgetCustomization");
   }
   else
   {
      $("#customizeButton").removeClass('btn-primary').addClass('btn-inverse');
      enableGridsterCustomization(false);
      $(".widgetCustomizationToolbar").addClass("hidden");
       $(".widget").removeClass("liWidgetCustomization");
   }
});