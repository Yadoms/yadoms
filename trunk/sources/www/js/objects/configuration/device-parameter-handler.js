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
   }
   else
   {
      //we look for a capacity name
      this.expectedCapacity = content.expectedCapacity;
      this.lookupMethod = "name";
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

          //we set the last selected device if it exist anymore
          if (itemToSelect != -1)
            $deviceList.prop('selectedIndex', itemToSelect);

          $deviceList.change();
      }
   };
}

/**
 * Apply script after DOM object has been added to the page
 * @returns {none}
 */
DeviceParameterHandler.prototype.applyScript = function () {

   //we listen for the changed value to get all keywords that match to the capacity name or the capacity type
   var $deviceList = $("select#" + this.uuid);
   var self = this;
   $deviceList.change(function(handler) {
      return function() {
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
                     }
                     else {
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
      }
   }(self));

   //we launch the async request to fill the combos
   if (self.lookupMethod == "type") {
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
   }
   else
   {
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
   }
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
   input += "</select>";

   input += "<select class=\"form-control\" id=\"" + self.uuidKeywordList + "\"></select>";

   input += "<div class=\"device-details\">" +
              "" +
            "</div>";
/*
   input += "<script>";
   input += "$deviceList.change(function() {\n" +
   "		//until we have no answer the combo will be disabled\n" +
   "		$deviceList.prop('disabled', true);\n" +
   "		//we ask for all keywords of the device and we will pick just those we need\n" +
   "		$.getJSON(\"/rest/device/\" + $(\"select#\" + " + self.uuid + ").val() + \"/keyword\")\n" +
   "		   .done(function(data2) {\n" +
   "			  //we parse the json answer\n" +
   "			  if (data2.result != \"true\")\n" +
   "			  {\n" +
   "				 notifyError($.t(\"modals.configure-widget.errorDuringGettingKeywordList\"), JSON.stringify(data));\n" +
   "				 return;\n" +
   "			  }\n" +
   "\n" +
   "			  var $cbKeywords = $(\"select#\" + " + self.uuidKeywordList + ");\n" +
   "			  $cbKeywords.empty();\n" +
   "\n" +
   "			  var newList = [];\n" +
   "\n" +
   "			  $.each(data2.data.keyword, function(index, value) {\n" +
   "				 //we add the keyword only if access mode is at least the same than expected\n" +
   "				 if ((" + self.expectedKeywordAccess.toLowerCase() + " != \"getset\") || (value.accessMode.toLowerCase() != \"get\")) {\n" +
   "					if (" + self.lookupMethod + " == \"name\") {\n" +
   "					   //we lookup by capacity name\n" +
   "					   if (value.capacityName.toLowerCase() == " + self.expectedCapacity.toLowerCase() + ") {\n" +
   "						  //this keyword interest us we push it into the list\n" +
   "						  newList.push(value);\n" +
   "					   }\n" +
   "					}\n" +
   "					else {\n" +
   "					   //we lookup by capacity type\n" +
   "					   if (value.type.toLowerCase() == " + self.expectedKeywordType.toLowerCase() + ") {\n" +
   "						  //this keyword interest us we push it into the list\n" +
   "						  newList.push(value);\n" +
   "					   }\n" +
   "					}\n" +
   "				 }\n" +
   "			  });\n" +
   "\n" +
   "			  //if there is only one item in the list we hide else it's visible\n" +
   "			  if (newList.length > 1) {\n" +
   "				 $cbKeywords.removeClass(\"hidden\");\n" +
   "			  }\n" +
   "			  else {\n" +
   "				 $cbKeywords.addClass(\"hidden\");\n" +
   "			  }\n" +
   "\n" +
   "			  //we append each keywords in the list\n" +
   "			  var keywordToSelect = 0;\n" +
   "\n" +
   "			  $.each(newList, function(index, value) {\n" +
   "				 //foreach keyword\n" +
   "				 $cbKeywords.append(\"<option value='\" + value.id + \"'>\" + decodeURIComponent(value.friendlyName) + \"</option>\");\n" +
   "\n" +
   "				 //we restore previously set value only if the deviceId is the same than the last one in the configuration\n" +
   "				 if (" + self.value.deviceId + "== $deviceList.val()) {\n" +
   "					if (value.id == " + self.value.keywordId + ")\n" +
   "					   keywordToSelect = index;\n" +
   "				 }\n" +
   "			  });\n" +
   "\n" +
   "			  //we select the last selected or the first one\n" +
   "			  $cbKeywords.prop('selectedIndex', keywordToSelect);\n" +
   "		   })\n" +
   "		   .fail(function() {notifyError($.t(\"modals.configure-widget.errorDuringGettingKeywordList\"));})\n" +
   "		   .always(function() {\n" +
   "			    //we re-enable the device combo\n" +
   "			    $deviceList.prop('disabled', false);\n" +
   "		   });\n" +
   "	});\n" +
   "</script>"

*/
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