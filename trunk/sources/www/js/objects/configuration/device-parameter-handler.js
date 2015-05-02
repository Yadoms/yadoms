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
   assert(content.expectedKeywordAccess !== undefined, "expectedKeywordAccess field must be defined");

   this.expectedKeywordAccess = content.expectedKeywordAccess;

   if (!isNullOrUndefined(currentValue)) {
      this.value = currentValue;
   }
   else {
      this.value = {};
   }

   this.name = content.name;
   this.uuidContainer = createUUID();
   this.uuid = createUUID();
   this.uuidKeywordList = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;

   if (!isNullOrUndefined(content.expectedKeywordType)) {
      //we look for a type
      this.expectedKeywordType = content.expectedKeywordType;
      this.lookupMethod = "type";
   }
   else if (!isNullOrUndefined(content.expectedCapacity)) {
      //we look for a capacity name
      this.expectedCapacity = content.expectedCapacity;
      this.lookupMethod = "name";
   }
   else {
      //we look for all
      this.lookupMethod = "all";
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

      //A device matches criteria
      if (data.data.device.length != 0) {
          var itemToSelect = -1;
          $.each(data.data.device, function(index, value) {
             //we add device only if it is not already in the list
             if ($deviceList.find("option[id=\"" + value.id + "\"]").length == 0) {
               $deviceList.append("<option value=\"" + value.id + "\">" + decodeURIComponent(value.friendlyName) + "</option>");
               //if the new element is those that we are looking for we save the position in the list
               if (value.id == handler.value.deviceId)
                  itemToSelect = $deviceList.find("option").length - 1;
             }
          });

          //until we have no device selected we have no keyword to select
          var $cbKeywords = $("select#" + handler.uuidKeywordList);
          $cbKeywords.prop('disabled', true);

          //we set the last selected device if it exist anymore
          if (itemToSelect != -1)
            $deviceList.prop('selectedIndex', itemToSelect);

          $deviceList.change();
      }
   };
}

/**
 * Apply script after DOM object has been added to the page
 */
