
Blockly.Yadoms.EnumerationsHelper = {

    allBlocks: [],

    /**
     * Create an enuemration block
     * @param {Object} enumItem The enumeration data (type, name, translated values,...)
     * @returns {} 
     * @private 
     */
    createBlock_: function (enumItem) {

        console.log("Creating Enum block : " + enumItem.name);
        console.log("Enum data : " + enumItem);

        var blockName = enumItem.name;

        Blockly.Yadoms.EnumerationsHelper.allBlocks.push(blockName);

        Blockly.Blocks[blockName] = {
            init: function() {
                this.setColour(Blockly.Yadoms.blockColour.HUE);
                var enumDropDown = new Blockly.FieldDropdown(enumItem.values);

                this.appendDummyInput()
                    .appendField(enumItem.translatedName)
                    .appendField(enumDropDown, "enumField");

                this.setTooltip($.t("blockly.blocks.yadoms_wait_for_event.mutator.base.tooltip"));
                this.contextMenu = false;
                this.setOutput(true, enumItem.typeToSet);


                this.getSelectedEnumValue = function() {
                    return enumDropDown.getValue();
                }
            }
        };

        Blockly.Python[blockName] = function (block) {
            var code = Blockly.Python.quote_(block.getSelectedEnumValue());
            return [code, Blockly.Python.ORDER_ATOMIC];
        };

    },

    /**
     * Create enumeration blocks
     * @returns {} 
     */
    createBlocks : function() {
        $.each(Blockly.Yadoms.data.enumerations, function (index, enumItem) {
            Blockly.Yadoms.EnumerationsHelper.createBlock_(enumItem);
        });
    }

}



