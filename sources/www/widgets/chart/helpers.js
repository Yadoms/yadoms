 /**
  * Simple Helpers
  */
 
function isOdd(num) {return num % 2;}
 
function isBoolVariable(keywordInfo) {
   if ((keywordInfo) && (keywordInfo.dataType === "Bool"))
      return true;
   else
      return false;
};
 
function isEnumVariable (keywordInfo) {
   if ((keywordInfo) && (keywordInfo.dataType === "Enum"))
      return true;
   else
      return false;
};

function roundNumber(num, scale) {
  if(!("" + num).includes("e")) {
    return +(Math.round(num + "e+" + scale)  + "e-" + scale);
  } else {
    var arr = ("" + num).split("e");
    var sig = ""
    if(+arr[1] + scale > 0) {
      sig = "+";
    }
    return +(Math.round(+arr[0] + "e" + sig + (+arr[1] + scale)) + "e-" + scale);
  }
}

 /**
  * transform old configuration to new interval/prefix configuration
  */
 
 function compatibilityManagement (intervalConfiguration) {
  var returnValue;
  switch (intervalConfiguration) {
      case "HOUR": returnValue = "HOUR/minute"; break;
      case "DAY": returnValue = "DAY/hour"; break;
      case "WEEK": returnValue = "WEEK/hour"; break;
      case "MONTH": returnValue = "MONTH/day"; break;
      case "HALF_YEAR": returnValue = "HALF_YEAR/day"; break;
      case "YEAR": returnValue = "YEAR/day"; break;
      case "FIVE_YEAR": returnValue = "FIVE_YEAR/day"; break; //This one doesn't exist
      default: returnValue = intervalConfiguration; break;          
  }
  return returnValue;
 };
 
 function defaultPrefixForInterval(interval) {
   var prefix;
   switch (interval){
      default:
      case "HOUR":
      case "DAY":
         prefix = "minute";
         break;
      case "WEEK":
      case "MONTH":
         prefix = "hour";
         break;
      case "HALF_YEAR":
      case "YEAR":
      case "FIVE_YEAR":
         prefix = "day";
         break;
   }
   return prefix;
 };
 
 function calculateBeginDate(interval, time, prefix) {
  return DateTimeFormatter.dateToIsoDate(moment(time).subtract(self.displayDefinition[interval].nb, self.displayDefinition[interval].type).startOf(prefix));
 };
 
 function calculateFinalDate(){
   //if (isBoolVariable(self.chart.keyword[keywordId]) || isEnumVariable(self.chart.keyword[keywordId])) {
   //}
   return DateTimeFormatter.dateToIsoDate(moment(self.serverTime).startOf(self.prefix).subtract(1, 'seconds'));
 };
 
 function computePrefixUIForRequest(keywordInformation, prefix){
	var prefixUri = "/" + prefix;
	
	if (isBoolVariable(keywordInformation) || isEnumVariable(keywordInformation)) {
		prefixUri = "";
	}else if (prefix == "minute"){
		prefixUri = "";
	}else if (prefix === "week") {
        // the prefix week doesn't exist at the server side we have to to it manually
	    // we use the day prefix		
		prefixUri = "/day";
	};
	return prefixUri;
 };

  /**
  * periods Helpers
  */   
  
function getWeeks(vectorToParse){
    var weekplot = [];
    if (vectorToParse.length == 0)
       return weekplot;
    
    $.each(vectorToParse, function (index, data) {
       weekNum = DateTimeFormatter.isoDateToDate(data.date).week();
       try{
          if (weekNum == weekplot[weekplot.length-1].week){
             weekplot[weekplot.length-1].avg += parseFloat(data.avg);
             
             // treat min and max values
             if (parseFloat(data.min)<parseFloat(weekplot[weekplot.length-1].min)) weekplot[weekplot.length-1].min = data.min;
             if (parseFloat(data.max)>parseFloat(weekplot[weekplot.length-1].max)) weekplot[weekplot.length-1].max = data.max;
             
             weekplot[weekplot.length-1].day += 1;
          }
          else 
             throw "";
          
       } catch(error){
          //Enter the first value
          weekplot.push({
             avg  : parseFloat(data.avg),
             date : DateTimeFormatter.isoDateToDate(data.date).startOf('week'),
             min  : data.min,
             max  : data.max,
             day  : 1,
             week : weekNum
          });
       }
    });
    
    // calcul all average // remove weeks with number of days lower than 7 days
    var index = 0;
    while (index < weekplot.length) {
       if (weekplot[index].day < 7){ 
          weekplot.splice(index, 1);
       }else {
          weekplot[index].avg = (weekplot[index].avg / weekplot[index].day).toString();
          index += 1;
       }
    };
    
    return weekplot;
};  
 
  /**
  * Highcharts Helpers
  */  
 
function changexAxisBound(chart, dateMin){
    var datet = DateTimeFormatter.isoDateToDate(dateMin)._d.getTime();
    chart.xAxis[0].setExtremes(datet, null);
};

