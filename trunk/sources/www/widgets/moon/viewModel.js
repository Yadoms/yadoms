widgetViewModelCtor =

/**
 * Create a Moon ViewModel
 * @constructor
 */
function MoonViewModel() {

   //observable data
   this.data = ko.observable("");
   this.MoonPhasesId = ko.observable ("");
   
   //simple data
   this.WidgetHeight = 69;
   this.WidgetWidth  = 69;   
   
   //Default value - This value is overwrite after
   this.photoName = ko.observable("widgets/moon/images/new.png");

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
	  $('<canvas class=\"canvas\" />').attr({
		id: elementID
	    }).appendTo( "#widget-" + this.widget.id );  
   };

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
   this.onNewAcquisition = function (keywordId, data) {
      var self = this;
	  
      if (keywordId == self.widget.configuration.device.keywordId) 
      {
		 var obj = jQuery.parseJSON( data.value );
		 self.data ( parseInt( obj.IlluminatedMoon ) + "%" );
		 var res = obj.DayOfMoon;
		 
		 elementID = "widget-moon-" + this.widget.id;
		 
		 console.log ( res );
		 
		 if (parseInt( obj.IlluminatedMoon ) <= 5)
		 {
			 self.photoName ( "widgets/moon/images/new.png" );
		 }
		 else if ( ( parseInt( obj.IlluminatedMoon ) > 5 ) && ( parseInt( obj.IlluminatedMoon ) <= 40 ))
		 {
			 if (parseInt(res) < 15)
				 self.photoName ( "widgets/moon/images/firstcrescent.png" );
			 else
				 self.photoName ( "widgets/moon/images/lastcrescent.png" ); 
		 }
		 else if ( ( parseInt( obj.IlluminatedMoon ) > 40 ) && ( parseInt( obj.IlluminatedMoon ) <= 60 ))
		 {
			 if (parseInt(res) < 15)
				 self.photoName ( "widgets/moon/images/firstquarter.png" );
			 else
				 self.photoName ( "widgets/moon/images/lastquarter.png" ); 
		 }
		 else if ( ( parseInt( obj.IlluminatedMoon ) > 60 ) && ( parseInt( obj.IlluminatedMoon ) <= 90 ))
		 {
			 if (parseInt(res) < 15)
				 self.photoName ( "widgets/moon/images/waninggibbous1.png" );
			 else
				 self.photoName ( "widgets/moon/images/waninggibbous2.png" ); 
		 }
         else
             self.photoName ( "widgets/moon/images/full.png" ); 			 
		 		 
		 self.RefreshImage( );
      }
   }
   
   this.RefreshImage = function( )
   {
	   var self = this;
	   
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
		{;
		   ctx.drawImage( base_image,100,100,300,300, 0, 0, self.WidgetWidth, self.WidgetHeight );

           ctx.fillStyle = "rgb(0,0,0)";		   
		  
		   if (self.WidgetHeight >115)
		   {
		      ctx.font="20px Georgia";
			  //write the text at the same position as the height of the column
		      ctx.fillText(self.data() ,self.WidgetWidth / 2 - ( 7 * String(self.data()).match(/\d/g).length ), self.WidgetHeight / 6 );
		   }
		  else
		  {
			  ctx.font="12px Georgia";
			  //write the text at the same position as the height of the column
		      ctx.fillText(self.data() ,self.WidgetWidth / 2 - ( 4 * String(self.data()).match(/\d/g).length ), self.WidgetHeight / 6 );
		  }
		}	   
   }
   
   this.configurationChanged = function() 
   {
       var self = this;

       this.widget.ListenKeyword(this.widget.configuration.device.keywordId);
   };

   this.resized = function() 
   {
       var self = this;
	   
	   //The size is x2 only when the widget is square
	   if (this.widget.height() <= 250 && this.widget.height() >= 150 && this.widget.width() <= 250 && this.widget.width() >= 150)
	   {
	      self.WidgetHeight = 172;
		  self.WidgetWidth = 172;	  
	   }
	   else
	   {
	      self.WidgetHeight = 67;
		  self.WidgetWidth = 67;	  
	   }

		self.RefreshImage();   
   };	   
};