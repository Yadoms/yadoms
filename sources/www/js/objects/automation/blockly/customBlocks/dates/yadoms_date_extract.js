/**
 * Define a block which extracts a part of a date/datetime/time
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_date_extract'] = {
  /**
   * The name of value input
   */
  valueInputName_ : "valueInput",	
	
  /**
   * The name of dropdown field
   */
  dropdownName_ : "whatToExtractField",	
	
  init: function() {
	  
	var value = [	
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.date"), "date"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.time"), "time"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.year"), "year"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.month"), "month"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.day"), "day"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.hour"), "hour"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.minute"), "minute"],
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.weekDay"), "weekDay"]
				];
	  
    this.appendDummyInput()
        .appendField($.t("blockly.blocks.dates.yadoms_date_extract.title"))
        .appendField(new Blockly.FieldDropdown(value), this.dropdownName_);
    this.appendValueInput(this.valueInputName_)
        .appendField($.t("blockly.blocks.dates.yadoms_date_extract.from"))
        .setCheck("datetime");
    this.setInputsInline(true);
    this.setOutput(true, ["number", "date", "time", "String"]);
    this.setColour(160);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_extract.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
  }
};
