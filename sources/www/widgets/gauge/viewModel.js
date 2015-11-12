widgetViewModelCtor =

/**
 * Create a Gauge ViewModel
 * @constructor
 */
function GaugeViewModel() 
{  
   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");
   
   //simple data
   this.WidgetHeight = "100px";
   this.WidgetWidth  = "200px";
   
   var isSmall = true;
   
   this.stopsArray = null;
   
   /**
    * Widget identifier
    */
   this.widget = null;   
   
   this.gaugeOptions = {

        chart: {
            type: 'solidgauge'
        },

        title: null,
        pane: {
            center: ['50%', '85%'],
            size: '140%',
            startAngle: -90,
            endAngle: 90,
            background: {
                backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                innerRadius: '60%',
                outerRadius: '100%',
                shape: 'arc'
            }
        },

        tooltip: {
            enabled: false
        },
		credits: {
			enabled: false
		},		

        // the value axis
        yAxis: {
//            stops are not defined here
            lineWidth: 0,
            minorTickInterval: null,
            tickPixelInterval: 400,
            tickWidth: 0,
            title: {
                y: -70
            },
            labels: {
				enabled: true,
				align : "left",
				x: -5,
                y: 16
            }
        },

        plotOptions: {
            solidgauge: {
                dataLabels: {
					enabled : true,
                    y: 5,
                    borderWidth: 0,
                    useHTML: true
                }
            }
        }
    };   
   
   /**
    * Initialization method
    * @param widget widget class object
    */
	
   this.initialize = function(widget) 
   {
      this.widget = widget;
	  
	  var elementID = "widget-gauge-" + this.widget.id; // Unique ID
	  
	  // Initialisation of a unique div associated to this widget
	   	$('<div style="width: 200px; height: 100px; float: left"></div>').attr({
		id: elementID
	    }).appendTo( "#widget-" + this.widget.id );
		
		//This div is for rounds borders.
	   	$('<div style="width: 200px; height: 5px; float: left"></div>').appendTo( "#widget-" + this.widget.id );
  };
   
   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) 
   {
      var self = this;	  
	  
      if (device == self.widget.configuration.device) 
	  {
		  if ( data.value != self.data() ) // refresh only when it's necessary.
		  {
			//it is the good device
			self.data ( data.value );

			self.refreshValue ();
			
			  //we get the unit of the keyword
			  KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) 
			  {
				 self.unit($.t(keyword.units));	  
			   });
		  }
      }
   };
   
   this.refreshValue = function()
   {
	    var self = this;
		
		var elementID = "widget-gauge-" + self.widget.id; // Unique ID
		 
		chart = $('#' + elementID).highcharts();
		
		if (chart) 
		{
			console.log ( chart );
			
			point = chart.series[0].points[0];
			point.update( self.data() );
		} 	   
   };
   
   this.configurationChanged = function() 
   {
      var self = this;
	  
	 if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
		return;
	   
	   // Delete all elements in stopArray
      this.stopsArray = new Array();
      
      switch (self.widget.configuration.displayMode.activeSection)
      {
         case "solidColor":
            self.stopsArray.push ( [ 0 , self.widget.configuration.displayMode.content.solidColor.content.color ] );
            break;
         case "thresholds":
            var previousColor = self.widget.configuration.displayMode.content.thresholds.content.firstColor;
            self.widget.configuration.displayMode.content.thresholds.content.addedThresholds.forEach(function(addedThreshold, index)
            {
               var thresholdRatio = (addedThreshold.content.value - self.widget.configuration.customYAxisMinMax.content.minimumValue) /
                  (self.widget.configuration.customYAxisMinMax.content.maximumValue - self.widget.configuration.customYAxisMinMax.content.minimumValue);

               self.stopsArray.push ( [ thresholdRatio - 0.001 , previousColor ] );
               self.stopsArray.push ( [ thresholdRatio, addedThreshold.content.color ] );
               
               previousColor = addedThreshold.content.color;
            })
            break;
         case "gradient":
            self.stopsArray.push ( [ 0, self.widget.configuration.displayMode.content.gradient.content.minColor ] );
            self.stopsArray.push ( [ 1, self.widget.configuration.displayMode.content.gradient.content.maxColor ] );
            break;
      }
	   
	   console.log ( self.stopsArray );
	   
	  //we get the unit of the keyword
      KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) 
	  {
         self.unit($.t(keyword.units));
		 
		 //The following function should be here, otherwise, the unit, is not up to date
		 self.refresh();
		 self.refreshValue ();
	   });
   };

   this.resized = function() 
   {
	   // The square size is not supported by the widget. So size only used here 1x2 and 2x3.
	   
       var self = this;
	   
	   //The maximum size
	   if (this.widget.height() <= 220 && this.widget.height() >= 180 && this.widget.width() <= 320 && this.widget.width() >= 280 )
	   {
		   self.WidgetWidth  = "277px";
		   self.WidgetHeight = "150px";
		   
		   //To be painted only one time
		   if ( isSmall == true )
		   {
			   isSmall = false;
			   self.refresh();
			   self.refreshValue ();
		   }
	   }
	   else
	   {
		   self.WidgetWidth  = "193px";
		   self.WidgetHeight = "100px";
		   
		   //To be painted only one time
		   if ( isSmall == false )
		   {
			   isSmall = true;
			   self.refresh();
			   self.refreshValue ();
		   }
	   }
   };	   
  
   this.refresh = function()
   {
	   self = this;
	   
	   var SizeValue = 12;
	   var SizeUnit = 8;
	   
	   if ( isSmall == true )
	   {
	      SizeValue = 12;
		  SizeUnit  = 8;
		  TitlePosition = -33;
	   }
	   else
	   {
	      SizeValue = 24;
		  SizeUnit  = 12;
          TitlePosition = -70;
	   }
	   
	   var elementID = "widget-gauge-" + self.widget.id; // Unique ID
	   
	   //Attributes of div could only be changed trough theses variables. In an other way the div is stretched.	   
	   document.getElementById(elementID).style.height = self.WidgetHeight;
	   document.getElementById(elementID).style.width  = self.WidgetWidth;	   
	   
	   //we configured the chart !
		$('#' + elementID).highcharts(Highcharts.merge(self.gaugeOptions, {
			navigation: {
				buttonOptions: {
					enabled: !isSmall
				}
			},			
			yAxis: {
				min: parseInt(self.widget.configuration.customYAxisMinMax.content.minimumValue), //Minimum value
				max: parseInt(self.widget.configuration.customYAxisMinMax.content.maximumValue), //Maximal value
				stops: self.stopsArray, // Stops for bar colors with thresholds
				title: {
					enabled : "middle",
					text: self.widget.configuration.text,
					y: TitlePosition
				}
			},

			series: [{
				name: 'Data',
				data: [1],
				dataLabels: {
					format: '<div style="text-align:center"><span style="text-align:center;font-size:'+ SizeValue +'px;color:' + 
						((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span>' +  //<br/>
						   '<span style="text-align:center;font-size:' + SizeUnit + 'px;color:silver"> ' + self.unit() + '</span></div>'
						   },
				    tooltip: {
					valueSuffix: self.unit()
				}
			}],
		}));
   };
  
   this.getDevicesForAcquisitions = function() 
   {
      var result = [];

      //Add the keyword
      result.push(this.widget.configuration.device);
	  
	  return result;
   }   
};