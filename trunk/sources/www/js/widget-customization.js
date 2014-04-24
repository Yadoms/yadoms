/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

/**
 * Indicate if customization is in progress or not
 * @type {boolean}
 */
var customization = false;

/**
 * Indicate if we already have released the click button after entering in customization with long press
 * @type {boolean}
 */
var waitForRealeaseButtonAfterEnteringCustomization = false;

/**
 * Enable or disable customization on gridster elements
 * @param enable
 */
function enableGridsterCustomization(enable) {
   $.each(pageContainer.pages, function (index, value) {
      if (enable) {
         value.gridster.enable();
         value.gridster.enable_resize();
      }
      else {
         value.gridster.disable();
         value.gridster.disable_resize();
      }
   });
}

/**
 * Animate the customization wrench button
 */
function animateCustomizeButton() {
   $("a#customizeButton i").transition({
      rotate: '+=30deg',
      duration: 500,
      easing: 'linear',
      complete: function() {
         if (customization)
            animateCustomizeButton();
         else
            $("a#customizeButton i").transition({
               rotate: '0deg',
               duration: 100
            });
      }
   });
}

/**
 * Callback of the click on the customize button
 */
$("a#customizeButton").click(function() {
   customization = !customization;

   if (customization)
   {
      enterCustomization();
   }
   else
       exitCustomization();
});

function enterCustomization() {
   customization = true;
   enableGridsterCustomization(true);
   animateCustomizeButton();
   $(".customization-item").removeClass("hidden");
   $("li.widget").addClass("liWidgetCustomization");
}

/**
 * End customization and send all configuration to server
 */
function exitCustomization() {
   customization = false;
   waitForRealeaseButtonAfterEnteringCustomization = false;
   enableGridsterCustomization(false);
   $(".customization-item").addClass("hidden");
   $("li.widget").removeClass("liWidgetCustomization");

   //we save all widgets in each page
   $.each(pageContainer.pages, function (index, currentPage) {
      $.each(currentPage.widgets, function (index, currentWidget) {
         //we synchronize gridster information into the widget class
         currentWidget.updateDataFromGridster();

      });
      $.ajax({
         type: "PUT",
         url: "/rest/page/" + currentPage.id + "/widget",
         data: JSON.stringify(currentPage.widgets),
         contentType: "application/json; charset=utf-8",
         dataType: "json"
      })
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError("Error during saving customization", JSON.stringify(data));
               console.error(data.message);
               return;
            }
         })
         .fail(function() {notifyError("Unable to save customization")});
   });
}

function createOrUpdatePage(pageId) {
   modals.pageModification.load(function (pageId) {return function() {showPageModificationModal(pageId)}}(pageId));
}