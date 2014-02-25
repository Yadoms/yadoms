/**
 * Created by nicolasHILAIRE on 06/02/14.
 */

/**
 * Indicate if customization is in progress or not
 * @type {boolean}
 */
var customization = false;

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
      $("a#customizeButton").removeClass('btn-inverse').addClass('btn-primary');
      enableGridsterCustomization(true);
      animateCustomizeButton();
      $(".customization-item").removeClass("hidden");
      $("li.widget").addClass("liWidgetCustomization");
   }
   else
       exitCustomization();
});

/**
 * Callback of the click on the background to stop customization
 */
$("div#tabContainer").click(function() {
   if (customization)
      exitCustomization();
});

/**
 * End customization and send all configuration to server
 */
function exitCustomization() {
   customization = false;
   $("a#customizeButton").removeClass('btn-primary').addClass('btn-inverse');
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
               notifyError("Error during saving customization");
               console.error(data.message);
               return;
            }
         })
         .fail(function() {notifyError("Unable to save customization")});
   });
}

function createOrUpdatePage(pageId) {
   if (modificationPageModalHasBeenLoaded)
   {
      //we show the modal to modify page name in add mode
      showPageModificationModal(pageId);
   }
   else
   {
      $.ajax( "modals/modify_page.html" )
         .done(function(data) {
            //we append it to the index
            $('body').append(data);
            //we show the modal to modify page
            showPageModificationModal(pageId);
         })
         .fail(function() {
            notifyError("Unable to load page modification form");
         });
   }
}

function deletePage(pageId) {
   if (deletePageModalHasBeenLoaded)
   {
      //we show the modal to delete a page
      showDeletePageModal(pageId);
   }
   else
   {
      $.ajax( "modals/delete_page.html" )
         .done(deletePageDone(pageId))
         .fail(function() {
            notifyError("Unable to load page deletion form");
         });
   }
}

function deletePageDone(pageId) {
   return function(data) {
      //we append it to the index
      $('body').append(data);
      //we show the modal to delete a page
      showDeletePageModal(pageId);
   };
}

function deleteWidget(pageId, widgetId) {
   if (deleteWidgetModalHasBeenLoaded)
   {
      //we show the modal to delete a widget
      showDeleteWidgetModal(pageId, widgetId);
   }
   else
   {
      $.ajax( "modals/delete_widget.html" )
         .done(deleteWidgetDone(pageId, widgetId))
         .fail(function() {
            notifyError("Unable to load widget deletion form");
         });
   }
}

function deleteWidgetDone(pageId, widgetId) {
   return function(data) {
      //we append it to the index
      $('body').append(data);
      //we show the modal to delete a widget
      showDeleteWidgetModal(pageId, widgetId);
   };
}