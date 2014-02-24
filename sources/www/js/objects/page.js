/** @module Page class */

/**
 * Creates an instance of Page
 * @param id database id of the page
 * @param name name of the page
 * @constructor
 */
function Page(id, name, pageOrder) {
   assert(id !== undefined, "id of a page must be defined");
   assert(name !== undefined, "name of a page must be defined");
   assert(pageOrder !== undefined, "order of a page must be defined");

   this.id = id;
   this.name = name;
   this.pageOrder = pageOrder;
   this.gridster;
   this.widgets = new Array();
   this.$tab = null;
   this.$content = null;
}

/**
 * Create a list of widget in json string format
 * @this {Circle}
 * @return {string} json string
 */
Page.prototype.widgetsToJsonString = function() {
   var data = new Array();
   for(widgetId in this.widgets) {
      data.push(this.widgets[widgetId]);
   }
   return JSON.stringify(data);
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {string}
 */
Page.prototype.toJSON = function () {
   return {
      id : this.id,
      name: encodeURIComponent(this.name),
      pageOrder: this.pageOrder
   };
};