/**
 * Define a block which targets a specific week day
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_date_weekDay'] = {
  /**
   * The dropdown name
   */
  weekDaysDropdownName_ : "weekDaysDropdown",

  /**
   *Block initialization function
   */
  init: function() {

    
    //get values from goog.ui (loaded by Blockly core)
    var symbols =  Blockly.Yadoms.Dates.getDateTimeSymbols();
    var translatedDropdownValues = [];
    $.each(symbols.WEEKDAYS, function(i, weekday) {
        //in goog.i180, we have sunday(0), monday(1), tuesday(2)
        //the aim is to make monday(0) and sunday(6)
        var weekDayNumer = (i - 1) % 7;
        translatedDropdownValues.push([weekday, weekDayNumer.toString()]);
    });

    this.dropdown_ = new Blockly.FieldDropdown(translatedDropdownValues);
    this.dropdown_.setValue(translatedDropdownValues[0][1]);

    this.appendDummyInput()
        .appendField($.t("blockly.blocks.dates.yadoms_date_weekDay.title", { defaultValue: "Every" }))
        .appendField(this.dropdown_, this.weekDaysDropdownName_);
    this.setInputsInline(true);
    this.setOutput(true, "date");
    this.setColour(Blockly.Yadoms.Dates.date.HUE);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_weekDay.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
  },

  /**
   * Obtains the chosen week day
   * @returns {Number} The chosen weekday, from 0 (sunday) to 6 (saturday)
   */
  getWeekDay : function() {
      return this.dropdown_.getValue();
  }
};

