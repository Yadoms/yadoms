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
    this.toolbarActivated = true; //by default the toolbar is activated

    //we save initial values that could change over the time
    this.initialValues = {};
    this.initialValues.sizeX = sizeX;
    this.initialValues.sizeY = sizeY;
    this.initialValues.position = position;

    this.configuration = configuration || "";

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
    }
};

Widget.prototype.setWidth = function (newWidth) {
    if ((this.$gridWidget)) {
        this.$gridWidget.width(newWidth);
        //this.$content.width(newWidth);
    }
};

Widget.prototype.setInitialPosition = function (position) {
    this.initialValues.position = position;
};

Widget.prototype.title = function () {
    return this.title;
};


Widget.prototype.getBoundConfigurationSchema = function() {
   var d = new $.Deferred();

   if (!isNullOrUndefined(this.package)) {
      if (this.package.configurationSchema && Object.keys(this.package.configurationSchema).length > 0) {
         var tmp = this.package.configurationSchema;
         this.applyBindingPrivate(tmp, ["system"])
            .done(d.resolve)
            .fail(d.reject);
      } else {
         //if configurationSchema is not defined, to not try to do any binding...
         //just resolve with undefined configurationSchema
         d.resolve(this.package.configurationSchema);
      }
   } else {
      d.reject("undefined package");
   }
   return d.promise();
};

/**
 * Apply binding
 * @param item The configuration item
 * @param allowedTypes Allowed types for this item
 * @returns {*}
 */
Widget.prototype.applyBindingPrivate = function(item, allowedTypes) {
   assert(!isNullOrUndefined(item), "item must be defined");
   assert(!isNullOrUndefined(allowedTypes), "allowedTypes must be defined");
   var self = this;

   var d = new $.Deferred();

   var arrayOfDeffered = [];

   $.each(item, function(key, confItem) {
      if ($.isPlainObject(confItem)) {
         if (!isNullOrUndefined(confItem.__Binding__)) {
            //we've got binding
            assert(!isNullOrUndefined(confItem.__Binding__.type), "type must be defined in binding");
            //is the binding type is allowed

            assert($.inArray(confItem.__Binding__.type.toLowerCase(), allowedTypes) != -1, "Binding of type " + confItem.__Binding__.type + " is not allowed here");
            assert(!isNullOrUndefined(confItem.__Binding__.query), "query must be defined in binding");

            switch (confItem.__Binding__.type.toLowerCase()) {
            case "system":
               //we ask synchronously the bound value
               var deffered = RestEngine.getJson("/rest/system/binding/" + confItem.__Binding__.query);
               arrayOfDeffered.push(deffered);
               deffered.done(function(data) {
                     //we replace the binded value by the result
                  item[key] = data;
                  })
                  .fail(function(error) {
                     notifyError($.t("objects.pluginInstance.errorApplyingBinding"), error);
                  });
               break;
            }
         } else {
            var innerDeferred = self.applyBindingPrivate(confItem, allowedTypes);
            arrayOfDeffered.push(innerDeferred);
            innerDeferred.done(function(data) {
               confItem = data;
            });
         }
      }
   });

   if (arrayOfDeffered.length > 0) {
      $.whenAll(arrayOfDeffered).done(function() {
         d.resolve(item);
      });
   } else {
      d.resolve(item);
   }

   return d.promise();
};