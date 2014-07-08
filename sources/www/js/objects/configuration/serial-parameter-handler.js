/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a device parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function SerialParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   if (!isNullOrUndefined(currentValue)) {
      this.value = currentValue;
   }
   else {
      this.value = {};
   }

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;

   var self = this;
   
   //we async ask for serial port list
   $.getJSON("/rest/general/serialport")
      .done(populateSerialPorts(self))
      .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingSerialPortList"));});
}

/**
 * Asynchronous populate the list
 * @param handler
 * @returns {Function}
 */
function populateSerialPorts(handler) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("modals.configure-widget.errorDuringGettingSerialPortList"), JSON.stringify(data));
         return;
      }

      var $serialList = $("select#" + handler.uuid);
      $serialList.empty();

      var itemToSelect = 0;

      //there is no serial port
      if (data.data.serialPorts.length == 0) {
          //we add an empty element to make the validation throw an error
          $serialList.append("<option value=\"\"></option>");
      }
      else {
          $.each(data.data.serialPorts, function(index, value) {
             $serialList.append("<option value=\"" + value.name + "\">" + value.friendlyName + "</option>");
             if (value.name == handler.value)
                itemToSelect = index;
          });
      }

      //we set the last selected serialPort if it exist anymore
      $serialList.prop('selectedIndex', itemToSelect);
      $serialList.change();
   };
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
SerialParameterHandler.prototype.getDOMObject = function () {
   var input = "<select " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   var i18nData = " data-i18n=\"";

   var self = this;
   //i18nData += "[data-content]" + self.i18nContext + self.paramName + ".description";

   //i18nData += "\" ";
   input += " >";
   input += "</select>";

   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Get the param name
 * @returns {string}
 */
SerialParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {string}
 */
SerialParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("select#" + this.uuid).val();
   return this.value;
};