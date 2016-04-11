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
			  
              //we ask for device information
              if ((!isNullOrUndefined(this.widget.configuration.device))) {

                      if (!isNullOrUndefined(this.widget.configuration.device.deviceId)) {
						  
                          //Initialisation initiale
                          self.command(0);
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
              }
          };
		  		  
    this.resized = function () {
		var self = this;
		if ( this.widget.getHeight() == 100 || this.widget.getWidth() == 100 )
		{
		   $(".shutter-image").css( "height", this.WidgetHeight );
           $(".shutter-image").css( "width",  this.WidgetWidth  );
           $(".shutter-image").removeAttr( "flex"  );
		}
		else 
		{
			$(".shutter-image").css( "width",  "100%" );	
			$(".shutter-image").css( "height", "auto" );
			$(".shutter-image").attr( "flex",  "auto" );
		}
	}
};
	