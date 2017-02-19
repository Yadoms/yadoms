widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
      function switchViewModel() {
		  
          //observable data
          this.state = [];
          this.command = ko.observable(1);
          this.kind = ko.observable("simple");
          this.icon = ko.observable("");
          this.capacity = [];
          this.accessMode = [];
          this.showDeviceName = ko.observable(true);
          this.readonly = ko.observable(true);
          this.update = ko.observable(false);
		  
          /**
           * method to send command to Yadoms
           * @param newState to send
           */        
          this.commandClick = function (newState) {

              var self = this;

              if  (!isNullOrUndefined(this.widget.configuration)) {
                  // Checks for the first device
                  if (!isNullOrUndefined(this.widget.configuration.device))
                  {
                     if (!isNullOrUndefined(self.capacity[0])) {
                        var cmd = null;
                        switch (self.capacity[0]) {
                           case "dimmable": cmd = newState == 1 ? 100 : 0; break;
                           case "event": cmd = 1; break;
                           default: cmd = newState; break;
                        }

                        // We send the command only for Set and GetSet variables
                        if ( self.accessMode[0] === "GetSet" || self.accessMode[0] === "Set" )
                           KeywordManager.sendCommand(this.widget.configuration.device.keywordId, cmd.toString());
                      }
                  }                   
                  
                  if (!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices)){
                     // Check for the others devices
                     $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
                    
                         if (!isNullOrUndefined(self.capacity[index+1])) {
                             var cmd = null;
                             switch (self.capacity[index+1]) {
                                 case "dimmable": cmd = newState == 1 ? 100 : 0; break;
                                 case "event": cmd = 1; break;
                                 default: cmd = newState; break;
                             }

                             // We send the command only for Set and GetSet variables
                             if ( self.accessMode[index+1] === "GetSet" || self.accessMode[index+1] === "Set" )
                                KeywordManager.sendCommand(device.content.source.keywordId, cmd.toString());
                         }
                     });
                  }
              }
          };

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
             self = this;
             
             //we configure the toolbar
             self.widgetApi.toolbar({
                 activated: true,
                 displayTitle: true,
                 batteryItem: false
             });
           
             // Initialization of the toogle button if exist
             this.widget.$content.find("input[type=checkbox]").bootstrapToggle();
          };

          /**
           * ConfigurationChanged method
           * @param none
           */          
          this.configurationChanged = function () {
              var self = this;
              var readOnlyMode=false;
              var arrayOfDeffered = [];

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              // registration of the first keyword
              if  (!isNullOrUndefined(this.widget.configuration.device)) {
                self.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);                 
              }
              
              // registration of additional keywords
              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices))) {
                  $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
                      self.widgetApi.registerKeywordAcquisitions(device.content.source.keywordId);
                  });
              }

              // update of the kind of switchs
              // keep the icon selected in ocase of pushbutton selected
              if (!isNullOrUndefined(this.widget.configuration.kind)) {
                  this.kind(this.widget.configuration.kind.activeSection);
                  if (this.widget.configuration.kind.content.pushButton.radio) {
                      this.icon("glyphicon " + this.widget.configuration.kind.content.pushButton.content.icon);
                  }
              }

              // keep the information if the device name should be shown
              if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
                  this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
              }

              //we ask for the first device information
              if  (!isNullOrUndefined(this.widget.configuration.device.deviceId)) {
                 
                // Get the capacity of the keyword
                var deffered = KeywordManager.get(this.widget.configuration.device.keywordId)
                .done(function (keyword) {
                  self.capacity[0]   = keyword.capacityName;
                  self.accessMode[0] = keyword.accessMode;

                  if ( keyword.accessMode === "Get" )
                     readOnlyMode = true;

                });
          
                arrayOfDeffered.push(deffered);

                //Initialization
                if (isNullOrUndefined(self.state[0]))
                   self.state[0] = 0;
              }
              
              //we ask for additional devices information
              if ((!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices))) {
                  $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
                      if (!isNullOrUndefined(device.content.source.deviceId)) {
						  
                         // Get the capacity of the keyword
                         var deffered = KeywordManager.get(device.content.source.keywordId)
                         .done(function (keyword) {
                           self.capacity[index+1]   = keyword.capacityName;
                           self.accessMode[index+1] = keyword.accessMode;
                           
                           if ( keyword.accessMode === "Get" )
                              readOnlyMode = true;

                         });
						 
						       arrayOfDeffered.push(deffered);

                         //Initialization
                         if (isNullOrUndefined(self.state[index+1]))
                            self.state[index+1] = 0;
                      }
                  });
				  
                  // This variable is used only for the display
                  $.whenAll(arrayOfDeffered).done(function () {
                     self.readonly ( readOnlyMode );
                  });				  
               }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;
              
              if (!isNullOrUndefined(this.widget.configuration)){

                 //Check if first device
                 if (!isNullOrUndefined(this.widget.configuration.device)) {
                    
                      if (keywordId === this.widget.configuration.device.keywordId) {
                          //it is the right device
                          if (self.capacity[0] === "event")
                              self.state[0] = 0;
                          else {
                              // Adapt for dimmable or switch capacities
                              if (parseInt(data.value) !== 0)
                                  self.state[0] = 1;
                              else
                                  self.state[0] = 0;
                          }
                      }
                 }                 
                 
                 //check if additional devices
                 if (!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices)) {
                   $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
                      if (keywordId === device.content.source.keywordId) {
                          //it is the right device
                          if (self.capacity[index] === "event")
                              self.state[index+1] = 0;
                          else {
                              // Adapt for dimmable or switch capacities
                              if (parseInt(data.value) !== 0)
                                  self.state[index+1] = 1;
                              else
                                  self.state[index+1] = 0;
                          }
                      }
                   });
                 }

                  self.evaluateCommand();
                  
                  //knockout doesn't work witj bootstrap. So change values have to be done manually
                  if (self.kind() === 'toggle')
                  {
                    self.update(true);
                    if (self.readonly())
                       this.widget.$content.find("input[type=checkbox]").prop('disabled', false);
                    
                    if (self.command()==1)
                       this.widget.$content.find("input[type=checkbox]").prop('checked', true).change();
                    else
                        this.widget.$content.find("input[type=checkbox]").prop('checked', false).change();
                     
                    self.update(false);
                    this.widget.$content.find("input[type=checkbox]").prop('disabled', self.readonly());
                  }
              }
          };

          this.evaluateCommand = function () {
              var self = this;
              var average = 0;
              
              $.each(this.state, function (index, state) {
                  average = average + state;
              });

              average = average / this.state.length;
              self.command(average);
          }
          
          this.toggleCommand = function () {
              self = this;

              if (this.command() === 0)
                  self.command(1);
              else
                  self.command(0);

              //Send the command
              self.commandClick(self.command());
          }          
      };