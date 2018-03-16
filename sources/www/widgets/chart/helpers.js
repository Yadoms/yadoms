 /**
  * Simple Helpers
  */       
 
 function isOdd(num) {return num % 2;}
 
 function isBoolVariable(keywordInfo) {
     if ((keywordInfo) && (keywordInfo.type === "Bool"))
        return true;
     else
        return false;
 };
 
 function isEnumVariable (keywordInfo) {
     if ((keywordInfo) && (keywordInfo.type === "Enum"))
        return true;
     else
        return false;
 };
 
 function calculateBeginDate(interval, time, prefix) {
  var dateValue;
  /*console.log ("c interval : ", interval);
  console.log ("c time : ", time);
  console.log ("c prefix : ", prefix);*/
  switch (interval) {
      case "HOUR":
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'hours').startOf(prefix));
          break;
      default:
      case "DAY": //we request hour summary data
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'days').startOf(prefix));
          break;
      case "WEEK": //we request hour summary data
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'weeks').startOf(prefix));
          break;
      case "MONTH": //we request day summary data
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'months').startOf(prefix));
          break;
      case "HALF_YEAR": //we request day summary data
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(6, 'months').startOf(prefix));
          break;
      case "YEAR": //we request day summary data
          dateValue = DateTimeFormatter.dateToIsoDate(moment(time).subtract(1, 'years').startOf(prefix));
          break;
  }
  return dateValue;
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
          
          if (weekNum == weekplot[weekplot.length-1].week)
          {
             weekplot[weekplot.length-1].avg += data.avg;
             
             // treat min and max values
             if (data.min<weekplot[weekplot.length-1].min) weekplot[weekplot.length-1].min = data.min;
             if (data.max>weekplot[weekplot.length-1].max) weekplot[weekplot.length-1].max = data.max;
             
             weekplot[weekplot.length-1].day += 1;
          }
          else 
             throw "";
          
       } catch(error){
          //Enter the first value
          weekplot.push({
             avg  : data.avg,
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
          weekplot[index].avg = weekplot[index].avg / weekplot[index].day;
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