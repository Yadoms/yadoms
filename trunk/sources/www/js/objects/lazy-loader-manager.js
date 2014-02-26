/**
 * Created by Nicolas on 26/02/14.
 */

//example :
// loader = new LazyLoaderManager("modals/configure_widget.html");
// modals.widgetConfiguration.load(function (pageId, widgetId) {return function() {showWidgetConfigurationModal(pageId, widgetId)}}(pageId, widgetId));

/**
 * Creates an instance of LazyLoaderManager
 * @constructor
 * @param modalPath
 */
function LazyLoaderManager(modalPath) {
   assert(modalPath !== undefined, "modalPath must be defined");

   this.modalHasBeenLoaded = false;
   this.modalPath = modalPath;
}

LazyLoaderManager.prototype.load = function (callback){
   assert(callback !== undefined, "callback must be defined");
   if (this.modalHasBeenLoaded) {
      //we simply call the callback function
      callback();
   }
   else
   {
      $.ajax(this.modalPath)
         .done(function(data) {
            //we add the modal to the body
            $('body').append(data);
            //we save the information that the modal has been loaded
            this.modalHasBeenLoaded = true;
            //we call the callback
            callback();
         })
         .fail(function() {
            notifyError("Unable to load modal " + this.modalPath);
         });
   }
}