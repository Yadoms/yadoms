widgetViewModelCtor =

   /**
    * Create a Shutter ViewModel
    * @constructor
    */
      function shutterViewModel() {
		  
          //observable data
          this.command = ko.observable(1);
          this.kind = ko.observable("simple");
          this.icon = ko.observable("");
		  
		  // default size
		  this.WidgetHeight = 70;
		  this.WidgetWidth  = 95;		  

          this.commandClick = function (newState) {

              var self = this;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                  var cmd = null;
				  
				  cmd = newState;
				  KeywordManager.sendCommand(this.widget.configuration.device.keywordId, cmd.toString());
              }
          };

		  this.shutterIcon = ko.pureComputed(function() {
			 if ( this.command() === 0 )
			    return "widgets/shutter/icons/close.png";
			 else 
				return "widgets/shutter/icons/open.png"; 
          }, this);
		  
          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
              //we configure the toolbar
              this.widgetApi.toolbar({
                  activated: true,
                  displayTitle: true,
                  batteryItem: false
              });
          };

        shutterClick = function() {
			self = this;

			  if ( self.command() === 0)
				 self.command(1);
			  else
				 self.command(0);
        }		   
		   
          this.configurationChanged = function () {
              var self = this;
              //we update the kind observable property

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                  self.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);
              }
			  
              if (!isNullOrUndefined(this.widget.configuration.kind)) {
                  this.kind(this.widget.configuration.kind.activeSection);
              }			  

              //if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
              //    this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
              //}

              //we ask for device information
              if ((!isNullOrUndefined(this.widget.configuration.device))) {

                      if (!isNullOrUndefined(this.widget.configuration.device.deviceId)) {
						  
                          //Initialisation initiale
                          self.command(0);
						  
						  console.log ( self.widget.getHeight() );
						  console.log ( self.widget.getWidth() );
						  
						   $("#shutterWidget").css("height", this.WidgetHeight + "px");						  
						   $("#shutterWidget").css("width", this.WidgetWidth + "px");
                      }
              }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;
			  
              if ((this.widget.configuration != undefined) && (this.widget.configuration.device != undefined)) {

                      if (keywordId === this.widget.configuration.device.keywordId) {
						  
						  // Adapt for dimmable or switch capacities
						  if (parseInt(data.value) !== 0)
							  self.command(1);
						  else
							  self.command(0);
                      }
					  
					  // Dans le cas du pushbutton, on fait le refresh du canvas
					  //if (self.kind() === 'pushButton')
					  //{
						  //console.log ( "valeur :", data.value );
						  //self.refresh();
					  //}
              }
          };
/*		  
   this.refresh = function()
   {
	   var self = this;
   
	   var element = self.widgetApi.find(".centerimage");
	   element.get(0).addEventListener("mousedown", mouseDown, false);
	   
	   function mouseDown() {
		  if ( self.command() == 0)
			 self.command(1);
		  else
			 self.command(0);

		  console.log ("mousedown");	
	   }		   
 
	   if ( self.command() == 0 )
	      self.shutterIcon("widgets/shutter/icons/close.png");
	   else
		   self.shutterIcon("widgets/shutter/icons/open.png");
*/
/*
	   //get a reference to the canvas
       var ctx = element.get(0).getContext("2d");
		
	   // Refresh the canvas, clear all existing information
	   ctx.clearRect(0, 0, self.WidgetWidth, self.WidgetHeight );
		
	   //Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
	   element.attr('width', self.WidgetWidth);
	   element.attr('height', self.WidgetHeight);
	   
       //Draw the window
	   
	   //ctx.fillStyle="#20E0FF";
	   ctx.fillStyle="#337ab7";
       //ctx.fillRect(10,10,50,50);
	   
       ctx.strokeStyle="black";//"#000000";
	   ctx.lineWidth=1;
       ctx.strokeRect(1,1,68,68);	   
	   
	   ctx.strokeRect(5,5,27,60);
	   ctx.fillRect  (5,5,27,60);
	   
	   ctx.strokeRect(38,5,27,60);
	   ctx.fillRect  (38,5,27,60);
	   
		ctx.beginPath();
		ctx.moveTo(5,45);
		ctx.lineTo(32,25);
		ctx.stroke();	   
	   
        ctx.beginPath();
		ctx.moveTo(38,22);
		ctx.lineTo(63,5);
		ctx.stroke();	   
	   */
	   /*
       ctx.fillStyle = "rgb(" + Math.round(255 - (tempMax - self.data()) * 255 / 90) + ",0," + Math.round(255 - (self.data() - tempMin) * 255 / 90) + ")";
		 
		//draw a circle
		ctx.beginPath();
		         // position x         position y             diameter
		ctx.arc(self.WidgetWidth / 2, posCenterBall , 8 * self.WidgetWidth / 100, 0, Math.PI*2, true); 
		ctx.closePath();
		ctx.fill();
		
   
		//write the text at the same position as the height of the column
		ctx.fillText(self.data() + "°",self.WidgetWidth / 2 + 15*self.WidgetWidth / 100, initialPositionY );
		  
        // Draw the thermometer
		//draw a circle
		ctx.beginPath();
		
		//black color
        ctx.fillStyle = "rgb(0,0,0)";
		ctx.lineWidth = 3;		
		
		//bubble
		         // position x        position y             diameter
		ctx.arc(self.WidgetWidth / 2, posCenterBall, 12 * self.WidgetWidth / 100, Math.PI*1.3, Math.PI*1.7, true);
		
		//top
		ctx.arc(self.WidgetWidth / 2, 10 * self.WidgetHeight / 100, 7 * self.WidgetWidth / 100, Math.PI*2, Math.PI*1, true);
		
		ctx.closePath();
		ctx.stroke();
   }	*/	  
      };
	