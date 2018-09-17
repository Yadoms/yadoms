widgetViewModelCtor =

   /**
    * Create a Shutter ViewModel
    * @constructor
    */
      function shutterViewModel() {
         var self = this;
         //observable data
         this.command = ko.observable(1);
         this.kind = ko.observable("simple");
         this.invert = ko.observable(false);
         this.icon = ko.observable("");
         this.readonly = ko.observable(true);
         this.capacity = "";

          // default size
          this.WidgetHeight = 70;
          this.WidgetWidth = 95;

          this.commandClick = function (newState) {
              var self = this;
              if(self.readonly() !== true) {
                 if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                     var cmd = null;

					  switch (self.capacity) {
						 case "curtain":
							if (newState == 0) {
								cmd = "Close";
							}else{
								cmd = "Open";
							}
							break;
                   case "dimmable":
                     if (newState == 0) {
                        cmd = 0;
                     }else{
                        cmd = 100;
                     }
                     break;                   
						 default:
							 cmd = newState;
							 break;
					  }
                     KeywordManager.sendCommand(this.widget.configuration.device.keywordId, cmd.toString());
                 }
              }
          };

          self.shutterIcon = ko.computed(function () {
             var displayValue = self.command();
              
              if (self.invert())
                  displayValue = !displayValue;
			  
              if (self.kind() !== "verticalInverter")
              {
                 if (displayValue)
                     return "widgets/shutter/icons/" + self.kind() + "-open.png";
                 else
                     return "widgets/shutter/icons/" + self.kind() + "-close.png";
              }
              else 
                 return "widgets/shutter/icons/window-open.png"; // default value
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

          this.shutterClick = function () {
             var self = this;
			    var cmd = null;
              
              if(self.readonly() !== true) {
                  if (self.command() == 0) {
                     self.command(1);
							cmd = 1;
						 }else{
                     self.command(0);
							cmd = 0;
						 }
                 //Send the command
                 this.commandClick(cmd);
              }
          }

          this.configurationChanged = function () {
              var self = this;

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              if (!isNullOrUndefined(this.widget.configuration.device)) {
                  //we register keyword new acquisition
                  self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);	   
			   
                  //we register keyword for get last value at web client startup 
				      self.widgetApi.getLastValue(self.widget.configuration.device.keywordId); 
                  self.widgetApi.registerAdditionalInformation(["accessMode", "capacity"]); // We would like the unit !

              if (!isNullOrUndefined(this.widget.configuration.kind)) {
                  this.kind(this.widget.configuration.kind.activeSection);
              }

              if (!isNullOrUndefined(this.widget.configuration.invert)) {
                  this.invert(parseBool(this.widget.configuration.invert));
              }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;

              if ((this.widget.configuration == undefined) || (this.widget.configuration.device == undefined))
                 return;
              
              if (keywordId === this.widget.configuration.device.keywordId) {
                 
                 if (!isNullOrUndefinedOrEmpty(data.capacityName))
                    self.capacity   = keyword.capacityName;
                 
                 if (!isNullOrUndefinedOrEmpty(data.accessMode)){
                     if (keyword.accessMode === "GetSet")
                         self.readonly(false);
                     else
                         self.readonly(true);
                 }
                 
                 switch (self.capacity) {
                   case "curtain":
                       if (data.value.toLowerCase()==="open")
                        self.command(1);
                      else
                        self.command(0);
                      break;
                    default:
                       // Adapt for dimmable or switch capacities
                       if (parseInt(data.value) !== 0)
                          self.command(1);
                       else
                          self.command(0);
                     break;
                 }
               }
          };
      };