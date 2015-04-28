widgetViewModelCtor =

/**
 * Create a MoonPhases ViewModel
 * @constructor
 */
function MoonPhasesViewModel() {

   //observable data
   this.data = ko.observable("");
   this.WidgetHeight = ko.observable ("100px");
   this.WidgetWidth  = ko.observable ("100px");
   
   //Default value - This value is overwrite after
   this.photoName = ko.observable("widgets/moon-phases/images/moon01.png");

   //self.unit = "%";

   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) {
      var self = this;
	  
      if (device == self.widget.configuration.device) 
      {
		 var obj = jQuery.parseJSON( data.value );
		 self.data ( parseInt( obj.IlluminatedMoon ) + "%" );
		 var res = obj.DayOfMoon;	
		
         //Hours are used to calculate the image number
         self.photoName ( "url(widgets/moon-phases/images/moon" + (parseInt(res)-1) + ".png)" );
      }
   }
   
   this.configurationChanged = function() 
   {
      var self = this;
   };

   this.resized = function() 
   {
       var self = this;
	   
	   //The size is x2 only when the widget is square
	   if (this.widget.height() <= 250 && this.widget.height() >= 150 && this.widget.width() <= 250 & this.widget.width() >= 150)
	   {
	      self.WidgetHeight ("200px");
		  self.WidgetWidth  ("200px");
	   }
	   else
	   {
	      self.WidgetHeight ("100px");
		  self.WidgetWidth  ("100px");	   
	   }
   };	   
  
   this.getDevicesForAcquisitions = function() {
      var result = [];

      //Add the moon keyword
      result.push(this.widget.configuration.device);
	  
	  return result;
   }
};