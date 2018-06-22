/**
 * Created by Nicolas on 26/02/14.
 */

//example :
// loader = new LazyLoaderManager("modals/widget-configure.html");
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

/**
 * Load the modal with lazy loading if needed
 * @param callback
 */
LazyLoaderManager.prototype.load = function (callback){
   assert($.isFunction(callback), "callback must be defined");
   var self = this;
   if (self.modalHasBeenLoaded) {
      //we simply call the callback function
      callback();
   }
   else
   {
      RestEngine.getHtml(self.modalPath)
         .done(function(data) {
            var $data = $(data);
            $data.i18n();
            //we add the modal to the body
            $('body').append($data);
            //we save the information that the modal has been loaded
            self.modalHasBeenLoaded = true;
            //we call the callback
            callback();
         })
         .fail(function(error) {
            notifyError($.t("objects.lazyLoaderManager.unableToLoadModal", {modalPath : self.modalPath}), error);
         });
   }
};

/**
 * Load the modal with lazy loading if needed
 * @param callback
 */
LazyLoaderManager.prototype.loadAsync = function () {
   var d = new $.Deferred();

   var self = this;
   if (self.modalHasBeenLoaded) {
      d.resolve();
   }
   else {
      RestEngine.getHtml(self.modalPath)
      .done(function(data) {
         var $data = $(data);
         $data.i18n();
         //we add the modal to the body
         $('body').append($data);
         //we save the information that the modal has been loaded
         self.modalHasBeenLoaded = true;
         d.resolve();
      })
      .fail(function(error) {
         notifyError($.t("objects.lazyLoaderManager.unableToLoadModal", { modalPath: self.modalPath }), error);
         d.reject(error);
      });
   }
   return d.promise();
};