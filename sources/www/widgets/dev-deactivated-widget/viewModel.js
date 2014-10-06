widgetViewModelCtor =

/**
 * Create a DevDeactivatedWidget ViewModel
 * @constructor
 */
function DevDeactivatedWidgetViewModel() {
   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;

      if (!isNullOrUndefined(this.widget.requiredType)) {
         this.widget.$gridsterWidget.find("div.deactivatedWidget span").text($.t("dev-deactivated-widget:deactivatedWithType", {widgetType : this.widget.requiredType}));
      }
      else {
         this.widget.$gridsterWidget.find("div.deactivatedWidget span").text($.t("dev-deactivated-widget:deactivated"));
      }
   };

   /**
    * Callback for a resized event
    */
   this.resized = function() {
      if (this.widget.$gridsterWidget.width() <= 200) {
         this.widget.$gridsterWidget.find("div.deactivatedWidget span").css("font-size", "1em");
      }
      else
         this.widget.$gridsterWidget.find("div.deactivatedWidget span").css("font-size", "2em");
   };
};
