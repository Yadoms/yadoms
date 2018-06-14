/**
 * Created by jean michel BERHAULT on 07/06/2018.
 */

/**
 * Adapt the unit and the value to an appropriate unit
 * @param value The Id to find
 * @param baseUnit Unit received from yadoms server
 */
adaptValueAndUnit = function (value, baseUnit, callback) {
   assert(!isNullOrUndefined(value), "value must be defined");
   assert(!isNullOrUndefined(baseUnit), "baseUnit must be defined");
   var unit = baseUnit;
   var newValue = value;
   
   switch (baseUnit){
      case "data.units.cubicMetre":
         if (Math.abs(value) <1) {
            newValue = parseFloat(value)*1000;
            unit = "data.units.liter";
         }
         break;
      case "data.units.wattPerHour":
         if (Math.abs(value)>2000) {
            newValue = parseFloat(value)/1000;
            unit = "data.units.KwattPerHour";
         }      
         break;
      case "data.units.watt":
         if (Math.abs(value)>2000) {
            newValue = parseFloat(value)/1000;
            unit = "data.units.Kwatt";
         }      
         break;
      case "data.units.ampere":
         if (Math.abs(value)>2000) {
            newValue = parseFloat(value)/1000;
            unit = "data.units.Kampere";
         } else if (Math.abs(value)<1) {
            newValue = parseFloat(value)*1000;
            unit = "data.units.mampere";
         }
         break;
      case "bit/s":
         if (Math.abs(value)>2000000000) {
            newValue = parseFloat(value)/1000000000;
            unit = "Gb/s";
         }else if (Math.abs(value)>2000000) {
            newValue = parseFloat(value)/1000000;
            unit = "Mb/s";            
         }else if (Math.abs(value)>2000) {
            newValue = parseFloat(value)/1000;
            unit = "Kb/s";            
         }
         break;
      default:
         break;
   }
   
   callback(newValue, unit);
};