widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
      function SwitchViewModel() {
      //observable data
      this.command = ko.observable(1);

      //widget identifier
      this.widget = null;

      this.kind = ko.observable("simple");
      this.icon = ko.observable("");
      
      this.capacity = [];

      this.showDeviceName = ko.observable(true);

      this.commandClick = function(newState) {
		  
		  self = this;
		  
         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.devices)) ) {
            var cmd = null;
			
			$.each(this.widget.configuration.devices, function(index, device) {
				
				console.log ("self.capacity", self.capacity[index] );
				
				if (!isNullOrUndefined(self.capacity[index]))
				{				
					switch (self.capacity[index]) {
					   case "dimmable": cmd = newState == 1 ? 100 : 0; break;
					   case "event": cmd = 1; break;
					   default: cmd = newState; break;
					}
				
				   KeywordManager.sendCommand( device.content.source.keywordId, cmd.toString() );
				   
				   console.log ( "Send Command to ", device.content.source.keywordId );
				   console.log ( "Send Command :", cmd );
				}
			});
         }
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
         //we update the kind observable property

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         if (!isNullOrUndefined(this.widget.configuration.kind)) {
            this.kind(this.widget.configuration.kind.activeSection);
            if (this.widget.configuration.kind.content.pushButton.radio) {
               this.icon("glyphicon " + this.widget.configuration.kind.content.pushButton.content.icon);
            }
         }

         if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
            this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
         }
		 
         //we ask for device information
         if ((!isNullOrUndefined(this.widget.configuration.devices)) ) {
			 
			$.each(this.widget.configuration.devices, function(index, device) {	
				
				console.log (device.content.source);
				
				if (!isNullOrUndefined(device.content.source.deviceId))
				{	
					// Get the capacity of the keyword
					KeywordManager.get(device.content.source.keywordId, function(keyword) {
					   self.capacity[index] = keyword.capacityName;
					});
				}
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
               //it is the good device
               if (this.capacity == "event")
                  self.command(0);
               else
                  // Adapt for dimmable or switch capacities
                  self.command(parseInt(data.value) != 0 ? "1" : "0");
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