function createAxis (index,         // index of the plot
                     chart,         // the chart
                     seriesUuid,    // Uuid array
                     configuration, // configuration of the widget
                     precision,     // display precision for this value
                     units,         // unit of this axis
                     device) {      // device associated to the keyword

  var colorAxis = "#606060"; // default color
  var yAxisName;
  
  // treat oneAxis configuration option => axis name and color
  if (parseBool(configuration.oneAxis.checkbox)) {
     yAxisName = 'axis0';
  }
  else {
     yAxisName = 'axis' + seriesUuid[index];
     colorAxis = device.content.color;
  }
  
  //create axis if needed
  if (isNullOrUndefined(chart.get(yAxisName))) {
      try {
          function getAxisTitle(){
             // create the structure
             var response= {
                text: null,
                style:{
                   color: colorAxis
                }
             };
             return response;
          }

          var align = 'right';
          if (isOdd(index))
              align = 'left';

          var unit = $.t(units);

          chart.addAxis({
              // new axis
              id: yAxisName, //The same id as the serie with axis at the beginning
              reversedStacks: false,
              title: getAxisTitle(),
              labels: {
                  align: align,
                  enabled: !isBoolVariable(chart.keyword[index]),
                  style: {
                      color: colorAxis
                  },
                  formatter: function () {
                     if (isEnumVariable(this.chart.keyword[index])) {  // Return the translated enum value
                        return this.chart.keyword[index].typeInfo.translatedValues[this.value];
                     }
                     else
                       return roundNumber(this.value, precision) + " " + unit;
                  }
              },
              opposite: isOdd(index)
          }, false, false, false);

      } catch (error) {
          console.log('Fail to create axis for keyword = ' + index + ' : ' + error);
      }
  } else {
      console.log('Axis already exists for keyword = ' + index);
  }

  if ((parseBool(configuration.oneAxis.checkbox))) {
      //Configure the min/max in this case
      try {
          var yAxis = chart.get(yAxisName);

          // Avec un seul axe, pas de nom
          yAxis.setTitle({ text: "" });

          if (parseBool(configuration.oneAxis.content.customYAxisMinMax.checkbox)) {
              //we manage min and max scale y axis
              var min = parseFloat(configuration.oneAxis.content.customYAxisMinMax.content.minimumValue);
              var max = parseFloat(configuration.oneAxis.content.customYAxisMinMax.content.maximumValue);
              yAxis.setExtremes(min, max);
          } else {
              //we cancel previous extremes
              yAxis.setExtremes(null, null);
          }
      } catch (error2) {
          console.log(error2);
      }
  }
  return yAxisName; // Return the name of the axis
};

/**
 * Adapt an array of values with coefficient
 * @param arrayToAdapt values to adapt
 * @param coeff the new coefficient
 */
adaptArray = function(arrayToAdapt, coeff) {
   var newArray = [];
   $.each(arrayToAdapt, function (index,value) {
      newArray.push([value[0],parseFloat(value[1])*coeff]);
   });
   return newArray;
};

/**
 * Adapt a range with coefficient
 * @param rangeToAdapt rangeToAdapt
 * @param coeff the new coefficient
 */
adaptRange = function(rangeToAdapt, coeff) {
   var newRange = [];
   $.each(rangeToAdapt, function (index,value) {
      newRange.push([value[0],parseFloat(value[1])*coeff,parseFloat(value[2])*coeff]);
   });
   return newRange;
};

/**
 * Adapt the unit and an array of values to an appropriate unit
 * @param values The Id to find
 * @param baseUnit Unit received from yadoms server
 */
adaptValuesAndUnit = function (values, range, baseUnit, callback) {
   assert(!isNullOrUndefined(values), "value must be defined");
   assert(!isNullOrUndefined(baseUnit), "baseUnit must be defined");
   var unit = baseUnit;
   var newValues = values;
   var newRange = range;
   var coeff = 1;
   
   evaluateArray = function(arrayToEvaluate) {
      var moy = 0;
      var nbre = 0;
      
      $.each(arrayToEvaluate, function (index,value) {
         if (value[1] != null) {
            moy = moy + parseFloat(value[1]);
            nbre = nbre + 1;
         }
      });
      
      if (nbre != 0)
         moy = moy / nbre;
      
      return moy;
   };
   
   switch (baseUnit){
      case "data.units.cubicMetre":
         if (evaluateArray(values) <1) {
            coeff = 1000;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.liter";
         }
         break;
      case "data.units.wattPerHour":
         if (evaluateArray(values) >2000) {
            coeff = 0.001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.KwattPerHour";
         }      
         break;
      case "data.units.watt":
         if (evaluateArray(values)>2000) {
            coeff = 0.001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.Kwatt";
         }      
         break;
      case "data.units.ampere":
         if (evaluateArray(values)>2000) {
            coeff = 0.001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.Kampere";
         } else if (evaluateArray(values)<1) {
            coeff = 1000;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.mampere";
         }
         break;
      case "bit/s":
         if (evaluateArray(values)>2000000000) {
            coeff = 0.000000001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "Gb/s";
         }else if (evaluateArray(values)>2000000) {
            coeff = 0.000001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "Mb/s";
         }else if (evaluateArray(values)>2000) {
            coeff = 0.001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "Kb/s";
         }else{}
         break;
      case "data.units.second":
         console.log(evaluateArray(values));
         if (evaluateArray(values)<0.002){
            coeff = 0.000001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.microsecond";
         }else if (evaluateArray(values)<2){
            coeff = 0.001;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.millisecond";
         }else if (evaluateArray(values)>86400){
            coeff = 1/86400;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.day";
         }else if (evaluateArray(values)>3600) {
            coeff = 1/3600;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.hour";
         }else if (evaluateArray(values)>60) {
            coeff = 1/60;
            newValues = adaptArray(values, coeff);
            newRange = adaptRange(range, coeff);
            unit = "data.units.minute";
         }else{}
      default:
         break;
   }
   callback(newValues, newRange, unit, coeff);
};