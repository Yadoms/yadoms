
/**
 * Define the python generation function for yadoms_at block
 * @param block The block
 * @return {*[]}
 */
Blockly.Python["yadoms_at"] = function (block) {
    var textYear = block.getFieldValue(block.yearFieldName);
    var textMonth = block.getFieldValue(block.monthFieldName);
    var textDay = block.getFieldValue(block.dayFieldName);
    var textHour = block.getFieldValue(block.hourFieldName);
    var textMinute = block.getFieldValue(block.minuteFieldName);
    var textSecond = block.getFieldValue(block.secondFieldName);
    
    var code = "yApi.wait(\"" + textYear + "-" + textMonth + "-" + textDay + " " + textHour + ":" + textMinute + ":" + textSecond + "\")\n";
    return code;
};

