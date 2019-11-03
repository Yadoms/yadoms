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
  * Menu Helpers
  */

function constructToolBarPeriodMenuItem(interval, datai18n) {return "<div class=\"widget-toolbar-button range-btn\" interval=" + interval + "><span data-i18n=\"widgets.chart:navigator." + datai18n + "\"/></div>";}
function constructToolBarIntervalMenuItem(prefix, datai18n) {return "<div class=\"widget-toolbar-button range-btn\" prefix=" + prefix + "><span data-i18n=\"widgets.chart:navigator." + datai18n + "\"/></div>";}
function constructExportCommandMenuItem(datai18n, mime) {return "<li><span class=\"export-command\" data-i18n=\"widgets.chart:export." + datai18n + "\" mime-type=" + mime + "></span></li>";}

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
 
 function calculateBeginDate(period, time, prefix, window) {
   return DateTimeFormatter.dateToIsoDate(moment(time).subtract(period.nb * window, period.type).startOf(prefix));
 };
 
 function calculateFinalDate(period, time, prefix, window){
   return DateTimeFormatter.dateToIsoDate(moment(time).subtract(period.nb * (window-1), period.type));
 };
 
 function computePrefixUIForRequest(keywordInformation, prefix){
	var prefixUri = "/" + prefix;
	
	if (isBoolVariable(keywordInformation) || isEnumVariable(keywordInformation) || (prefix == "minute")) {
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
 
function setDateMinAndMax(chart, dateMin, dateMax){
    var datet = DateTimeFormatter.isoDateToDate(dateMin)._d.getTime();
	var datem = DateTimeFormatter.isoDateToDate(dateMax)._d.getTime();
    chart.xAxis[0].setExtremes(datet, datem);
};

function setDateMin(chart, dateMin){
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
            unit = "data.units.liter";
         }
         break;
      case "data.units.wattPerHour":
         if (evaluateArray(values) >2000) {
            coeff = 0.001;
            unit = "data.units.KwattPerHour";
         }      
         break;
      case "data.units.watt":
         if (evaluateArray(values)>2000) {
            coeff = 0.001;
            unit = "data.units.Kwatt";
         }      
         break;
      case "data.units.ampere":
         if (evaluateArray(values)>2000) {
            coeff = 0.001;
            unit = "data.units.Kampere";
         } else if (evaluateArray(values)<1) {
            coeff = 1000;
            unit = "data.units.mampere";
         }
         break;
      case "bit/s":
         if (evaluateArray(values)>2000000000) {
            coeff = 0.000000001;
            unit = "Gb/s";
         }else if (evaluateArray(values)>2000000) {
            coeff = 0.000001;
            unit = "Mb/s";
         }else if (evaluateArray(values)>2000) {
            coeff = 0.001;
            unit = "Kb/s";
         }else{}
         break;
      case "data.units.second":
         console.log(evaluateArray(values));
         if (evaluateArray(values)<0.002){
            coeff = 0.000001;
            unit = "data.units.microsecond";
         }else if (evaluateArray(values)<2){
            coeff = 0.001;
            unit = "data.units.millisecond";
         }else if (evaluateArray(values)>86400){
            coeff = 1/86400;
            unit = "data.units.day";
         }else if (evaluateArray(values)>3600) {
            coeff = 1/3600;
            unit = "data.units.hour";
         }else if (evaluateArray(values)>60) {
            coeff = 1/60;
            unit = "data.units.minute";
         }else{}
      default:
         break;
   }
   if (coeff!=1){
      newValues = adaptArray(values, coeff);
      newRange = adaptRange(range, coeff);	   
   }
   callback(newValues, newRange, unit, coeff);
};

