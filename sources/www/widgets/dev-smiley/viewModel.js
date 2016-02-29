/**
 * Create a Smiley ViewModel
 * @constructor
 */
 
widgetViewModelCtor = function() {
   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function () {
      var self = this;
      self.widgetApi.loadCss([
         "widgets/dev-smiley/widget.css"
      ]).done(function() {
         self.widgetApi.find(".widget-api-textfit").fitText();
      });
   };

   this.configurationChanged = function() {
      
   }

   
};
