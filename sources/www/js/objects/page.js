/** @module Page class */

/**
 * Creates an instance of Page
 * @param id database id of the page
 * @param name name of the page
 * @param pageOrder
 * @constructor
 */
function Page(id, name, pageOrder) {
   assert(id !== undefined, "id of a page must be defined");
   assert(name !== undefined, "name of a page must be defined");
   assert(pageOrder !== undefined, "order of a page must be defined");

   this.id = id;
   this.name = name;
   this.pageOrder = pageOrder;
   this.widgets = [];
   this.$tab = null;
   this.$content = null;
   this.loaded = false;
}

/**
 * Get Widget with its id in the page. return null if it's not exist
 * @param widgetId
 * @returns {Widget}
 */
Page.prototype.getWidget = function(widgetId) {
   var res = $.grep(this.widgets, function(item) {
      return (item.id === parseInt(widgetId));
   });
   if (res.length !== 1)
      return null;
   return res[0];
};

Page.prototype.addWidget = function(widget) {
   //we look if it isn't already in page
   var res = this.getWidget(widget.id);
   assert(res == null, "Widget has already been added to this page");
   this.widgets.push(widget);
};

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, name: string, pageOrder: *}}
 */
Page.prototype.toJSON = function () {
   return {
      id : this.id,
      name: this.name,
      pageOrder: this.pageOrder
   };
};