createPlotVector = function (data, KeywordInformation, differentialDisplay, lastValue, keywordId) {
   var plot=[];
   var lastDisplayDate = undefined;
   var timeBetweenAcquisition = [];
   var sum = 0;
   var nb = 0;
   $.each(data, function (index,value) { //data comes from acquisition table
       var d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
       var v;
	   
	   if (isNullOrUndefined(value.key))
		   throw $.t("widgets.chart:errorInitialization");
	   
       if (isEnumVariable(KeywordInformation)) {
          v = parseFloat(getKeyByValue(KeywordInformation.typeInfo.values, value.key));
       }else {
          v = parseFloat(value.key);
       }

	   if (!isNullOrUndefined(lastDisplayDate)){
		  sum += (d-lastDisplayDate);
		  nb++;
		  
		  if ((d-lastDisplayDate)> 2*(sum/nb)){
			  plot.push([d, null]);
		  }
		  
		  timeBetweenAcquisition.push(d-lastDisplayDate);
	   }
	   lastDisplayDate = d;

       // The differential display is disabled if the type of the data is enum or boolean
       if (differentialDisplay && !isBoolVariable(KeywordInformation) && !isEnumVariable(KeywordInformation)){
          if (!isNullOrUndefined(lastValue[keywordId]))
             plot.push([d, v-lastValue[keywordId]]);

          lastValue[keywordId] = v;
       }else // standard display
          plot.push([d, v]);
    });
	return plot;
}

createSummaryPlotVector = function (
              data, 
			  periodValueType, 
			  timeBetweenTwoConsecutiveValues, 
			  plotType,
			  plot,
			  range,
			  KeywordInformation, 
			  differentialDisplay, 
			  lastValue,
			  keywordId) {
	 var vMin;
	 var vMax;
	 var lastDate;
	 var d = undefined;

	 $.each(data, function (index2, value) {
		 lastDate = d;
		 d = DateTimeFormatter.isoDateToDate(value.date)._d.getTime();
		 var vplot;

		 if (!isNullOrUndefined(value[periodValueType])) { // read the computed desired value (avg/min/max)
			 vplot = parseFloat(value[periodValueType]);
			 vMin = parseFloat(value.min);
			 vMax = parseFloat(value.max);
		 } else {
			throw $.t("widgets.chart:errorInitialization");
		 }

		 //we manage the missing data
		 if ((lastDate != undefined) && (timeBetweenTwoConsecutiveValues != undefined) &&
		 (lastDate + timeBetweenTwoConsecutiveValues < d)) {
			 if (plotType === "arearange")
				 range.push([d, null, null]);

			 plot.push([d, null]);
		 }
		 
		 // The differential display is disabled if the type of the data is enum or boolean
		 if (differentialDisplay && !isBoolVariable(KeywordInformation) && !isEnumVariable(KeywordInformation)){  
			 if (periodValueType =="avg") {
			   if (!isNullOrUndefined(lastValue[keywordId]))
				 plot.push([d, vplot-lastValue[keywordId]]);
			   
			   lastValue[keywordId] = vplot;
			}
			else if (periodValueType =="max") { // In this case, we display vMax-vMin
			   plot.push([d, vMax-vMin]);
			   lastValue = vMax;
			}
		 }
		 else{
			if (plotType === "arearange")
				range.push([d, vMin, vMax]);

			plot.push([d, vplot]);                                                   
		 }
	 });
	 
	 // Add here a null plot to separate points (no connections) if there are a missing point between the last one and the future one (a future acquisition)
	 if (!isNullOrUndefinedOrEmpty(data) && data.length>0){
		d = DateTimeFormatter.isoDateToDate(data[data.length-1].date)._d.getTime();
		var time = moment(self.serverTime).startOf(self.prefix)._d.getTime().valueOf();
		var registerDate = moment(self.serverTime).startOf(self.prefix).subtract(1, self.prefix + 's')._d.getTime().valueOf();
		
		if ((time - d) > self.summaryTimeBetweenNewPoint){
			if (plotType === "arearange")
				 range.push([registerDate, null, null]);

			plot.push([registerDate, null]);                                             
		}
	 }
};

createLegendText = function (configuration, deviceName, keywordName){
   var legendText = "";
   try{
      if (configuration ==="Device")
         legendText = deviceName;
      else if (configuration ==="Keyword")
         legendText = keywordName;                                       
      else
         legendText = deviceName + "/" + keywordName;

      return legendText;
   }catch(error){
	  throw error;
   }	
};