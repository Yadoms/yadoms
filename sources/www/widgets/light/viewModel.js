widgetViewModelCtor =

   /**
    * Create a light ViewModel
    * @constructor
    */
      function lightViewModel() {
          var self = this;
          //observable data
          this.command = ko.observable(1);
          this.icon = ko.observable("");
          this.readonly = ko.observable(true);
          this.capacity = "";

          // default size
          this.WidgetHeight = 70;
          this.WidgetWidth = 95;

          this.commandClick = function (newState) {

              var self = this;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                  var cmd = null;

                  switch (self.capacity) {
                     case "dimmable": cmd = newState == 1 ? 100 : 0; break;
					 case "curtain":
					    if (newState==1)
							cmd = "Open";
						else
							cmd = "Close";
						break;
                     default: cmd = newState; break;
                  }
                                    
                  KeywordManager.sendCommand(this.widget.configuration.device.keywordId, cmd.toString());
              }
          };

          self.LightIcon = ko.computed(function () {
              if (self.command() === 0)
                  return "widgets/light/icons/off.png";
              else
                  return "widgets/light/icons/on.png";
          });

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
              //we configure the toolbar
              this.widgetApi.toolbar({
                  activated: true,
                  displayTitle: true,
                  batteryItem: true
              });
          };

          this.LightClick = function () {
              var self = this;
              
              if (self.command() === 0)
                  self.command(1);
              else
                  self.command(0);
			  
			  //Send the command
			  this.commandClick ( self.command() );
          }

          this.configurationChanged = function () {
              var self = this;
              var deffered;

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              if (!isNullOrUndefined(this.widget.configuration.device)) {
                  //we register keyword new acquisition
                  self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
			   
                  //we register keyword for get last value at web client startup 
				  self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 				  
				  
                 // Get the capacity of the keyword
                 deffered = KeywordManager.get(this.widget.configuration.device.keywordId);
                 
                 deffered
                 .done(function (keyword) {
                     if ( keyword.accessMode ==="GetSet" )
                       self.readonly ( false );
                     else
                       self.readonly ( true );
                    
                     self.capacity   = keyword.capacityName;
                 });
              }else {
                 deffered = new $.Deferred().resolve();
              }
              
              return deffered.promise();
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;

              if ((this.widget.configuration != undefined) && (this.widget.configuration.device != undefined)) {

                  if (keywordId === parseInt(this.widget.configuration.device.keywordId)) {
                        switch (self.capacity) {
                           case "dimmable": 
						      if (parseInt(data.value) !== 0) 
								  self.command(100);
							  else 
								  self.command(0); 
							  break;
						   case "curtain":
						      if (data.value.toLowerCase() === "open")
							     self.command(1);
							  else if (data.value.toLowerCase() === "close")
								 self.command(0);
						      break;
                           default: 
						      if (parseInt(data.value) !== 0)
								  self.command(1); 
							  else 
								  self.command(0); 
							  break;
                        }                         
				  }
			    }
          };
      };
