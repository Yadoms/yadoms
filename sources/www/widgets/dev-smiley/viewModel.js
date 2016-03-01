/**
 * Create a Smiley ViewModel
 * @constructor
 */
 
widgetViewModelCtor = function() {
   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function() {
   };

   /**
    * Callback for a resized event
    */
   this.resized = function() {
      var minDimension = Math.min(this.widget.width(), this.widget.height());
      if (minDimension <= 200)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "6em");
      else if (minDimension <= 300)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "12em");
      else if (minDimension <= 400)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "20em");
      else if (minDimension <= 500)
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "26em");
      else
         this.widget.$gridWidget.find("div.smileyWidget span").css("font-size", "31em");
   };
};
