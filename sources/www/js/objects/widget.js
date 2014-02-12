/** @module Widget class */

/**
 * Create an instance of Widget
 * @param id database id of the widget
 * @param idPage database page id of the widget
 * @param name name of the widget
 * @param sizeX x size of the widget
 * @param sizeY y size of the widget
 * @param positionX x position of the widget
 * @param positionY y position of the widget
 * @param configuration configuration of the widget
 * @constructor
 */
function Widget(id, idPage, name, sizeX, sizeY, positionX, positionY, configuration) {
   assert(id !== undefined, "id of a widget must be defined");
   assert(idPage !== undefined, "idPage of a widget must be defined");
   assert(name !== undefined, "name of a widget must be defined");
   assert(sizeX !== undefined, "sizeX of a widget must be defined");
   assert(sizeY !== undefined, "sizeY of a widget must be defined");
   assert(positionX !== undefined, "positionX of a widget must be defined");
   assert(positionY !== undefined, "positionY of a widget must be defined");
   //configuration can be undefined

   this.id = id;
   this.idPage = idPage;
   this.name = name;
   this.sizeX = sizeX;
   this.sizeY = sizeY;
   this.positionX = positionX;
   this.positionY = positionY;
   this.configuration = configuration;

   this.viewModel;

   //gridster item
   this.$gridsterWidget;

   //div where is embed the widget
   this.$div;

   //package information of the current widget type (package.json file)
   this.package;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {string}
 */
Widget.prototype.toJSON = function () {
   return { 
			id : this.id, 
			idPage: this.idPage, 
			name: this.name,
			sizeX: this.sizeX,
			sizeY: this.sizeY,
			positionX: this.positionX,
			positionY: this.positionY,
			configuration: this.configuration
		  };
};

/**
 * Synchronize data from grister object and properties of the class
 */
Widget.prototype.updateDataFromGridster = function() {
   this.sizeX = this.$gridsterWidget.data('coords').grid.size_x;
   this.sizeY = this.$gridsterWidget.data('coords').grid.size_y;
   this.positionX = this.$gridsterWidget.data('col');
   this.positionY = this.$gridsterWidget.data('row');
}