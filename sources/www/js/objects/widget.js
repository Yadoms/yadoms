/** @module Widget class */

/**
 * Create an instance of Widget
 * @param id database id of the widget
 * @param idPage database page id of the widget
 * @param type type of the widget
 * @param title title of the widget
 * @param sizeX x size of the widget
 * @param sizeY y size of the widget
 * @param positionX x position of the widget
 * @param positionY y position of the widget
 * @param configuration configuration of the widget
 * @constructor
 */
function Widget(id, idPage, type, title, sizeX, sizeY, positionX, positionY, configuration) {
    assert(id !== undefined, "id of a widget must be defined");
    assert(idPage !== undefined, "idPage of a widget must be defined");
    assert(type !== undefined, "type of a widget must be defined");
    assert(title !== undefined, "type of a widget must be defined");
    assert(sizeX !== undefined, "sizeX of a widget must be defined");
    assert(sizeY !== undefined, "sizeY of a widget must be defined");
    assert(positionX !== undefined, "positionX of a widget must be defined");
    assert(positionY !== undefined, "positionY of a widget must be defined");
    //configuration can be undefined

    this.id = id;
    this.idPage = idPage;
    this.type = type;
    this.title = title;
    this.sizeX = sizeX;
    this.sizeY = sizeY;
    this.positionX = positionX;
    this.positionY = positionY;
    this.configuration = configuration;

    this.viewModel = null;

    //grid item
    this.$gridWidget = null;

    //div where is embed the widget
    this.$div = null;

    //package information of the current widget type (package.json file)
    this.package = null;

    //toolbar of the widget
    this.$toolbar = null;

    //list of all devices {deviceId, keywordId} to listen
    this.listenedKeywords = [];
}

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, idPage: *, type: *,title: *, sizeX: *, sizeY: *, positionX: *, positionY: *, configuration: *}}
 */
Widget.prototype.toJSON = function () {
    return {
        id: this.id,
        idPage: this.idPage,
        type: this.type,
        title: this.title,
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
Widget.prototype.updateDataFromGrid = function () {
    this.sizeX = parseInt(this.$gridWidget.attr("data-gs-width"));
    this.sizeY = parseInt(this.$gridWidget.attr("data-gs-height"));
    this.positionX = parseInt(this.$gridWidget.attr("data-gs-x"));
    this.positionY = parseInt(this.$gridWidget.attr("data-gs-y"));
};

Widget.prototype.height = function () {
    return parseInt(this.$gridWidget.css("height").replace('px', ''));
};

Widget.prototype.width = function () {
    return parseInt(this.$gridWidget.css("width").replace('px', ''));
};

Widget.prototype.setHeight = function (newHeight) {
    return this.$gridWidget.css("height", newHeight + 'px');
};

Widget.prototype.setWidth = function (newWidth) {
    return this.$gridWidget.css("width", newWidth + 'px');
};

Widget.prototype.title = function () {
    return this.title;
};