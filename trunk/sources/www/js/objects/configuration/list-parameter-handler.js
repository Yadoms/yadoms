/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a list parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @param radioButtonSectionName
 * @param radioSectionActive
 * @constructor
 */
function ListParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(content.item !== undefined, "content.content must be defined");

   this.items = [];
   this.configurationValues = currentValue;
   this.name = content.name;
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
   this.uuid = createUUID();
   this.addBtnUuid = createUUID();
   this.selectorUuid = createUUID();

   var self = this;

   //for each key in currentValue we re-create items previously saved
   if (!isNullOrUndefined(self.configurationValues)) {
      $.each(self.configurationValues, function (key, value) {

         var newI18nContext = i18nContext + self.paramName + ".item.";
         var item = ConfigurationHelper.createParameterHandler(newI18nContext, self.paramName, self.content.item, value);
         self.items.push(item);
      });
   }
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
ListParameterHandler.prototype.getDOMObject = function () {

   var self = this;
   var input = "<div class=\"control-group configuration-section well\" >" +
                  "<div class=\"configuration-header\" >";

   input +=                "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                              this.name +
                           "</span>";
   input +=                "<button class=\"pull-right btn btn-primary\" id=\"" + this.addBtnUuid + "\" type=\"button\"><span><i class=\"fa fa-file-o\"></i></span></button>";

   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div id=\"" + this.uuid + "\" class=\"well control-group list-item-container\">";

   //we append each param in the section
   $.each(this.items, function (key, value) {
      input += self.createItemLine(value);
      input += "\n";
   });

   input +=       "</div>" +
               "</div>";

   return input;
};

ListParameterHandler.prototype.createItemLine = function(item) {
   //div container of one item
   var itemLine = "<div class=\"list-item-line\">" +
                      "<div class=\"col-md-10\" >" +
                           item.getDOMObject() +
                      "</div>" +
                      "<div class=\"col-md-2\" >" +
                         "<div class=\"btn-group\" role=\"group\">" +
                            "<button class=\"btn btn-primary btn-duplicate\" type=\"button\"><span><i class=\"fa fa-files-o\"></i></span></button>" +
                            "<button class=\"btn btn-danger btn-remove\" type=\"button\"><span><i class=\"fa fa-trash-o\"></i></span></button>" +
                         "</div>" +
                      "</div>" +
                   "</div>";
   return itemLine;
}

/**
 * Apply script after DOM object has been added to the page
 * @returns {}
 */
ListParameterHandler.prototype.applyScript = function () {

   var self = this;

   $("button#" + self.addBtnUuid).unbind("click").bind("click", function() {
      var newI18nContext = self.i18nContext + self.paramName + ".item.";
      var item = ConfigurationHelper.createParameterHandler(newI18nContext, self.paramName, self.content.item, "");
      self.items.push(item);
      var itemLine = self.createItemLine(item);
      $("div#" + self.uuid).append(itemLine);
      $("div#" + self.uuid).i18n();

      //we manage event binding
      $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
      $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

      if ($.isFunction(item.applyScript))
         item.applyScript();
   });

   $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
   $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

   //we apply script in each children
   $.each(self.items, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });

   $("div#" + self.uuid).i18n();
}

ListParameterHandler.prototype.getPosFromSiblings = function(item) {
   var i = -1;
   var c = item;
   while (c.length > 0) {
      i++;
      c = c.prev();
   }
   return i;
}

ListParameterHandler.prototype.duplicateLine = function() {
   var self = this;
   return function(handler) {
      var $container = $(handler.currentTarget).parents("div.list-item-line");
      assert($container.length >= 1, "Unable to locate list item container");
      //we take the nearest

      //we look for the position of the current element
      var i = self.getPosFromSiblings($container);

      //i contains the position into the items array
      assert(i >= 0, "Unable to locate item position");

      var confToDuplicate = self.items[i].getCurrentConfiguration();

      var newI18nContext = self.i18nContext + self.paramName + ".item.";
      var item = ConfigurationHelper.createParameterHandler(newI18nContext, self.paramName, self.content.item, confToDuplicate);

      //we insert item in the right place
      self.items.insert(i+1, item);

      var itemLine = self.createItemLine(item);

      //and in the right place for the DOM

      $(itemLine).insertAfter($container);
      $("div#" + self.uuid).i18n();

      //we manage event binding
      $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
      $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

      if ($.isFunction(item.applyScript))
         item.applyScript();

      //TODO : duplicate $container[0]
   };
};

ListParameterHandler.prototype.deleteLine = function() {
   var self = this;
   return function(handler) {
      var $container = $(handler.currentTarget).parents("div.list-item-line");
      assert($container.length >= 1, "Unable to locate main item container");
      //we look for the position of the current element
      var i = self.getPosFromSiblings($container);

      //i contains the position into the items array
      assert(i >= 0, "Unable to locate item position");
      //we delete it from list
      self.items.splice(i, 1);
      //we delete it from the DOM
      $container[0].remove();
   };
};

/**
 * Get the param name
 * @returns {string}
 */
ListParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {object}
 */
ListParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var self = this;
   self.configurationValues = [];
   $.each(self.items, function (key, value) {
      self.configurationValues.push(value.getCurrentConfiguration());
   });

   return self.configurationValues;
};