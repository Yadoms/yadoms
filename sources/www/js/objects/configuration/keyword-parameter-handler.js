/**
 * Created by Nicolas on 01/03/14.
 */

/**
 * Create a keyword parameter handler
 * @param i18NContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function KeywordParameterHandler(i18NContext, paramName, content, currentValue) {
   assert(i18NContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(content && content.expectedKeywordAccess !== undefined, "expectedKeywordAccess field must be defined");
   if (content) {
      this.expectedKeywordAccess = content.expectedKeywordAccess;

      if (!isNullOrUndefined(currentValue)) {
         this.value = currentValue;
      } else {
         this.value = {};
      }

      this.name = content.name;
      this.uuidContainer = createUUID();
      this.uuid = createUUID();
      this.uuidKeywordList = createUUID();
      this.paramName = paramName;
      this.description = isNullOrUndefined(content.description) ? "" : content.description;
      this.i18nContext = i18NContext;
      this.content = content;

      if (!isNullOrUndefined(content.expectedKeywordType)) {
         //we look for a type
         this.expectedKeywordType = content.expectedKeywordType;
         this.lookupMethod = "type";
      } else if (!isNullOrUndefined(content.expectedCapacity)) {
         //we look for a capacity name
         this.expectedCapacity = content.expectedCapacity;
         this.lookupMethod = "name";
      } else {
         //we look for all
         this.lookupMethod = "all";
      }
   }
}

/**
 * Return all devices
 * @returns {Function}
 */

function returnDevices (handler, tabDevice, devicePromise)
{
   return function(data) {
      var $deviceList = $("select#" + handler.uuid);

      $.each(data.device, function(index, value) {
          tabDevice.push ( value );
      });
      
      devicePromise.resolve();
   };
}

/**
 * Asynchronous populate the list
 * @param handler
 * @returns {Function}
 */
function addDeviceList(handler, tabDevice) {
   
     var $deviceList = $("select#" + handler.uuid);

   //A device matches criteria
   if (tabDevice.length !== 0) {
     var itemToSelect = -1;
     var listDevice = sortListItemsWithFriendlyName ( tabDevice );
     
       $.each(listDevice, function(index, value) {
          //we add device only if it is not already in the list
          if ($deviceList.find("option[value=\"" + value.id + "\"]").length === 0 && parseBool(value.blacklist) === false) {
            $deviceList.append("<option value=\"" + value.id + "\">" + value.friendlyName + "</option>");
            //if the new element is those that we are looking for we save the position in the list
            if (value.id == handler.value.deviceId)
               itemToSelect = $deviceList.find("option").length - 1;
          }
       });

       //until we have no device selected we have no keyword to select
       var $cbKeywords = $("select#" + handler.uuidKeywordList);
       $cbKeywords.prop('disabled', true);

       //we set the last selected device if it exist anymore
       if (itemToSelect !== -1)
         $deviceList.prop('selectedIndex', itemToSelect);

       $deviceList.change();
   }
}

/**
 * Asynchronous populate the list
 * @param handler
 * @returns {Function}
 */
 
function populateDeviceList(handler) {
   return function(data) {
      var $deviceList = $("select#" + handler.uuid);
	  
	  var tabDevice = [];

      //A device matches criteria
      if (data.device.length !== 0) {
          var itemToSelect = -1;
		  
          $.each(data.device, function(index, value) {
             tabDevice.push ( value );
          });
		  
		  tabDevice = sortListItemsWithFriendlyName ( tabDevice );
		  
          $.each(tabDevice, function(index, value) {
			  
             //we add device only if it is not already in the list
             if ($deviceList.find("option[value=\"" + value.id + "\"]").length === 0 && parseBool(value.blacklist) === false) {
               $deviceList.append("<option value=\"" + value.id + "\">" + value.friendlyName + "</option>");
               //if the new element is those that we are looking for we save the position in the list
               if (value.id == handler.value.deviceId)
                  itemToSelect = $deviceList.find("option").length - 1;
             }
          });

          //until we have no device selected we have no keyword to select
          var $cbKeywords = $("select#" + handler.uuidKeywordList);
          $cbKeywords.prop('disabled', true);

          //we set the last selected device if it exist anymore
          if (itemToSelect !== -1)
            $deviceList.prop('selectedIndex', itemToSelect);

          $deviceList.change();
      }
   };
}

/**
 * Apply script after DOM object has been added to the page
 */
