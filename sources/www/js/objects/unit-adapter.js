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
      default:
         break;
   }
   
   callback(newValue, unit);
};

/**
 * Adapt the unit and an array of values to an appropriate unit
 * @param values The Id to find
 * @param baseUnit Unit received from yadoms server
 */
adaptValuesAndUnit = function (values, baseUnit) {
   assert(!isNullOrUndefined(value), "value must be defined");
   assert(!isNullOrUndefined(baseUnit), "baseUnit must be defined");
   var unit = baseUnit;
   
   evaluateArray = function(arrayToEvaluate) {
      var moy = 0;
      
      $.each(arrayToEvaluate, function (index,value) {
         moy = moy + value;
      });
      
      if (arrayToEvaluate.length != 0)
         moy = moy / arrayToEvaluate.length;
      
      return moy;
   };
   
   adaptArray = function(arrayToEvaluate, coeff) {
      var newArray = [];
      $.each(arrayToEvaluate, function (index,value) {
         newArray.push(value*coeff);
      });
      
      return newArray;
   };   
   
   switch (baseUnit){
      case "data.units.cubicMetre":
         if (evaluateArray(values) <1) {
            values = adaptArray(values, 1000);
            unit = "data.units.liter";
         }
         break;
      case "data.units.wattPerHour":
         if (evaluateArray(values) >2000) {
            values = adaptArray(values, 0.001);
            unit = "data.units.KwattPerHour";
         }      
         break;
      case "data.units.watt":
         if (evaluateArray(values)>2000) {
            values = adaptArray(values, 0.001);
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
      default:
         break;
   }
   
   return unit;
};