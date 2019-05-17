widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
   function switchViewModel() {
      //observable data
      this.state = ko.observable("off"); // ["off", "on", "unknown" ]
      this.kind = ko.observable("simple");
      this.icon = ko.observable("");
      this.showDeviceName = ko.observable(true);
      this.readonly = ko.observable(true);
      this.invert = ko.observable(false);
      this.update = ko.observable(false);
      this.askConfirmation = false;

      this.capacity="";
      this.accessMode="";

      /**
       * method to send command to Yadoms
       * @param newState to send
       */
      this.formatAndSend = function (keywordId, newState) {
         var self = this;
         var cmd = null;

         if (self.invert()) {
            if (newState == "on")
               newState = "off";
            else if (newState == "off")
               newState = "on";
            else
               console.log("Widget Switch : formatAndSend : invalid newState value: " + newState + " (expected 'on' or 'off')");
         }
      
         if (!isNullOrUndefined(self.capacity)) {
            switch (self.capacity) {
               case "dimmable":
                  cmd = (newState == "on" ? 100 : 0);
                  break; // Adapt here the state for dimmable
               case "event":
                  cmd = 0;
                  break;
               default:
                  cmd = (newState == "off" ? 0 : 1);
                  break;
            }



            // We send the command only for Set and GetSet variables
            if (self.accessMode === "GetSet" || self.accessMode === "Set") {
               if (self.askConfirmation) {
                  Yadoms.modals.confirmation.loadAsync()
                     .done(() => {
                        Yadoms.showConfirmModal(Yadoms.btnKind.confirmCancel, $.t("modals.confirmation.title"), $.t("widgets.switch:confirmationExplanation"), function () {
                           KeywordManager.sendCommand(keywordId, cmd.toString());
                        });
                     });
               } else
                  KeywordManager.sendCommand(keywordId, cmd.toString());
            }
         }
      };
      
      this.command = ko.computed(function () {
         var self = this;
         
         var result=self.state();
         
         if (self.invert()) {
            //manage inversion
            switch (self.state()) {
               case "off":
                  result = "on";
                  break;
                  
               case "on":
                  result ="off";
                  break;
                  
               case "unknown":
               default:
                  result ="unknown";
                  break;
            }
         }
         console.log("switch val = " + result);
         return result;
      }, this);


      this.commandClick = function (newState) {
         var self = this;

         if (isNullOrUndefined(self.widget.configuration))
            return;

         // Checks for the first device
         if (!isNullOrUndefined(self.widget.configuration.device)) {
            self.formatAndSend(parseInt(self.widget.configuration.device.keywordId), newState);
         }

      };

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function () {
         var self = this;

         //we configure the toolbar
         self.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: true
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
         var arrayOfDeffered = [];
         var d = new $.Deferred();

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         // registration of the first keyword
         if (!isNullOrUndefined(this.widget.configuration.device)) {
            //we register keyword new acquisition
            self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.device.keywordId));

            //we register keyword for get last value at web client startup 
            self.widgetApi.getLastValue(parseInt(self.widget.configuration.device.keywordId));

            self.widgetApi.registerAdditionalInformation(["accessMode", "capacity"]); // We would like the unit !
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

         // register if the final state should be reversed
         try {
            self.invert(parseBool(self.widget.configuration.invert));
         } catch (error) {
            self.invert(false);
         }

         try {
            self.askConfirmation = parseBool(self.widget.configuration.askConfirmation);
         } catch (error) {
            self.askConfirmation = false;
         }

         // load library if needed, if it's a push button with icon
         if (self.kind() === 'pushButton') {
            arrayOfDeffered.push(asyncLoadJSLibs(["libs/bootstrap-iconpicker-1.9.0/js/bootstrap-iconpicker-iconset-all.min.js",
               "libs/bootstrap-iconpicker-1.9.0/js/bootstrap-iconpicker.min.js"
            ]));
            arrayOfDeffered.push(asyncLoadCss("libs/bootstrap-iconpicker-1.9.0/css/bootstrap-iconpicker.min.css"));
         }
         // This variable is used only for the display
         $.when.apply($, arrayOfDeffered)
            .done(d.resolve)
            .fail(d.reject);
         return d.promise();
      };

      /**
       * New acquisition handler
       * @param keywordId keywordId on which new acquisition was received
       * @param data Acquisition data
       */
      this.onNewAcquisition = function (keywordId, data) {
         var self = this;
         var readonly = false;

         if (isNullOrUndefined(this.widget.configuration) || isNullOrUndefined(this.widget.configuration.device))
            return;

         //Check if first device
         if (keywordId == parseInt(this.widget.configuration.device.keywordId)) {
            if (!isNullOrUndefinedOrEmpty(data.capacity))
               self.capacity = data.capacity;

            if (!isNullOrUndefinedOrEmpty(data.accessMode)) {
               self.accessMode = data.accessMode;
               if (data.accessMode === "GetSet")
                  readonly |= false;
               else
                  readonly |= true;
            }

            //it is the right device
            switch (self.capacity) {
               case "dimmable":
                  switch(parseInt(data.value)) {
                     case 0:
                        self.state("off");
                        break;
                     case 100:
                        self.state("on");
                        break;
                     default: //all other values
                        self.state("unknown");
                        break;
                  }
                  break; // Adapt here the state for dimmable
               case "event":
                  self.state("off");
                  break;
               default:
                  self.state((parseInt(data.value) == 0?"off":"on"));
                  break;
            }


         }

         self.readonly(readonly);

         //knockout doesn't work with bootstrap. So change values have to be done manually
         if (self.kind() === 'toggle') {
            self.update(true);
            if (self.readonly())
               this.widget.$content.find("input[type=checkbox]").prop('disabled', false);

            if (self.command() != "off")
               this.widget.$content.find("input[type=checkbox]").prop('checked', true).change();
            else
               this.widget.$content.find("input[type=checkbox]").prop('checked', false).change();

            switch(self.command())
            {
               case "on":
                  this.widget.$content.find(".toggle-on").removeClass("indeterminate");
                  this.widget.$content.find(".toggle-on").html("On");
                  this.widget.$content.find(".toggle-handle").removeClass("hidden");
                  break;
               case "off":
                  this.widget.$content.find(".toggle-on").removeClass("indeterminate");
                  this.widget.$content.find(".toggle-on").html("Off");
                  this.widget.$content.find(".toggle-handle").removeClass("hidden");
                  break;
               case "unknown":
               default:
                  this.widget.$content.find(".toggle-on").html("-");
                  this.widget.$content.find(".toggle-on").addClass("indeterminate");
                  this.widget.$content.find(".toggle-handle").addClass("hidden");
                  break;
            }            
            
            self.update(false);
            this.widget.$content.find("input[type=checkbox]").prop('disabled', self.readonly());
         }
      };

      this.toggleCommand = function () {
         var self = this;
         if (self.command() == "off")
            self.commandClick("on");
         else
            self.commandClick("off"); //for on and unknown 
      };
   };