

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

        this.appendDummyInput()
            .appendField($.t("blockly.blocks.yadoms_get_info.title"))
            .appendField(new Blockly.FieldDropdown(this.informationIdList_, function(val) {
                //var val = self.getFieldValue(self.dropdownValueName_);
                switch (val) {
                    case self.informationIdList_[0][1]: //sunrise
                    case self.informationIdList_[1][1]: //sunset
                        self.outputConnection.setCheck("time");
                        self.setColour(160);
                        break;
                    case self.informationIdList_[2][1]: //latitude
                    case self.informationIdList_[3][1]: //longitude
                    case self.informationIdList_[4][1]: //altitude
                        self.outputConnection.setCheck("Number");
                        self.setColour(230);
                        break;
                    default:
                        self.outputConnection.setCheck("String");
                        self.setColour(210);
                        break;
                }
            }), this.dropdownValueName_);
        this.setColour(210);
        this.setTooltip($.t("blockly.blocks.yadoms_get_info.tooltip", { defaultValue: "" }));
        this.setHelpUrl('http://www.example.com/');
        this.setOutput(true, "String");

    },
    onValueChange: function (block) {
        
    }
};

