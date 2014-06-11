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
function DeviceParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(content.expectedCapacity !== undefined, "expectedCapacity field must be defined");
   assert(content.expectedCapacityAccess !== undefined, "expectedCapacityAccess field must be defined");

   this.expectedCapacity = content.expectedCapacity;
   this.expectedCapacityAccess = content.expectedCapacityAccess;

   if (!isNullOrUndefined(currentValue)) {
      this.value = currentValue;
   }
   else {
      this.value = {};
   }

   this.name = content.name;
   this.uuid = createUUID();
   this.uuidKeywordList = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;

   var self = this;
   //we async ask for device list that support expectedKeyword
   $.getJSON("/rest/device/matchcapacity/" + self.expectedCapacityAccess + "/" + self.expectedCapacity)
      .done(populateDeviceList(self))
      .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedCapacityAccess : self.expectedCapacityAccess, expectedCapacity : self.expectedCapacity}));});
}

/**
 * Asynchronous populate the list
 * @param handler
 * @returns {Function}
 */
function populateDeviceList(handler) {
   return function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("modals.configure-widget.errorDuringGettingDeviceList"), JSON.stringify(data));
         return;
      }

      var $deviceList = $("select#" + handler.uuid);
      $deviceList.empty();

      var itemToSelect = 0;
      $.each(data.data.device, function(index, value) {
         $deviceList.append("<option value=\"" + value.id + "\">" + value.name + "</option>");
         if (value.id == handler.value.deviceId)
            itemToSelect = index;
      });

      //we listen for the changed value to get all keywords that match to the capacity
      $deviceList.change(function() {
         //until we have no answer the combo will be disabled
         $deviceList.prop('disabled', true);
         //we ask for keywords
         $.getJSON("/rest/device/" + $("select#" + handler.uuid).val() + "/" + handler.expectedCapacityAccess + "/" + handler.expectedCapacity)
            .done(function(data2) {
               //we parse the json answer
               if (data2.result != "true")
               {
                  notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"), JSON.stringify(data));
                  return;
               }

               var $cbKeywords = $("select#" + handler.uuidKeywordList);
               $cbKeywords.empty();

               //if there is only one the list is hidden else it's visible
               if (data2.data.keyword.count > 1) {
                  $cbKeywords.addClass("hidden");
               }
               else {
                  $cbKeywords.removeClass("hidden");
               }

               //we append each keywords in the list
               var keywordToSelect = 0;

               $.each(data2.data.keyword, function(index, value) {
                  //foreach keyword
                  $cbKeywords.append("<option value=\"" + value.id + "\">" + value.name + "</option>");

                  //we restore previously set value only if the deviceId is the same than the last one in the configuration
                  if (handler.value.deviceId == $deviceList.val()) {
                     if (value.id == handler.value.keywordId)
                        keywordToSelect = index;
                  }
               });

               //we select the last selected or the first one
               $cbKeywords.prop('selectedIndex', keywordToSelect);
            })
            .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingKeywordListMatchCapacity", {expectedCapacityAccess : handler.expectedCapacityAccess, expectedCapacity : handler.expectedCapacity}));})
            .always(function() {
               //we re-enable the device combo
               $deviceList.prop('disabled', false);
            });
      });

      //we set the last selected device if it exist anymore
      $deviceList.prop('selectedIndex', itemToSelect);
      $deviceList.change();
   };
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
DeviceParameterHandler.prototype.getDOMObject = function () {
   var input = "<select " +
                        "class=\"form-control\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"";// +
                        //"required ";
   var i18nData = " data-i18n=\"";

   var self = this;
   //i18nData += "[data-content]" + self.i18nContext + self.paramName + ".description";

   //i18nData += "\" ";
   input += " >";
   input += "</select>";

   input += "<select class=\"form-control hidden\" id=\"" + self.uuidKeywordList + "\"></seclect>";

   input += "<div class=\"device-details\">" +
              "" +
            "</div>";

   return ConfigurationHelper.createControlGroup(self, input);
};

/**
 * Get the param name
 * @returns {string}
 */
DeviceParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Get the current configuration in the form
 * @returns {string}
 */
DeviceParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value.deviceId = $("select#" + this.uuid).val();
   this.value.keywordId = $("select#" + this.uuidKeywordList).val();
   return this.value;
};