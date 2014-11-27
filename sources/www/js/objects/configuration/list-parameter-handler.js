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
debugger;
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
   debugger;
   var input = "<div class=\"control-group configuration-section well\" >" +
                  "<div class=\"configuration-header\" >";

   input +=                "<span data-i18n=\"" + this.i18nContext + this.paramName + ".name\" >" +
                              this.name +
                           "</span>";
   input +=                "<button class=\"pull-right btn btn-primary\" id=\"" + this.addBtnUuid + "\" type=\"button\"><span><i class=\"glyphicon glyphicon-plus\"></i></span></button>";

   input +=       "</div>" +
                  "<div class=\"configuration-description\" data-i18n=\"" + this.i18nContext + this.paramName + ".description\" >" +
                     this.description +
                  "</div>" +
                  "<div id=\"" + this.uuid + "\" >";

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
   //TODO : add control boutons remove / duplicate
   var itemLine = "<div class=\"well\" >" +
                     "<div class=\"col-md-11\" >" +
                        item.getDOMObject() +
                      "</div>" +
                      "<button class=\"col-md1 btn btn-primary btn-remove\" type=\"button\"><span><i class=\"glyphicon glyphicon-minus\"></i></span></button>" +
                  "</div>";

   return itemLine;
}

/**
 * Apply script after DOM object has been added to the page
 * @returns {}
 */
ListParameterHandler.prototype.applyScript = function () {
debugger;
   var self = this;

   $("button#" + self.addBtnUuid).unbind("click").bind("click", function() {
      var newI18nContext = self.i18nContext + self.paramName + ".item.";
      var item = ConfigurationHelper.createParameterHandler(newI18nContext, self.paramName, self.content.item, "");
      self.items.push(item);
      var itemLine = self.createItemLine(item);
      $("div#" + self.uuid).append(itemLine);
      if ($.isFunction(item.applyScript))
         item.applyScript();
   });

   //we apply script in each children
   $.each(self.items, function (key, value) {
      if ($.isFunction(value.applyScript))
         value.applyScript();
   });
}

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