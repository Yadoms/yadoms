/**
 * Create a device parameter handler
 * @param i18NContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function DeviceParameterHandler(i18NContext, i18nKey, paramName, content, currentValue) {
   assert(i18NContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   if (content) {
      if (!isNullOrUndefined(currentValue)) {
         this.value = currentValue;
      } else {
         this.value = {};
      }

      this.name = content.name;
      this.uuidContainer = createUUID();
      this.uuid = createUUID();
      this.paramName = paramName;
      this.description = isNullOrUndefined(content.description) ? "" : content.description;
      this.i18nContext = i18NContext;
      this.i18nKey = i18nKey || paramName;
      this.content = content;
   }
}

function getDeviceMatchingCriteria(self) {
   var requestData = {};

   if (!isNullOrUndefined(self.content.expectedName))
      requestData["expectedName"] = Array.isArray(self.content.expectedName) ? self.content.expectedName : [self.content.expectedName];

   return RestEngine.postJson("/rest/device/matchdevicecriteria/", {
      data: JSON.stringify(requestData)
   });
}

/**
 * Asynchronous populate the list
 * @param handler
 * @returns {Function}
 */
function populateDeviceList(handler, tabDevice) {

   var $deviceList = $("select#" + handler.uuid);

   //A device matches criteria
   if (tabDevice.length !== 0) {
      var itemToSelect = -1;
      var listDevice = sortListItemsWithFriendlyName(tabDevice);

      $.each(listDevice, function (index, value) {
         if (parseBool(value.blacklist) === false) {
            $deviceList.append("<option value=\"" + value.id + "\">" + value.friendlyName + "</option>");
            //if the new element is those that we are looking for we save the position in the list
            if (value.id == handler.value.deviceId)
               itemToSelect = $deviceList.find("option").length - 1;
         }
      });

      //we set the last selected device if it exist anymore
      if (itemToSelect !== -1)
         $deviceList.prop('selectedIndex', itemToSelect);

      $deviceList.change();
   }
}

/**
 * Apply script after DOM object has been added to the page
 */
DeviceParameterHandler.prototype.applyScript = function () {

   //we listen for the changed value to get all keywords that match to the capacity name or the capacity type
   var $deviceList = $("select#" + this.uuid);

   var self = this;

   $deviceList.on('change', function (handler) {
      $deviceList.prop('disabled', true);
      $deviceList.prop('disabled', false);
   });

   getDeviceMatchingCriteria(self)
      .done(function (data) {
         var tabDevice = [];
         $.each(data.devices, function (index, value) {
            tabDevice.push(value);
         });
         self.deviceList = data.devices;

         populateDeviceList(self, tabDevice);
      })
      .fail(function (error) {
         notifyError($.t("modals.configure-widget.errorDuringGettingDeviceList"), error);
      });
};

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
DeviceParameterHandler.prototype.getDOMObject = function () {
   var input = "<div id=\"" + this.uuidContainer + "\"><select " +
      "class=\"form-control enable-validation\" " +
      "id=\"" + this.uuid + "\" " +
      "data-content=\"" + this.description + "\" " +
      "required "; //
   var self = this;

   input += " >";
   input += "<option value=\"\"></option>" +
      "</select>";

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
DeviceParameterHandler.prototype.getParamName = function () {
   return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
DeviceParameterHandler.prototype.setEnabled = function (enabled) {
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
DeviceParameterHandler.prototype.getCurrentConfiguration = function () {
   var self = this;
   this.value = {};
   this.value.deviceId = parseInt($("select#" + this.uuid).val());
   var d = new $.Deferred();
   DeviceManager.getKeywordsByDeviceId(this.value.deviceId)
      .done(function (keywords) {
         self.value.keywords = keywords;
         d.resolve(self.value);
      })
      .fail(d.reject);

   return d.promise();
};