/**
 * Define a block which extracts a part of a date/datetime/time
 * @param block The block
 * @return {*[]}
 */
Blockly.Blocks['yadoms_date_extract'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Extract ")
        .appendField(new Blockly.FieldDropdown([["date", "date"], ["time", "time"], ["year", "year"], ["month", "month"], ["day", "day"], ["hour", "hour"], ["minute", "minute"]]), "NAME");
    this.appendValueInput("VALUE")
        .appendField("From")
        .setCheck(["datetime", "date", "time"]);
    this.setInputsInline(true);
    this.setOutput(true, ["number", "date", "time"]);
    this.setColour(160);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
