

/**
 * Define the yadoms_log block (allow logging into yaomds.log)
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_get_info'] = {

    dropdownValueName_ : "chosenInfo",

    init: function () {
        var self = this;

        this.informationIdList_ = [
            [$.t("blockly.blocks.yadoms_get_info.values.sunrise"), "kSunrise"],
            [$.t("blockly.blocks.yadoms_get_info.values.sunset"), "kSunset"],
            [$.t("blockly.blocks.yadoms_get_info.values.latitude"), "kLatitude"],
            [$.t("blockly.blocks.yadoms_get_info.values.longitude"), "kLongitude"],
            [$.t("blockly.blocks.yadoms_get_info.values.altitude"), "kAltitude"],
            [$.t("blockly.blocks.yadoms_get_info.values.serverOs"), "kYadomsServerOS"],
            [$.t("blockly.blocks.yadoms_get_info.values.serverVersion"), "kYadomsServerVersion"]
        ];

		this.dropDown_ = new Blockly.FieldDropdown(this.informationIdList_, function(val) {
            self.updateTypes_(val);
        });		
		
        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_get_info.title"))
            .appendField(this.dropDown_, this.dropdownValueName_);
        this.setColour(Blockly.Blocks.texts.HUE);
        this.setTooltip($.t("blockly.blocks.yadoms_get_info.tooltip", { defaultValue: "" }));
        this.setHelpUrl('http://www.example.com/');
        this.setOutput(true, "String");

		//set the value to the first available item, to force the setcheck to be done
		this.initDropDown_();
    },
	
    initDropDown_ : function() {
		//update check/outputType
		var value = this.informationIdList_[0][1];
		this.dropDown_.setValue(value);
		this.updateTypes_(value);
	},
	  
	updateTypes_ : function(val) {
		console.log("change type : " + val);
		//var val = self.getFieldValue(self.dropdownValueName_);
		switch (val) {
			case this.informationIdList_[0][1]: //sunrise
			case this.informationIdList_[1][1]: //sunset
				this.outputConnection.setCheck("time");
				this.setColour(Blockly.Yadoms.Dates.time.HUE);
				break;
			case this.informationIdList_[2][1]: //latitude
			case this.informationIdList_[3][1]: //longitude
			case this.informationIdList_[4][1]: //altitude
				this.outputConnection.setCheck("Number");
				this.setColour(Blockly.Blocks.math.HUE);
				break;
			default:
				this.outputConnection.setCheck("String");
				this.setColour(Blockly.Blocks.texts.HUE);
				break;
		}
	}
};

