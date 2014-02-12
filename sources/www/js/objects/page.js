/** @module Page class */

/**
 * Creates an instance of Page
 * @param id database id of the page
 * @param name name of the page
 * @constructor
 */
function Page(id, name) {
   assert(id !== undefined, "id of a page must be defined");
   assert(name !== undefined, "name of a page must be defined");

   this.id = id;
   this.name = name;
   this.gridster;
   this.widgets = new Array();
}

/**
 * Create a list of widget in json string format
 * @this {Circle}
 * @return {string} json string
 */
Page.prototype.widgetsToJsonString = function() {
   data = new Array();
   for(widgetId in this.widgets) {
      data.push(this.widgets[widgetId]);
   }
   return JSON.stringify(data);
}