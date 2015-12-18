/**
 * Empty constructor
 * @constructor 
 */
Blockly.Yadoms.Dates = function() {
}

/**
 * Localized dates symbols
 */
Blockly.Yadoms.Dates.DateTimeSymbols = null;

/**
 * Load the appropriate localized dates symbols
 * @returns {} 
 */
Blockly.Yadoms.Dates.loadLanguage_ = function() {
  var reg = /^DateTimeSymbols_(.+)$/;
  for (var prop in goog.i18n) {
    var m = prop.match(reg);
    if (m) {
      var lang = m[1].toLowerCase().replace('_', '.');  // E.g. 'pt.br'
      if (goog.getObjectByName(lang, Blockly.Msg)) {
        Blockly.Yadoms.Dates.DateTimeSymbols = goog.i18n[prop];
      }
    }
  }
};

/**
 * Get the localized dates symbols 
 * @returns {goog.i18n.DateTimeSymbols} 
 */
Blockly.Yadoms.Dates.getDateTimeSymbols = function() {
    if (!Blockly.Yadoms.Dates.DateTimeSymbols)
        Blockly.Yadoms.Dates.loadLanguage_();
    return Blockly.Yadoms.Dates.DateTimeSymbols;
}
