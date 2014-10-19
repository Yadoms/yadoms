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
   assert(content.expectedCapacity !== undefined || content.expectedKeywordType !== undefined, "expectedCapacity or expectedKeywordType field must be defined");
   assert(content.expectedKeywordAccess !== undefined, "expectedKeywordAccess field must be defined");

   this.expectedKeywordAccess = content.expectedKeywordAccess;

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

   if (isNullOrUndefined(content.expectedCapacity)) {
      //we look for a type
      this.expectedKeywordType = content.expectedKeywordType;
      this.lookupMethod = "type";

      //we async ask for keyword list of the device
      $.getJSON("/rest/device/matchcapacitytype/" + self.expectedKeywordAccess + "/" + self.expectedKeywordType)
         .done(populateDeviceList(self))
         .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacityType", {expectedKeywordAccess : self.expectedKeywordAccess, expectedKeywordType : self.expectedKeywordType}));});

   }
   else
   {
      //we look for a capacity name
      this.expectedCapacity = content.expectedCapacity;
      this.lookupMethod = "name";

      //we async ask for device list that support expectedKeyword
      $.getJSON("/rest/device/matchcapacity/" + self.expectedKeywordAccess + "/" + self.expectedCapacity)
         .done(populateDeviceList(self))
         .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedKeywordAccess : self.expectedKeywordAccess, expectedCapacity : self.expectedCapacity}));});
   }
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

      //there is no device that match criteria
      if (data.data.device.length == 0) {
          //we add an empty element to make the validation throw an error
          $deviceList.append("<option value=\"\"></option>");
      }
      else {
          var itemToSelect = 0;
          $.each(data.data.device, function(index, value) {
             $deviceList.append("<option value=\"" + value.id + "\">" + decodeURIComponent(value.friendlyName) + "</option>");
             if (value.id == handler.value.deviceId)
                itemToSelect = index;
          });

          //we listen for the changed value to get all keywords that match to the capacity name or the capacity type
          $deviceList.change(function() {
             //until we have no answer the combo will be disabled
             $deviceList.prop('disabled', true);
             //we ask for all keywords of the device and we will pick just those we need
             $.getJSON("/rest/device/" + $("select#" + handler.uuid).val() + "/keyword")
                .done(function(data2) {
                   //we parse the json answer
                   if (data2.result != "true")
                   {
                      notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"), JSON.stringify(data));
                      return;
                   }

                   var $cbKeywords = $("select#" + handler.uuidKeywordList);
                   $cbKeywords.empty();

                   var newList = [];

                   $.each(data2.data.keyword, function(index, value) {
                      //we add the keyword only if access mode is at least the same than expected
                      if ((handler.expectedKeywordAccess.toLowerCase() != "getset") || (value.accessMode.toLowerCase() != "get")) {
                          if (handler.lookupMethod == "name") {
                             //we lookup by capacity name
                             if (value.capacityName.toLowerCase() == handler.expectedCapacity.toLowerCase()) {
                                //this keyword interest us we push it into the list
                                newList.push(value);
                             }
                          }
                          else {
                              //we lookup by capacity type
                             if (value.type.toLowerCase() == handler.expectedKeywordType.toLowerCase()) {
                                //this keyword interest us we push it into the list
                                newList.push(value);
                             }
                          }
                       }
                   });

                   //if there is only one item in the list we hide else it's visible
                   if (newList.length > 1) {
                      $cbKeywords.removeClass("hidden");
                   }
                   else {
                      $cbKeywords.addClass("hidden");
                   }

                   //we append each keywords in the list
                   var keywordToSelect = 0;

                   $.each(newList, function(index, value) {
                      //foreach keyword
                      $cbKeywords.append("<option value=\"" + value.id + "\">" + decodeURIComponent(value.friendlyName) + "</option>");

                      //we restore previously set value only if the deviceId is the same than the last one in the configuration
                      if (handler.value.deviceId == $deviceList.val()) {
                         if (value.id == handler.value.keywordId)
                            keywordToSelect = index;
                      }
                   });

                   //we select the last selected or the first one
                   $cbKeywords.prop('selectedIndex', keywordToSelect);
                })
                .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"));})
                .always(function() {
                   //we re-enable the device combo
                   $deviceList.prop('disabled', false);
                });
          });

          //we set the last selected device if it exist anymore
          $deviceList.prop('selectedIndex', itemToSelect);
          $deviceList.change();
      }
   };
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
DeviceParameterHandler.prototype.getDOMObject = function () {
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
   input += "<option></option></select>";

   input += "<select class=\"form-control hidden\" id=\"" + self.uuidKeywordList + "\"></select>";

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