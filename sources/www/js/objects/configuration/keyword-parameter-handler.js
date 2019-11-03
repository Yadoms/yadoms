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
function KeywordParameterHandler(i18NContext, i18nKey, paramName, content, currentValue) {
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
      this.uuidKeywordList = createUUID();
      this.paramName = paramName;
      this.description = isNullOrUndefined(content.description) ? "" : content.description;
      this.i18nContext = i18NContext;
      this.i18nKey = i18nKey || paramName;
      this.content = content;
   }
}

function getDeviceMatchingCriteria(self) {
   var requestData = {};
   if (!isNullOrUndefined(self.content.expectedKeywordType))
      requestData["expectedKeywordType"] = Array.isArray(self.content.expectedKeywordType) ? self.content.expectedKeywordType : [self.content.expectedKeywordType];
   if (!isNullOrUndefined(self.content.expectedCapacity))
      requestData["expectedCapacity"] = Array.isArray(self.content.expectedCapacity) ? self.content.expectedCapacity : [self.content.expectedCapacity];
   if (!isNullOrUndefined(self.content.expectedKeywordAccess))
      requestData["expectedKeywordAccess"] = Array.isArray(self.content.expectedKeywordAccess) ? self.content.expectedKeywordAccess : [self.content.expectedKeywordAccess];
   if (!isNullOrUndefined(self.content.expectedKeywordHistoryDepth))
      requestData["expectedKeywordHistoryDepth"] = Array.isArray(self.content.expectedKeywordHistoryDepth) ? self.content.expectedKeywordHistoryDepth : [self.content.expectedKeywordHistoryDepth];

   return RestEngine.postJson("/rest/device/matchcriteria/", {
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

      //until we have no device selected we have no keyword to select
      var $cbKeywords = $("select#" + handler.uuidKeywordList);
      $cbKeywords.prop('disabled', true);

      //we set the last selected device if it exist anymore
      if (itemToSelect !== -1)
         $deviceList.prop('selectedIndex', itemToSelect);

      $deviceList.change();
   }
}

function onSelectDevice(selectedDeviceId, self, $cbKeywords) {
   if (isNullOrUndefined(self.keywordList) || isNullOrUndefinedOrEmpty(selectedDeviceId)) {
      //the line selected is the empty one
      //we clear and disable the second combo
      $cbKeywords.empty();
      $cbKeywords.prop('disabled', true);
      $cbKeywords.append("<option value=\"\"></option>");
      return;
   }

   //until we have no answer the combos will be disabled
   $cbKeywords.prop('disabled', true);

   var keywords = getKeywordsForDeviceId(self, selectedDeviceId);

   //we append each keywords in the list
   var keywordToSelect = 0;

   keywords = sortListItemsWithFriendlyName(keywords);

   $cbKeywords.empty();
   $cbKeywords.append("<option value=\"\"></option>");

   $.each(keywords, function (index, value) {
      $cbKeywords.append("<option value=\"" + value.id + "\">" + value.friendlyName + "</option>");

      //we restore previously set value only if the deviceId is the same than the last one in the configuration
      if (selectedDeviceId == self.value.deviceId && value.id == self.value.keywordId) {
         keywordToSelect = index + 1; //the +1 is for the empty line
      }
   });

   //we select the last selected or the first one
   $cbKeywords.prop('selectedIndex', keywordToSelect);
   $cbKeywords.trigger('change');

   //we re-enable the combo
   $cbKeywords.prop('disabled', false);
}

function getKeywordsForDeviceId(self, deviceId) {
   filteredKeywords = [];
   $.each(self.keywordList, function (index, value) {
      if (value.deviceId == deviceId)
         filteredKeywords.push(value);
   });
   return filteredKeywords;
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

   $deviceList.on('change', function (handler) {
      $deviceList.prop('disabled', true);
      onSelectDevice(this.value, self, $cbKeywords);
      $deviceList.prop('disabled', false);
   });

   getDeviceMatchingCriteria(self)
      .done(function (data) {
         var tabDevice = [];
         $.each(data.devices, function (index, value) {
            tabDevice.push(value);
         });
         self.keywordList = data.keywords;

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
KeywordParameterHandler.prototype.getDOMObject = function () {
   var input = "<div id=\"" + this.uuidContainer + "\"><select " +
      "class=\"form-control enable-validation\" " +
      "id=\"" + this.uuid + "\" " +
      "data-content=\"" + this.description + "\" " +
      "required "; //
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
KeywordParameterHandler.prototype.getParamName = function () {
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
   this.value = {};
   this.value.deviceId = parseInt($("select#" + this.uuid).val());
   this.value.keywordId = parseInt($("select#" + this.uuidKeywordList).val());

   var d = new $.Deferred();
   d.resolve(this.value);
   return d.promise();

};