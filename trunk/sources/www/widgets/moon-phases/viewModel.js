widgetViewModelCtor =

/**
 * Create a MoonPhases ViewModel
 * @constructor
 */
function MoonPhasesViewModel() {

   //observable data
   this.data = ko.observable("");
   this.MoonPhasesId = ko.observable ("");
   
   //simple data
   this.WidgetHeight = 100;
   this.WidgetWidth  = 100;   
   
   //Default value - This value is overwrite after
   this.photoName = ko.observable("widgets/moon-phases/images/moon01.png");

   /**
    * Widget identifier
    */
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) 
   {
      this.widget = widget;
	  
	  elementID = "widget-moon-" + this.widget.id;
	  
	  // Initialisation of a unique canvas associated to this widget
	  $('<canvas />').attr({
		id: elementID
	    }).appendTo( "#widget-" + this.widget.id );  
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
		 
		 elementID = "widget-moon-" + this.widget.id;
		 
        //Hours are used to calculate the image number
        self.photoName ( "widgets/moon-phases/images/moon" + (parseInt(res)-1) + ".png" );			 
		 
		//get a reference to the canvas
		var ctx = $( "#" + elementID ).get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, self.WidgetWidth, self.WidgetHeight );
		
		//Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		$("#" + elementID ).attr('width' , self.WidgetWidth  );
        $("#" + elementID ).attr('height', self.WidgetHeight );	
		 
		base_image = new Image();
		base_image.id = elementID + "-image";
		base_image.src = self.photoName();
		
		base_image.onload = function()
		{
		   ctx.drawImage( base_image, 0, 0, self.WidgetWidth, self.WidgetHeight );
           
		   ctx.font="20px Georgia";
		   ctx.fillStyle = "rgb(255,255,255)";
		   //write the text at the same position as the height of the column
		   ctx.fillText(self.data() ,self.WidgetWidth / 2 - ( 10 * String(self.data()).match(/\d/g).length ), self.WidgetHeight / 2 );
		}  		
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
	   if (this.widget.height() <= 250 && this.widget.height() >= 150 && this.widget.width() <= 250 && this.widget.width() >= 150)
	   {
	      self.WidgetHeight = 200;
		  self.WidgetWidth = 200;	  
	   }
	   else
	   {
	      self.WidgetHeight = 100;
		  self.WidgetWidth = 100;	  
	   }
   };	   
  
   this.getDevicesForAcquisitions = function() {
      var result = [];

      //Add the moon keyword
      result.push(this.widget.configuration.device);
	  
	  return result;
   }
};