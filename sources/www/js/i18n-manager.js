/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function i18nManager() {}

//we start i18n engine
i18nManager.option = {
  resGetPath: "locales/__lng__.json",
  backend: {
    // load from i18next-gitbook repo
    loadPath: '{{ns}}/locales/{{lng}}.json'
  },
  useDataAttrOptions: true,
  debug: false,
  fallbackNS: "translation",
  load: 'languageOnly',
  getAsync: false,
  useLocalStorage: true,
  localStorageExpirationTime: 86400000 // in ms, default 1 week
};

i18nManager.init = function () {
  var d = new $.Deferred();

  if (isNullOrUndefined(i18nManager.option.fallbackLng))
    i18nManager.option.fallbackLng = navigator.language || navigator.userLanguage || "en";

  i18next
    .use(i18nextXHRBackend)
    .init(i18nManager.option, function () {
      jqueryI18next.init(i18next, $, {
        tName: 't', // --> appends $.t = i18next.t
        i18nName: 'i18n', // --> appends $.i18n = i18next
        handleName: 'i18n', // --> appends $(selector).localize(opts);
        selectorAttr: 'data-i18n', // selector for translating elements
        targetAttr: 'i18n-target', // data-() attribute to grab target element to translate (if diffrent then itself)
        optionsAttr: 'i18n-options', // data-() attribute that contains options, will load/set if useOptionsAttr = true
        useOptionsAttr: false, // see optionsAttr
        parseDefaultValueFromContent: true // parses default values from content ele.val or ele.text
      });
      d.resolve();
    });
  return d.promise();
}

i18nManager.loadNamespace = function (category, object) {
  var d = new $.Deferred();
  i18next.options.backend.loadPath = category + '/' + object + '/locales/{{lng}}.json';
  i18next.loadNamespaces(category + '.' + object, function (err, t) {
    if (err)
      d.reject(err);
    else
      d.resolve();
  });
  return d.promise();
}

i18nManager.getLanguage = function () {
  return i18next.language.substring(0, 2);
}

i18nManager.getFallbackLanguage = function () {
  return i18next.options.fallbackLng[0];
}