/** @module Widget class */

/**
 * Create an instance of Widget
 * @param id database id of the widget
 * @param idPage database page id of the widget
 * @param type type of the widget
 * @param title title of the widget
 * @param sizeX x size of the widget
 * @param sizeY y size of the widget
 * @param position position of the widget
 * @param configuration configuration of the widget
 * @constructor
 */
function Widget(id, idPage, type, title, sizeX, sizeY, position, configuration) {
    assert(id !== undefined, "id of a widget must be defined");
    assert(idPage !== undefined, "idPage of a widget must be defined");
    assert(type !== undefined, "type of a widget must be defined");
    assert(title !== undefined, "type of a widget must be defined");
    assert(sizeX !== undefined, "sizeX of a widget must be defined");
    assert(sizeY !== undefined, "sizeY of a widget must be defined");
    assert(position !== undefined, "position of a widget must be defined");
    //configuration can be undefined

    this.id = id;
    this.idPage = idPage;
    this.type = type;
    this.title = title;

    //we save initial values that could change over the time
    this.initialValues = {};
    this.initialValues.sizeX = sizeX;
    this.initialValues.sizeY = sizeY;
    this.initialValues.position = position;

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
        sizeX: this.getWidth() || this.initialValues.sizeX,
        sizeY: this.getHeight() || this.initialValues.sizeY,
        position: this.getPosition() || this.initialValues.position,
        configuration: this.configuration
    };
};

/**
 * Get the position of the current widget in the page
 * @returns {int} return the position begining by 0. -1 if element is not on a page 
 */
Widget.prototype.getPosition = function () {
    var page = PageManager.getPage(this.idPage);
    if (!page)
        return null;
    
    var value = $(page.$grid.packery('getItemElements')).index(this.$gridWidget);
    return (value !== -1) ? value : null;
};

Widget.prototype.getHeight = function () {
    return (this.$gridWidget) ? this.$gridWidget.height() : null;
};

Widget.prototype.getWidth = function () {
    return (this.$gridWidget) ? this.$gridWidget.width() : null;
};

Widget.prototype.getInnerHeight = function () {
    return (this.$gridWidget) ? this.$content.height() : null;
};

Widget.prototype.getInnerWidth = function () {
    return (this.$gridWidget) ? this.$content.width() : null;
};

Widget.prototype.setHeight = function (newHeight) {
    if ((this.$gridWidget)) {
        this.$gridWidget.height(newHeight);
        this.$content.height(newHeight - this.$gridWidget.find("div.panel-widget-header").height());
    }
};

Widget.prototype.setWidth = function (newWidth) {
    if ((this.$gridWidget)) {
        this.$gridWidget.width(newWidth);
        this.$content.width(newWidth);
    }
};

Widget.prototype.setInitialPosition = function (position) {
    this.initialValues.position = position;
};

Widget.prototype.title = function () {
    return this.title;
};