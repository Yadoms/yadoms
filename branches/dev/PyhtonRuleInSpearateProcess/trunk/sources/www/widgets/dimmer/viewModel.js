widgetViewModelCtor =

   /**
    * Create a Dimmer ViewModel
    * @constructor
    */
      function DimmerViewModel() {
      //observable data
      this.command = ko.observable(1).extend({ numeric: 1 });

	  this.DisplayValue = ko.observable("");
	  
      //widget identifier
      this.widget = null;

	  this.step = ko.observable(0.1).extend({ numeric: 1 });
      this.consoleText = ko.observable("");
	  
	  this.unit = ko.observable("");

      this.showDeviceName = ko.observable(true);

      this.commandClick = function(value) {
         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device)) )
		 {	
			this.command(this.command() + value);	
			KeywordManager.sendCommand(this.widget.configuration.device.keywordId, this.command().toString());
         }
		 
		 this.DisplayValue( this.command().toString() + this.unit() );
      };

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function(widget) {
         this.widget = widget;
      };

      this.configurationChanged = function() {
		  var self = this;

         if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;
		 
         if (!isNullOrUndefined(self.widget.configuration.showDeviceName)) {
            this.showDeviceName(parseBool(self.widget.configuration.showDeviceName));
         }
		 
		 //Read the step
		 self.step ( self.widget.configuration.StepValue ); 
		 
		//we get the unit of the keyword
        KeywordManager.get(self.widget.configuration.device.keywordId, function(keyword) {
           self.unit(keyword.units);
        });
		 
         //we ask for keyword information
         if ((!isNullOrUndefined(self.widget.configuration.device)) && (!isNullOrUndefined(this.widget.configuration.device.deviceId))) {
			
			KeywordManager.get (self.widget.configuration.device.keywordId, function (keyword) {	
			   self.consoleText(decodeURIComponent(keyword.friendlyName));
            });
         }
      };

      /**
       * New acquisition handler
       * @param searchedDevice Device on which new acquisition was received
       * @param data Acquisition data
       */
      this.onNewAcquisition = function(device, data) {
         var self = this;
         if ((this.widget.configuration !== undefined) && (this.widget.configuration.device !== undefined)) {
            if (device == this.widget.configuration.device) {
				self.command( parseFloat(data.value) );
		        self.DisplayValue( self.command() + self.unit() );
            }
         }
      };

      this.getDevicesForAcquisitions = function() {
         var result = [];

         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
            result.push(this.widget.configuration.device);
         }
         return result;
      }
   };