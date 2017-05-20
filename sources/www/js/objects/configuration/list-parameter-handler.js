/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a list parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function ListParameterHandler(i18nContext, i18nKey, paramName, content, currentValue) {
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
   this.i18nKey = i18nKey || paramName;
   this.content = content;
   this.uuid = createUUID();
   this.addBtnUuid = createUUID();
   this.selectorUuid = createUUID();
   this.containerUuid = createUUID();
   //the checkbox for validation
   this.itemNumberTextBoxUuid = createUUID();

   this.nbItemsMin = parseFloat(content.nbItemsMin);
   if (!isNaN(this.nbItemsMin)) {
      //a number of max item can't be lower than min number of items
      this.nbItemsMax = Math.max(parseFloat(content.nbItemsMax), this.nbItemsMin);
   }
   else {
      this.nbItemsMax = parseFloat(content.nbItemsMax);
   }
   this.allowDuplication = parseBool(content.allowDuplication);

   var self = this;

   //for each key in currentValue we re-create items previously saved
   if (!isNullOrUndefined(self.configurationValues)) {
      $.each(self.configurationValues, function (key, value) {

         var newI18nContext = i18nContext + self.i18nKey + ".item.";
         var item = ConfigurationHelper.createParameterHandler(newI18nContext, value.i18nKey, null, self.content.item, value);
         self.items.push(item);
      });
   }
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
ListParameterHandler.prototype.getDOMObject = function () {

   //we create the header of the control group with a 0px height textbox to set the number of items
   //this textbox is used to make form validation

   var i18nData = " data-i18n=\"";
   var i18nOptions = {};

   var self = this;
   var input = "<div id=\"" + this.uuid + "\" class=\"control-group configuration-section well\" >" +
                  "<div class=\"configuration-header\" >";

   input +=                "<span data-i18n=\"" + this.i18nContext + this.i18nKey + ".name\" >" +
                              this.name +
                           "</span>";
   input +=                "<button class=\"pull-right btn btn-primary\" id=\"" + this.addBtnUuid + "\" type=\"button\"><span><i class=\"fa fa-file-o\"></i></span></button>";

   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.i18nKey + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div class=\"controls\" style=\"height: 0px\">" +
                     "<input type=\"number\" id=\"" + this.itemNumberTextBoxUuid + "\" class=\"enable-validation list-parameter-handler-validation-textbox\" ";

   if (!isNaN(this.nbItemsMin)) {
      input +=          "min=\"" + this.nbItemsMin + "\" ";
      i18nOptions["minValue"] = this.nbItemsMin;
   }

   if (!isNaN(this.nbItemsMax)) {
      input +=          "max=\"" + this.nbItemsMax + "\" ";
      i18nOptions["maxValue"] = this.nbItemsMax;
   }

   input +=             "data-i18n=\"" +
                            "[data-validation-max-message]configuration.validationForm.maxNumberItemsAllowed;" +
                            "[data-validation-min-message]configuration.validationForm.minNumberItemsRequired" +
                        "\"" +
                        "data-i18n-options=\'" + JSON.stringify(i18nOptions) + "\' />" +
                  "</div>" +
                  "<p class=\"help-block\"></p>" +
                  "<div id=\"" + this.containerUuid + "\" class=\"well control-group list-item-container\">";

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
                      "<div class=\"col-md-11\" >" +
                           item.getDOMObject() +
                      "</div>" +
                      "<div class=\"list-item-content col-md-1\" style =\"padding-right: 0px;\">" + //Fix here a problem with Firefox by fixing the padding-right to 0px.
                          "<div class=\"btn-group-vertical btn-group-sm\" role=\"group\">";
   if (this.allowDuplication)
      itemLine +=           "<button class=\"btn btn-primary btn-sm btn-duplicate\" type=\"button\" ><span ><i class=\"fa fa-files-o \"></i></span></button>";

   itemLine +=              "<button class=\"btn btn-danger btn-sm btn-remove\" type=\"button\"><span><i class=\"fa fa-trash-o\"></i></span></button>" +
                         "</div>" +
                      "</div>" +
                   "</div>";
   return itemLine;
};

ListParameterHandler.prototype.updateItemNumberVerificator = function() {
   //we get the number of item in the list and set it into a textbox to ensure form validation
   var count = $("div#" + this.uuid).find("div.list-item-line").length;
   $("input#" + this.itemNumberTextBoxUuid).val(count);
};

/**
 * Apply script after DOM object has been added to the page
 */
ListParameterHandler.prototype.applyScript = function () {
   var self = this;

   $("button#" + self.addBtnUuid).unbind("click").bind("click", function() {
      var newI18nContext = self.i18nContext + self.i18nKey + ".item.";
      var item = ConfigurationHelper.createParameterHandler(newI18nContext, undefined, null, self.content.item, "");
      self.items.push(item);
      var itemLine = self.createItemLine(item);
      $("div#" + self.uuid).find("div.list-item-container").append(itemLine);
      $("div#" + self.uuid).find("div.list-item-container").i18n();
      //we add it to the form validation
      $("div#" + self.uuid).find("div.list-item-container").last().find("input,select,textarea").jqBootstrapValidation();

      //we manage event binding
      $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
      $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

      self.updateItemNumberVerificator();

      if ($.isFunction(item.applyScript))
         item.applyScript();

   });

   $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
   $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

   //we update the number of item in the textbox for the verification
   self.updateItemNumberVerificator();

   //we apply script in each children
   $.each(self.items, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });

   $("div#" + self.uuid).i18n();
};

