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
   $.each(PageManager.pages, function (index, page) {
      if (enable) {
         page.gridster.enable();
         page.gridster.enable_resize();

         //after foreach widget we hide handle on widget that are not resizable
         $.each(page.widgets, function (wIndex, widget) {
            try {
               if ((widget.package.dimensions.min.x == widget.package.dimensions.max.x) &&
                  (widget.package.dimensions.min.y == widget.package.dimensions.max.y)) {
                  //the widget is not resizable
                  widget.$gridsterWidget.find("span.gs-resize-handle").addClass("hidden");
               }
            }
            catch (err) {
            }

         });
      }
      else {
         page.gridster.disable();
         page.gridster.disable_resize();
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
   $.each(PageManager.pages, function (index, currentPage) {
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
               notifyError($.t("mainPage.errors.errorSavingCustomization"), JSON.stringify(data));
               console.error(data.message);
            }
         })
         .fail(function() {notifyError($.t("mainPage.errors.errorSavingCustomization"))});
   });
}

function createOrUpdatePage(pageId) {
   modals.pageConfigure.load(function (pageId) {return function() {showPageModificationModal(pageId)}}(pageId));
}