DeviceParameterHandler.prototype.applyScript = function () {

   //we listen for the changed value to get all keywords that match to the capacity name or the capacity type
   var $deviceList = $("select#" + this.uuid);
   var $cbKeywords = $("select#" + this.uuidKeywordList);

   //until no device has been select the list is disabled
   $cbKeywords.prop('disabled', true);

   var self = this;

   $deviceList.change(function(handler) {
      return function() {

         if (isNullOrUndefinedOrEmpty($("select#" + handler.uuid).val())) {
            //the line selected is the empty one
            //we clear and disable the second combo
            $cbKeywords.empty();
            $cbKeywords.prop('disabled', true);
            $cbKeywords.append("<option value=\"\"></option>");
            handler.locateInDOM().change();
         }
         else {
            //until we have no answer the combos will be disabled
            $deviceList.prop('disabled', true);
            $cbKeywords.prop('disabled', true);

            //we ask for all keywords of the device and we will pick just those we need
            $.getJSON("/rest/device/" + $("select#" + handler.uuid).val() + "/keyword")
               .done(function(data2) {
                  //we parse the json answer
                  if (data2.result != "true")
                  {
                     notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"), JSON.stringify(data2));
                     return;
                  }

                  $cbKeywords.empty();
                  $cbKeywords.append("<option value=\"\"></option>");
                  var newList = [];

                  $.each(data2.data.keyword, function(index, value) {
                     //we add the keyword only if access mode is at least the same than expected
                     if ((handler.expectedKeywordAccess.toLowerCase() != "getset") || (value.accessMode.toLowerCase() != "get")) {
                        switch (handler.lookupMethod)
                        {
                           case "name":
                              //we lookup by capacity name (configuration should have several capacities)
                              if (Array.isArray(handler.expectedCapacity)) {
                                 $.each(handler.expectedCapacity, function(indexHandlerCapacity, handlerCapacity) {
                                    if (value.capacityName.toLowerCase() == handlerCapacity.toLowerCase()) {
                                       //this keyword interest us we push it into the list
                                       newList.push(value);
                                    }
                                 });
                              }
                              else {
                                 if (value.capacityName.toLowerCase() == handler.expectedCapacity.toLowerCase()) {
                                    //this keyword interest us we push it into the list
                                    newList.push(value);
                                 }
                              }
                              break;

                           case "type":
                              //we lookup by capacity type (configuration should have several types)
                              if (Array.isArray(handler.expectedKeywordType)) {
                                 $.each(handler.expectedKeywordType, function(indexHandlerKwType, handlerKwType) {
                                    if (value.type.toLowerCase() == handlerKwType.toLowerCase()) {
                                       //this keyword interest us we push it into the list
                                       newList.push(value);
                                    }
                                 });
                              }
                              else {
                                 if (value.type.toLowerCase() == handler.expectedKeywordType.toLowerCase()) {
                                    //this keyword interest us we push it into the list
                                    newList.push(value);
                                 }
                              }
                              break;

                           default:
                           case "all":
                              //we take everyone
                              newList.push(value);
                              break;
                        }
                     }
                  });

                  //we append each keywords in the list
                  var keywordToSelect = 0;

                  $.each(newList, function(index, value) {
                     //foreach keyword
                     $cbKeywords.append("<option value=\"" + value.id + "\">" + decodeURIComponent(value.friendlyName) + "</option>");

                     //we restore previously set value only if the deviceId is the same than the last one in the configuration
                     if (handler.value.deviceId == $deviceList.val()) {
                        if (value.id == handler.value.keywordId)
                           keywordToSelect = index + 1; //the +1 is for the empty line
                     }
                  });

                  //we select the last selected or the first one
                  $cbKeywords.prop('selectedIndex', keywordToSelect);
                  handler.locateInDOM().change();
               })
               .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"));})
               .always(function() {
                  //we re-enable the device combo
                  $deviceList.prop('disabled', false);
                  $cbKeywords.prop('disabled', false);
               });
         }
      }
   }(self));

   //we launch the async request to fill the device combos
   switch (self.lookupMethod)
   {
      case "type":
         //we look for a type
         if (Array.isArray(self.expectedKeywordType)) {
            //we have a list of types
            //we async ask for device list that support a type
            $.each(self.expectedKeywordType, function (index, type) {
               $.getJSON("/rest/device/matchcapacitytype/" + self.expectedKeywordAccess + "/" + type)
                  .done(populateDeviceList(self))
                  .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacityType", {expectedKeywordAccess : self.expectedKeywordAccess, expectedKeywordType : type}));});
            });
         }
         else {
            //we async ask for device list that support a type
            $.getJSON("/rest/device/matchcapacitytype/" + self.expectedKeywordAccess + "/" + self.expectedKeywordType)
               .done(populateDeviceList(self))
               .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacityType", {expectedKeywordAccess : self.expectedKeywordAccess, expectedKeywordType : self.expectedKeywordType}));});
         }
         break;
      case "name":
         //we look for a capacity name
         if (Array.isArray(self.expectedCapacity)) {
            //we have a list of capacities
            //we async ask for keyword list of the device for each capacity
            $.each(self.expectedCapacity, function (index, capacity) {
               $.getJSON("/rest/device/matchcapacity/" + self.expectedKeywordAccess + "/" + capacity)
                  .done(populateDeviceList(self))
                  .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedKeywordAccess : self.expectedKeywordAccess, expectedCapacity : self.expectedCapacity}));});
            });
         }
         else {
            //we have only one capacity
            $.getJSON("/rest/device/matchcapacity/" + self.expectedKeywordAccess + "/" + self.expectedCapacity)
               .done(populateDeviceList(self))
               .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedKeywordAccess : self.expectedKeywordAccess, expectedCapacity : self.expectedCapacity}));});
         }
         break;
      default:
      case "all":
         //we get all devices
         $.getJSON("/rest/device/")
            .done(populateDeviceList(self))
            .fail(function() {notifyError($.t("modals.configure-widget.errorDuringGettingDeviceList"));});
         break;
   }
};

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
DeviceParameterHandler.prototype.getDOMObject = function () {
   var input = "<div id=\"" + this.uuidContainer + "\"><select " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   var i18nData = " data-i18n=\"";

   var self = this;
   //i18nData += "[data-content]" + self.i18nContext + self.paramName + ".description";

   //i18nData += "\" ";
   input += " >";
   input += "<option value=\"\"></option>" +
            "</select>";

   input += "<select class=\"form-control\" id=\"" + self.uuidKeywordList + "\"></select>";

   input += "<div class=\"device-details\">" +
              "" +
            "</div></div>";
   return ConfigurationHelper.createControlGroup(self, input);
};

DeviceParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuidContainer);
}

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
   var value = {};
   value.deviceId = $("select#" + this.uuid).val();
   value.keywordId = $("select#" + this.uuidKeywordList).val();
   return value;
};