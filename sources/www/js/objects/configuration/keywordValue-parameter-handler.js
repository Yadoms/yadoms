/**
 * Created by Jean-Michel DECORET on 13/06/17.
 */

/**
 * Create a keywordValue parameter handler
 * @param i18NContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function KeywordValueParameterHandler(i18NContext, i18nKey, paramName, content, currentValue) {
   assert(i18NContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   
   this.name = content.name;
   this.paramName = paramName;
   this.uuidContainer = createUUID();
   this.description = isNullOrUndefined(content.description) ? "" : content.description;
   this.i18nContext = i18NContext;
   this.i18nKey = i18nKey || paramName;
   this.content = content;

   this.content.expectedKeywordAccess = "GetSet";
   this.currentValue = currentValue;
   this.keywordPH = new KeywordParameterHandler(i18NContext, i18nKey, paramName, content, currentValue);
}

KeywordValueParameterHandler.prototype.applyScript = function () {
   var self = this;

   $("select#" + self.keywordPH.uuidKeywordList).on('change', function (e) { 
      self.onKeywordChange(); 
   });
   
   self.keywordPH.applyScript();
};


KeywordValueParameterHandler.prototype.onKeywordChange = function () {
   var self = this;
   var d = new $.Deferred();
   
   var selectedKeywordId = $("select#" + self.keywordPH.uuidKeywordList).val();
   
   if(self.valuePH && self.valuePH.uuid) {
      $("#" + self.valuePH.uuid).parents(".control-group").first().remove();
   }
    
   if(!isNullOrUndefinedOrEmpty(selectedKeywordId)) {
      KeywordManager.get(selectedKeywordId)
      .done(function(keywordData) {
         var curVal;
         if(self.currentValue && self.currentValue.value) {
            curVal = self.currentValue.value.toString();
         }
         
         if(keywordData.type !== "NoData") {
            DeviceManager.get(keywordData.deviceId)
            .done(function(deviceData) {
               DeviceManager.getAttachedPlugin(deviceData)
               .done(function() {
                  PluginInstanceManager.downloadPackage(deviceData.attachedPlugin)
                  .done(function() {
                     self.valuePH = ConfigurationHelper.createKeywordValueParameterHandler(self.i18nContext, undefined, "value", keywordData, curVal, deviceData.attachedPlugin);
                     if(self.valuePH) {
                        $("#" + self.uuidContainer).append(self.valuePH.getDOMObject());
                        if ($.isFunction(self.valuePH.applyScript)) {
                           self.valuePH.applyScript();
                        }
                        $("#" + self.uuidContainer).i18n();
                        d.resolve();
                     } else {
                       d.reject("Cannot create parameter handler for keyword id=" + selectedKeywordId);
                     }                  
                  })
                  .fail(d.reject);
               })
               .fail(d.reject);
            })
            .fail(d.reject);
         } else {
            //if this is a keyword without data (such as events : system/restart)
            //just ensure value parameter handler is null
            self.valuePH = null;
            $("#" + self.uuidContainer).i18n();
            d.resolve();
         }
      })
      .fail(d.reject);
   } else {
      //clear value div
      d.reject("No keyword selected");
   }
   
   return d.promise();
};

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
KeywordValueParameterHandler.prototype.getDOMObject = function () {
   var keywordSelectionDom = this.keywordPH.getDOMObject();
   var input = '<div id="' + this.uuidContainer + '">' + keywordSelectionDom + '</div>';
   return input;
};

KeywordValueParameterHandler.prototype.locateInDOM = function () {
   return $("div#" + this.uuidContainer);
}

/**
 * Get the param name
 * @returns {string}
 */
KeywordValueParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
KeywordValueParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;
    this.keywordPH.setEnabled(enabled);
}

/**
 * Get the current configuration in the form
 * @returns {string}
 */
KeywordValueParameterHandler.prototype.getCurrentConfiguration = function () {
   var d = new $.Deferred();
   var self = this;
   
   this.keywordPH.getCurrentConfiguration()
   .done(function(keySel) {
      if(self.valuePH) {
         self.valuePH.getCurrentConfiguration()
         .done(function(commandValue) {
            var result = keySel;
            result.value = commandValue;
            self.value = result;
            d.resolve(result);
         })
         .fail(d.reject);
      } else {
         var result = keySel;
         result.value = null;
         self.value = result;
         d.resolve(result);
      }
   })
   .fail(d.reject);
   return d.promise();
};