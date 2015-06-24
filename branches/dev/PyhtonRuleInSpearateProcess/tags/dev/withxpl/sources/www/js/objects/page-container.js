/**
 * Created by nicolasHILAIRE on 25/02/14.
 */

/**
 * Creates an instance of PageContainer
 * @constructor
 */
function PageContainer() {

   /**
    * Array of pages
    * @type {Array}
    */
   this.pages = [];
}

/**
 * Get Page with its id in the page. return null if it's not exist
 * @param pageId
 * @returns {Page}
 */
PageContainer.prototype.getPage = function(pageId) {
   var res = $.grep(this.pages, function(item) {
      return (item.id == pageId);
   });
   if (res.length != 1)
      return null;
   return res[0];
};

PageContainer.prototype.addPage = function(page) {
   //we look if it isn't already in pageContainer
   var res = this.getPage(page.id);
   assert(res == null, "Page has already been added to the container");
   this.pages.push(page);
};