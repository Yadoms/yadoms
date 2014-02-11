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

dateReplacer = function(key, value){
   if (this[key] instanceof Date){
      var date = this[key];
      return date.getDay() + "/" + date.getMonth() + "/" + date.getYear();
   }else{
      return value;
   }
}

//This function end customization and send all configuration to server
function exitCustomization() {
   customization = false;
   $("#customizeButton").removeClass('btn-primary').addClass('btn-inverse');
   enableGridsterCustomization(false);
   $(".customization-item").addClass("hidden");
   $(".widget").removeClass("liWidgetCustomization");

   //we save all widgets in each page
   for(pageId in pageArray) {
      for(widgetId in pageArray[pageId].widgets) {
         var widget =  pageArray[pageId].widgets[widgetId];
         //we synchronize gridster information into the widget class
         widget.updateDataFromGridster();
      }
      data = pageArray[pageId].widgetsToJson();

      $.ajax({
          type: "PUT",
          url: "/rest/page/" + pageId + "/widget",
          data: pageArray[pageId].widgetsToJson(),
          contentType: "application/json; charset=utf-8",
          dataType: "json"
       })
       .done(function(data) {
          //we parse the json answer
          if (data.result != "true")
          {
            notifyError("Error during saving customization");
            console.log(data.message);
            return;
          }
          notifySuccess("Customization successfully saved");
       })
       .fail(function() {notifyError("Unable to save customization")});
   }
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




