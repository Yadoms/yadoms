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
      var minDimension = Math.min(this.widget.width(), this.widget.height());
      if (minDimension <= 200)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "7em");
      else if (minDimension <= 300)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "14em");
      else if (minDimension <= 400)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "15em");
      else if (minDimension <= 500)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "23em");
      else
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "32em");
   };



};
