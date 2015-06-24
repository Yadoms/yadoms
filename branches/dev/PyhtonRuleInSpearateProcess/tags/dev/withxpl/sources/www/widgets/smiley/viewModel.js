widgetViewModelCtor =

/**
 * Create a Smiley ViewModel
 * @constructor
 */
function SmileyViewModel() {
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
   };

   /**
    * Callback for a resized event
    */
   this.resized = function() {
      if (this.widget.$gridsterWidget.width() <= 200) {
         this.widget.$gridsterWidget.find("div.smileyWidget span").css("font-size", "7em");
      }
      else if (this.widget.$gridsterWidget.width() <= 300)
         this.widget.$gridsterWidget.find("div.smileyWidget span").css("font-size", "15em");
      else if (this.widget.$gridsterWidget.width() <= 400)
         this.widget.$gridsterWidget.find("div.smileyWidget span").css("font-size", "23em");
      else
         this.widget.$gridsterWidget.find("div.smileyWidget span").css("font-size", "32em");
   };



};
