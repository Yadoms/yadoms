widgetViewModelCtor =

/**
 * Create a Numeric Display ViewModel
 * @constructor
 */
function NumericDisplayViewModel() {
   //observable data
   this.text = ko.observable("");
   this.data = ko.observable(0).extend({ numeric: 1 });
   this.unit = ko.observable("");
   this.fontSize = ko.observable (25);

   //widget identifier
   this.widget = null;

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
   };

   /**
    * FontSize Method for the size of the Data to display
    */   
   
   this.fontStyleCSS = ko.computed(function() {
	   return {"font-size": this.fontSize() + "px"};
   }, this );
   
   this.configurationChanged = function() {
      this.text(this.widget.configuration.text);

      var self = this;

      //we get the unit of the keyword
      KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) {
         self.unit($.t(keyword.units));
      });
   }
   
   /**
    * New acquisition handler
    * @param searchedDevice Device on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function(device, data) {
      var self = this;

      if (device == self.widget.configuration.device) 
	  {
         //it is the good device
         self.data(data.value);
		 
		 self.resizefont();
      }
   };

   this.resizefont = function()
   {
	   self = this;
	   
	 switch ( self.data().toString().length + self.unit().toString().length )
	 {
		 case 1:
		 case 2:
		 case 3:
		 case 4:
		 case 5:
		    self.fontSize (30);
			break;
		 case 6:
		 case 7:		 
			if (self.widget.width() < 120)
			   self.fontSize ( 25 );
			else
				self.fontSize ( 30 );
			break;
			break;
		 case 8:
		 case 9:		 
			if (self.widget.width() < 120)
			   self.fontSize ( 22 );
			else
				self.fontSize ( 30 );
			break;
         case 10:
		 case 11:
			if (self.widget.width() < 120)
			   self.fontSize ( 18 );
			else
				self.fontSize ( 30 );
			break;
		 default:
			self.fontSize ( 25 );
			break;
	 }
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