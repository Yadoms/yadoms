/**
 * Created by nicolasHILAIRE on 09/02/2016.
 */

/**
 * Creates an instance of WidgetApi
 * @constructor
 */
function WidgetApi(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   this.widget = widget;
   this.toolbar = new ToolbarApi(widget);
}

/**
 * Jquery find method into widget instance
 */
WidgetApi.prototype.find = function (pattern) {
   return this.widget.$gridWidget.find(pattern);
}

/**
 * Obtain information about a keyword
 * @param {} keywordId to query
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.prototype.getKeywordInformation = function (keywordId) {
   assert(!isNullOrUndefined(keywordId), "keywordId must be defined");
   return KeywordManager.get(keywordId);
};

/**
 * Register keywords to receive notifications when a new acquisition triggers
 * @param {} keywordIds to register (can be a single value or an array of values)
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.prototype.registerKeywordAcquisitions = function (keywordIds) {
   assert(!isNullOrUndefined(keywordIds), "keywordIds must be defined");

   var self = this;

   if (!self.widget.listenedKeywords)
      self.widget.listenedKeywords = [];

   if (Array.isArray(keywordIds)) {
      $.each(keywordIds, function (index, value) {
         self.widget.listenedKeywords.push(value);
      });
   } else {
      self.widget.listenedKeywords.push(keywordIds);
   }
};

/**
 * Lazy load required libraries
 * @param {} librayNames to load (can be a single value or an array of values)
 * @returns {} a promise that's return done when libraries are loaded
 */
WidgetApi.prototype.loadLibrary = function (librayNames) {
   assert(!isNullOrUndefined(librayNames), "librayNames must be defined");

   if (Array.isArray(librayNames)) {
      return asyncLoadJSLibs(librayNames);
   } else {
      return asyncLoadJSLib(librayNames);
   }
};

/**
 * Lazy load required CSS stylesheet
 * @param {} librayNames to load (can be a single value or an array of values)
 * @returns {} a promise that's return done when libraries are loaded
 */
WidgetApi.prototype.loadCss = function (cssFiles) {
   assert(!isNullOrUndefined(cssFiles), "cssFiles must be defined");

   if (Array.isArray(cssFiles)) {
      return asyncLoadManyCss(cssFiles);
   } else {
      return asyncLoadCss(cssFiles);
   }
};