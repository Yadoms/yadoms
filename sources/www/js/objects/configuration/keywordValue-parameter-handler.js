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
   this.content.lookupMethod = "all";
   
   this.keywordPH = new KeywordParameterHandler(i18NContext, i18nKey, paramName, content, currentValue);
}

KeywordValueParameterHandler.prototype.applyScript = function () {

   this.keywordPH.applyScript();
   /*
   $cbKeywords.change(function(handler) {
      return function() {
         debugger;
            //clear value div
            var v = $("select#" + this.uuidKeywordList).val();
            //var configObject = ConfigurationHelper.createKeywordValueParameterHandler("modals.set-value-keyword.", undefined, "keyword-configuration", keyword, value, pluginInstance);

      };
   });
   */
   var self = this;
   
   $("select#" + this.keywordPH.uuidKeywordList).on('change', function (e) {
       
      var selectedKeywordId = this.value;
      
       
      if(self.valuePH && self.valuePH.uuidContainer) {
         $("#" + self.valuePH.uuidContainer).remove();
      }
       
      if(!isNullOrUndefined(selectedKeywordId)) {
        RestEngine.getJson("/rest/device/keyword/" + selectedKeywordId)
        .done(function(keywordData) {
           self.valuePH = ConfigurationHelper.createKeywordValueParameterHandler(self.i18nContext, "value", "value", keywordData);
           if(self.valuePH) {
              $("#" + self.uuidContainer).append(self.valuePH.getDOMObject());
              if ($.isFunction(self.valuePH.applyScript))
               self.valuePH.applyScript();
           }
        })
        .fail(function() {
           //clear value div
        });
      } else {
           //clear value div
      }
      //clear value div
      //var v = $("select#" + self.keywordPH.uuidKeywordList).val();
      //var configObject = ConfigurationHelper.createKeywordValueParameterHandler("modals.set-value-keyword.", undefined, "keyword-configuration", keyword, value, pluginInstance);
  });
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
    /*
    if (enabled) {
            $("#" + self.uuidContainer + " select").addClass("enable-validation");
    } else {
            $("#" + self.uuidContainer + " select").removeClass("enable-validation");
    }*/
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
      self.valuePH.getCurrentConfiguration()
      .done(function(commandValue) {
         var result = keySel;
         result.value = commandValue;
         self.value = result;
         d.resolve(result);
      })
      .fail(d.reject);
   })
   .fail(d.reject);
   return d.promise();
};