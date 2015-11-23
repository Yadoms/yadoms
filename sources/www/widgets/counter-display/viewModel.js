widgetViewModelCtor =

/**
 * Create a Counter Display ViewModel
 * @constructor
 */
function CounterDisplayViewModel() {
   //observable data
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");
   this.fontSize = ko.observable (20);
   
   this.minimumIntegerDigit = 9;
   this.odometer = null;

   //widget identifier
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
	  
	odometerOptions = { auto: false }; // Disables auto-initialization
	
	  var elementID = "widget-counter-" + this.widget.id; // Unique ID
  
  // Initialisation of a unique div associated to this widget
	var elem = $('<div />').attr({
	id: elementID,
	"class":"odometer",
	"display":"table"
	}).appendTo( "#widget-" + this.widget.id );
	
	// For each odometer, initialize with the theme passed in:
	this.odometer = new Odometer({ el: $("#" + elementID + ".odometer")[0], format: '(.ddd)', value: 0, theme: 'car', duration: 1000, selector: '.my-numbers', minimumIntegerDigit: this.minimumIntegerDigit });
   };

   /**
    * FontSize Method for the size of the Data to display
    */   
   
   this.fontStyleCSS = ko.computed(function() {
	   return {"font-size": this.fontSize() + "px"};
   }, this );
   
   this.configurationChanged = function() {

      var self = this;
	  
      //we get the unit of the keyword
      KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) {
		  
		  // Read the unit
         self.unit($.t(keyword.units));
		 
		 //Set the Unit for odometer
		 self.odometer.SetUnit( self.unit() );
		 
		 self.resizefont();
      });
   }
   
   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) {
      var self = this;	  
	  
	  var elementID = "widget-counter-" + self.widget.id; // Unique ID
	  
      if (device == self.widget.configuration.device) 
	  {
         //it is the good device
		 $("#" + elementID + ".odometer").html( data.value );
      }
   };

   this.resizefont = function()
   {
	   self = this;
	   
	   var elementID = "widget-counter-" + self.widget.id; // Unique ID
	   
       //Compute the font-size needed.
	   self.fontSize ( ((this.widget.width() - 9 - ( (this.minimumIntegerDigit / 3) - 1)*8.33) / (this.minimumIntegerDigit + self.unit().length) ) *1.11 );
	   
	   //Change the font-size value
	   $("div#" + elementID + ".odometer").css({"font-size": self.fontSize() + "px", "margin":"auto", "display": "table"}); 
   }
   
   this.resized = function() 
   {
       var self = this;
   
	   self.resizefont();
   };	   
   
   this.getDevicesForAcquisitions = function() {
      var result = [];
      result.push(this.widget.configuration.device);

      return result;
   }
};