KeywordParameterHandler.prototype.applyScript = function () {

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
            RestEngine.getJson("/rest/device/" + $("select#" + handler.uuid).val() + "/keyword")
               .done(function(data2) {
                  $cbKeywords.empty();
                  $cbKeywords.append("<option value=\"\"></option>");
                  var newList = [];

                  $.each(data2.keyword, function(index, value) {
                     //we add the keyword only if access mode is at least the same than expected
                     if ((handler.expectedKeywordAccess.toLowerCase() !== "getset") || (value.accessMode.toLowerCase() !== "get")) {
                        switch (handler.lookupMethod)
                        {
                           case "name":
                              //we lookup by capacity name (configuration should have several capacities)
                              if (Array.isArray(handler.expectedCapacity)) {
                                 $.each(handler.expectedCapacity, function(indexHandlerCapacity, handlerCapacity) {
                                    if (value.capacityName.toLowerCase() === handlerCapacity.toLowerCase()) {
                                       //this keyword interest us we push it into the list
                                       newList.push(value);
                                    }
                                 });
                              }
                              else {
                                 if (value.capacityName.toLowerCase() === handler.expectedCapacity.toLowerCase()) {
                                    //this keyword interest us we push it into the list
                                    newList.push(value);
                                 }
                              }
                              break;

                           case "type":
                              //we lookup by capacity type (configuration should have several types)
                              if (Array.isArray(handler.expectedKeywordType)) {
                                 $.each(handler.expectedKeywordType, function(indexHandlerKwType, handlerKwType) {
                                    if (value.type.toLowerCase() === handlerKwType.toLowerCase()) {
                                       //this keyword interest us we push it into the list
                                       newList.push(value);
                                    }
                                 });
                              }
                              else {
                                 if (value.type.toLowerCase() === handler.expectedKeywordType.toLowerCase()) {
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
              
				  newList = sortListItemsWithFriendlyName ( newList );

                  $.each(newList, function(index, value) {
                     //foreach keyword
                     $cbKeywords.append("<option value=\"" + value.id + "\">" + value.friendlyName + "</option>");

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
               .fail(function(error) {
                  notifyError($.t("modals.configure-widget.errorDuringGettingKeywordList"), error);
               })
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
            
            var arrayOfDeffered = [];
            var tabDevice = [];            
            
            //we have a list of types
            //we async ask for device list that support a type
            $.each(self.expectedKeywordType, function (index, type) {
               
               var promise = new $.Deferred();
               arrayOfDeffered.push(promise);               
               
               RestEngine.getJson("/rest/device/matchcapacitytype/" + self.expectedKeywordAccess + "/" + type)
                  .done(returnDevices(self, tabDevice, promise))
                  .fail(function(error) {
                     notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacityType", {expectedKeywordAccess : self.expectedKeywordAccess, expectedKeywordType : type}), error);
                  });
            });
            $.whenAll(arrayOfDeffered).done(function () {
               addDeviceList(self, tabDevice);
            });            
         }
         else {
            //we async ask for device list that support a type
            RestEngine.getJson("/rest/device/matchcapacitytype/" + self.expectedKeywordAccess + "/" + self.expectedKeywordType)
               .done(populateDeviceList(self))
               .fail(function(error) {
                  notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacityType", {expectedKeywordAccess : self.expectedKeywordAccess, expectedKeywordType : self.expectedKeywordType}), error);
               });
         }
         break;
      case "name":
      
         //we look for a capacity name
         if (Array.isArray(self.expectedCapacity)) {
            
            var arrayOfDeffered = [];
            var tabDevice = [];
            
            //we have a list of capacities
            //we async ask for keyword list of the device for each capacity
            $.each(self.expectedCapacity, function (index, capacity) {
               
               var promise = new $.Deferred();
               arrayOfDeffered.push(promise);
               
               RestEngine.getJson("/rest/device/matchcapacity/" + self.expectedKeywordAccess + "/" + capacity)
                  .done(returnDevices(self, tabDevice, promise))
                  .fail(function(error) {
                     notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedKeywordAccess : self.expectedKeywordAccess, expectedCapacity : self.expectedCapacity}), error);
                  });
            });
            $.whenAll(arrayOfDeffered).done(function () {
               addDeviceList(self, tabDevice);
            });
         }
         else {
            //we have only one capacity
            RestEngine.getJson("/rest/device/matchcapacity/" + self.expectedKeywordAccess + "/" + self.expectedCapacity)
               .done(populateDeviceList(self))
               .fail(function(error) {
                  notifyError($.t("modals.configure-widget.errorDuringGettingDeviceListMatchCapacity", {expectedKeywordAccess : self.expectedKeywordAccess, expectedCapacity : self.expectedCapacity}), error);
               });
         }
         break;
      default:
      case "all":
      
         //we get all devices
         RestEngine.getJson("/rest/device/")
            .done(populateDeviceList(self))
            .fail(function(error) {
               notifyError($.t("modals.configure-widget.errorDuringGettingDeviceList"), error);
            });
         break;
   }
};

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
KeywordParameterHandler.prototype.getDOMObject = function () {
   var input = "<div id=\"" + this.uuidContainer + "\"><select " +
                        "class=\"form-control enable-validation\" " +
                        "id=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\" " +
                        "required ";//
   var self = this;

   input += " >";
   input += "<option value=\"\"></option>" +
            "</select>";

   input += "<select class=\"form-control enable-validation\" id=\"" + self.uuidKeywordList + "\" required ></select>";
   
   input += "<div class=\"device-details\">" +
              "" +
            "</div></div>";
   return ConfigurationHelper.createControlGroup(self, input);
};

KeywordParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuidContainer);
}

/**
 * Get the param name
 * @returns {string}
 */
KeywordParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
KeywordParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuidContainer + " select").addClass("enable-validation");
    } else {
            $("#" + self.uuidContainer + " select").removeClass("enable-validation");
    }
}

/**
 * Get the current configuration in the form
 * @returns {string}
 */
KeywordParameterHandler.prototype.getCurrentConfiguration = function () {
   var value = {};
   value.deviceId = $("select#" + this.uuid).val();
   value.keywordId = $("select#" + this.uuidKeywordList).val();

   var d = new $.Deferred();
   d.resolve(this.value);
   return d.promise();

};