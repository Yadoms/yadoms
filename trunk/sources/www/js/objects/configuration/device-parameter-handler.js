/**
 * Created by Nicolas on 01/03/14.
 */

function DeviceParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(content.expectedKeyword !== undefined, "expectedKeyword field must be defined");

   this.expectedKeyword = content.expectedKeyword;

   this.value = parseInt(currentValue);
   if (isNaN(this.value)) {
      this.value = 0;
   }

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;

   var self = this;
   //we async ask for device list that support expectedKeyword
   $.getJSON("/rest/device/matchkeyword/" + self.expectedKeyword)
      .done(populateDeviceList(self))
      .fail(function() {notifyError("Unable to get device list that match keyword " + slef.expectedKeyword)});
}

function populateDeviceList(handler) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError("Error during requesting devices");
         return;
      }

      var itemToSelect = 0;
      $.each(data.data.device, function(index, value) {
         $("#" + handler.uuid).append("<option value=\"" + value.id + "\">" + value.name + "</option>");
         if (value.id == handler.value)
            itemToSelect = index;
      });

      //we set the last selected device if it exist anymore
      $("#" + handler.uuid).prop('selectedIndex', itemToSelect);
   };
}

DeviceParameterHandler.prototype.getDOMObject = function () {
   var input = "<select " +
                        "class=\"form-control\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   var i18nData = " data-i18n=\"";

   var self = this;
   i18nData += "[data-content]" + self.i18nContext + self.paramName + ".description";

   i18nData += "\" ";
   input += i18nData + " >";
   input += "</select>";

   input += "<div class=\"device-details\">" +
              "" +
            "</div>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

DeviceParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

DeviceParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("select#" + this.uuid).val();
   return this.value;
};