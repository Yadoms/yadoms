widgetViewModelCtor =

/**
 * Create a DevDeactivatedWidget ViewModel
 * @constructor
 */
function systemDeactivatedWidgetViewModel() {
   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function() {
      if (!isNullOrUndefined(this.widget.requiredType)) {
          this.widgetApi.find(".deactivatedWidget").text($.t("widgets.system-deactivated-widget:deactivatedWithType", { widgetType: this.widget.requiredType }));
      }
      else {
          this.widgetApi.find(".deactivatedWidget").text($.t("widgets.system-deactivated-widget:deactivated"));
      }
   };

   /**
    * Callback for a resized event
    */
   this.resized = function () {
   };
};
