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
	var self = this;
	
	this.values_ = [	
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.date"), "date"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.time"), "time"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.year"), "year"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.month"), "month"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.day"), "day"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.hour"), "hour"], 
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.minute"), "minute"],
					[$.t("blockly.blocks.dates.yadoms_date_extract.what.weekDay"), "weekDay"]
				];
				
	this.dropdown_ = new Blockly.FieldDropdown(this.values_, function(val) {
		self.updateTypes_(val);
	});
	  
    this.appendDummyInput()
        .appendField($.t("blockly.blocks.dates.yadoms_date_extract.title"))
        .appendField(this.dropdown_, this.dropdownName_);
    this.appendValueInput(this.valueInputName_)
        .appendField($.t("blockly.blocks.dates.yadoms_date_extract.from"))
        .setCheck(["datetime", "date", "time"]);
    this.setInputsInline(true);
    this.setOutput(true, ["number", "date", "time", "String"]);
    this.setColour(Blockly.Yadoms.Dates.datetime.HUE);
    this.setTooltip($.t("blockly.blocks.dates.yadoms_date_extract.tooltip", { defaultValue: "" }));
    this.setHelpUrl('http://www.example.com/');
	console.log("Création");
	
	this.initDropDown_();
  },
  
  initDropDown_ : function() {
	//update check/outputType
	var value = this.values_[0][1];
	this.dropdown_.setValue(value);
	this.updateTypes_(value);
  },
  
  updateTypes_ : function(val) {
	switch (val) {
			case "date":
				this.getInput(this.valueInputName_).setCheck("datetime");
				this.outputConnection.setCheck("date");
				this.setColour(Blockly.Yadoms.Dates.date.HUE);
				break;
				
			case "time":
				this.getInput(this.valueInputName_).setCheck("datetime");
				this.outputConnection.setCheck("time");
				this.setColour(Blockly.Yadoms.Dates.time.HUE);
				break;
				
			case "weekDay":
				this.getInput(this.valueInputName_).setCheck("datetime");
				this.outputConnection.setCheck("Number");
				this.setColour(Blockly.Blocks.math.HUE);
				break;
				
			case "year":
			case "month":
			case "day":
				this.getInput(this.valueInputName_).setCheck(["datetime", "date"]);
				this.outputConnection.setCheck("Number");
				this.setColour(Blockly.Blocks.math.HUE);
				break;
				
			case "hour":
			case "minute":
				this.getInput(this.valueInputName_).setCheck(["datetime", "time"]);
				this.outputConnection.setCheck("Number");
				this.setColour(Blockly.Blocks.math.HUE);
				break;

			default:
				this.getInput(this.valueInputName_).setCheck(["datetime", "date", "time"]);
				this.outputConnection.setCheck(["datetime", "date", "time"]);
				this.setColour(Blockly.Yadoms.Dates.datetime.HUE);
				break;
		}			  
  }
};
