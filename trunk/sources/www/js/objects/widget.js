/** @module Widget class */

/**
 * Create an instance of Widget
 * @param id database id of the widget
 * @param idPage database page id of the widget
 * @param type type of the widget
 * @param sizeX x size of the widget
 * @param sizeY y size of the widget
 * @param positionX x position of the widget
 * @param positionY y position of the widget
 * @param configuration configuration of the widget
 * @constructor
 */
function Widget(id, idPage, type, sizeX, sizeY, positionX, positionY, configuration) {
   assert(id !== undefined, "id of a widget must be defined");
   assert(idPage !== undefined, "idPage of a widget must be defined");
   assert(type !== undefined, "type of a widget must be defined");
   assert(sizeX !== undefined, "sizeX of a widget must be defined");
   assert(sizeY !== undefined, "sizeY of a widget must be defined");
   assert(positionX !== undefined, "positionX of a widget must be defined");
   assert(positionY !== undefined, "positionY of a widget must be defined");
   //configuration can be undefined

   this.id = id;
   this.idPage = idPage;
   this.type = type;
   this.sizeX = sizeX;
   this.sizeY = sizeY;
   this.positionX = positionX;
   this.positionY = positionY;
   this.configuration = configuration;

   this.viewModel = null;

   //gridster item
   this.$gridsterWidget = null;

   //div where is embed the widget
   this.$div = null;

   //package information of the current widget type (package.json file)
   this.package = null;
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, idPage: *, type: *, sizeX: *, sizeY: *, positionX: *, positionY: *, configuration: *}}
 */
Widget.prototype.toJSON = function () {
   return { 
			id : this.id, 
			idPage: this.idPage, 
			type: this.type,
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
   this.sizeX = this.$gridsterWidget.coords().grid.size_x;
   this.sizeY = this.$gridsterWidget.coords().grid.size_y;
   this.positionX = this.$gridsterWidget.coords().grid.col;
   this.positionY = this.$gridsterWidget.coords().grid.row;
};

Widget.prototype.height = function () {
   this.updateDataFromGridster();
   return this.sizeY * gridWidth + (this.sizeY - 1) * (gridMargin * 2);
};

Widget.prototype.width = function () {
   this.updateDataFromGridster();
   return this.sizeX * gridWidth + (this.sizeX - 1) * (gridMargin * 2);
};