ListParameterHandler.prototype.getPosFromSiblings = function(item) {
   var i = -1;
   var c = item;
   while (c.length > 0) {
      i++;
      c = c.prev();
   }
   return i;
};

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

      var newI18nContext = self.i18nContext + self.i18nKey + ".item.";
      var item = ConfigurationHelper.createParameterHandler(newI18nContext, undefined, null, self.content.item, confToDuplicate);

      //we insert item in the right place
      self.items.insert(i+1, item);

      var itemLine = self.createItemLine(item);

      //and in the right place for the DOM

      $(itemLine).insertAfter($container);
      $("div#" + self.uuid).i18n();
      //we add it to the form validation
      $("div#" + self.uuid).find("div.list-item-container").last().find("input,select,textarea").jqBootstrapValidation();

      //we manage event binding
      $("div#" + self.uuid).find("button.btn-duplicate").unbind('click').bind('click', self.duplicateLine());
      $("div#" + self.uuid).find("button.btn-remove").unbind('click').bind('click', self.deleteLine());

      self.updateItemNumberVerificator();

      if ($.isFunction(item.applyScript))
         item.applyScript();
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

      self.updateItemNumberVerificator();
   };
};

ListParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
ListParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
ListParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation");
            $("#" + self.addBtnUuid).attr("disabled", false);
            $("#" + self.containerUuid + " > div.list-item-line > div.list-item-content > div > button").attr("disabled", false);
    } else {
            $("#" + self.uuid).removeClass("enable-validation");
            $("#" + self.addBtnUuid).attr("disabled", true);
            $("#" + self.containerUuid + " > div.list-item-line > div.list-item-content > div > button").attr("disabled", true);
    }

    $.each(self.items, function (key, value) {
      if ($.isFunction(value.setEnabled)) {
        value.setEnabled(enabled);
      }
    });
}

/**
 * Get the current configuration in the form
 * @returns {object}
 */
ListParameterHandler.prototype.getCurrentConfiguration = function () {
   //we update configurationValues with content of DOM
   var d= new $.Deferred();
   var self = this;
   self.configurationValues = [];
   var deferredArray =[];
   
   $.each(self.items, function (key, value) {
      var deferred = value.getCurrentConfiguration();
      deferredArray.push(deferred);
      deferred.done(function (data) {
          self.configurationValues.push(data);
      });      
   });


   $.whenAll(deferredArray)
   .done(function() {
      d.resolve(self.configurationValues);
   });   
   
   return d.promise();
};