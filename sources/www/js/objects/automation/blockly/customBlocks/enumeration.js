
Blockly.Yadoms.EnumerationsHelper = {

    /**
     * All created enumeration blocks
     */
    allBlocks: [],

    /**
     * Initialize enumeration blocks
     */
    initialize: function () {
        //clear previously created block (in case of loading multiple times Blockly)
        Blockly.Yadoms.EnumerationsHelper.allBlocks = [];

        $.each(Blockly.Yadoms.data.enumerations, function (index, enumItem) {
            Blockly.Yadoms.EnumerationsHelper.createBlock_(enumItem);
        });
    },


    /**
     * Create an enuemration block
     * @param {Object} enumItem The enumeration data (type, name, translated values,...)
     * @returns {} 
     * @private 
     */
    createBlock_: function (enumItem) {
        var blockName = enumItem.name;

        //psuh the block name
        Blockly.Yadoms.EnumerationsHelper.allBlocks.push(blockName);

        //push the block code
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

        //push python generation
        Blockly.Python[blockName] = function (block) {
            var code = Blockly.Python.quote_(block.getSelectedEnumValue());
            return [code, Blockly.Python.ORDER_ATOMIC];
        };

    }


}



