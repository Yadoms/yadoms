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
   for(var pageId in pageArray) {
      if (pageArray.hasOwnProperty(pageId)) {
         if (enable)
         {
            pageArray[pageId].gridster.enable();
            pageArray[pageId].gridster.enable_resize();
         }
         else
         {
            pageArray[pageId].gridster.disable();
            pageArray[pageId].gridster.disable_resize();
         }
      }
   }
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
   for(var pageId in pageArray) {
      if (pageArray.hasOwnProperty(pageId)) {
         var page = pageArray[pageId];
         for(var widgetId in page.widgets) {
            if (page.widgets.hasOwnProperty(widgetId)) {
               //we synchronize gridster information into the widget class
               pageArray[pageId].widgets[widgetId].updateDataFromGridster();
            }
         }

         $.ajax({
             type: "PUT",
             url: "/rest/page/" + pageId + "/widget",
             data: page.widgetsToJsonString(),
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
             //notifySuccess("Customization successfully saved");
          })
          .fail(function() {notifyError("Unable to save customization")});
      }
   }
}

/**
 * Callback of the click on the add widget button
 * Make lazy loading of the add widget modal
 */
$("#btn-add-widget").click(function() {
    if (addWidgetHasBeenLoaded)
    {
        //we ask the package to display the modal
        askWidgetPackages();
    }
    else
    {
        $.ajax( "modals/add_widget.html" )
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
         .done(function(data) {
            //we append it to the index
            $('body').append(data);
            //we show the modal to delete a page
            showDeletePageModal(pageId);
         })
         .fail(function() {
            notifyError("Unable to load page deletion form");
         });
   }
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
         .done(function(data) {
            //we append it to the index
            $('body').append(data);
            //we show the modal to delete a widget
            showDeleteWidgetModal(widgetId);
         })
         .fail(function() {
            notifyError("Unable to load widget deletion form");
         });
   }
}