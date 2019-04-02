widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
   function switchViewModel() {
      //observable data
      this.state = ko.observableArray();
      this.kind = ko.observable("simple");
      this.icon = ko.observable("");
      this.showDeviceName = ko.observable(true);
      this.readonly = ko.observable(true);
      this.invert = ko.observable(false);
      this.update = ko.observable(false);
      this.askConfirmation = false;

      this.capacity = [];
      this.accessMode = [];

      /**
       * method to send command to Yadoms
       * @param newState to send
       */
      this.formatAndSend = function (index, keywordId, newState) {
         var self = this;
         var cmd = null;

         if (!isNullOrUndefined(self.capacity[index])) {
            switch (self.capacity[index]) {
               case "dimmable":
                  cmd = newState == 1 ? 100 : 0;
                  break; // Adapt here the state for dimmable
               case "event":
                  cmd = 0;
                  break;
               default:
                  cmd = newState;
                  break;
            }

            if (self.invert()) {
               if (cmd == 1)
                  cmd = 0;
               else if (cmd == 0)
                  cmd = 1;
            }

            // We send the command only for Set and GetSet variables
            if (self.accessMode[index] === "GetSet" || self.accessMode[index] === "Set") {
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
         var average = 0;

         if (self.state().length != 0) {
            $.each(self.state(), function (index, value) {
               average = average + value;
            });
            average = average / self.state().length;
         }

         if (self.invert()) {
            if (average == 1)
               average = 0;
            else if (average == 0)
               average = 1;
         }

         return average;
      }, this);

      this.commandClick = function (newState) {
         var self = this;

         if (isNullOrUndefined(self.widget.configuration))
            return;

         // Checks for the first device
         if (!isNullOrUndefined(self.widget.configuration.device)) {
            self.formatAndSend(0, self.widget.configuration.device.keywordId, newState);
         }

         if (!isNullOrUndefined(self.widget.configuration.additionalDevices.content.devices)) {
            // Check for the others devices
            $.each(self.widget.configuration.additionalDevices.content.devices, function (index, device) {
               self.formatAndSend(index + 1, device.content.source.keywordId, newState);
            });
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
            self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);

            //we register keyword for get last value at web client startup 
            self.widgetApi.getLastValue(self.widget.configuration.device.keywordId);

            self.widgetApi.registerAdditionalInformation(["accessMode", "capacity"]); // We would like the unit !
         }

         if (self.state.length == 0)
            self.state.push(1);

         // registration of additional keywords
         if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices))) {
            $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
               //we register keyword new acquisition
               self.widgetApi.registerKeywordForNewAcquisitions(device.content.source.keywordId);

               //we register keyword for get last value at web client startup 
               self.widgetApi.getLastValue(device.content.source.keywordId);
               if (self.state.length != index + 1)
                  self.state.push(1);
            });
         } else { // we remove elements if any
            if (self.state().length > 1)
               self.state.splice(1, self.state().length - 1);
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
            .done(function () {
               d.resolve();
            })
            .fail(function () {
               d.reject();
            });

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
               self.capacity[0] = data.capacity;

            if (!isNullOrUndefinedOrEmpty(data.accessMode)) {
               self.accessMode[0] = data.accessMode;
               if (data.accessMode === "GetSet")
                  readonly |= false;
               else
                  readonly |= true;
            }

            //it is the right device
            if (self.capacity[0] === "event")
               self.state.replace(self.state()[0], 0);
            else {
               self.state.replace(self.state()[0], parseInt(data.value));
            }
         }

         //check if additional devices
         if (!isNullOrUndefined(this.widget.configuration.additionalDevices.content.devices)) {
            $.each(this.widget.configuration.additionalDevices.content.devices, function (index, device) {
               if (keywordId == parseInt(device.content.source.keywordId)) {
                  if (!isNullOrUndefinedOrEmpty(data.capacity))
                     self.capacity[index + 1] = data.capacity;

                  if (!isNullOrUndefinedOrEmpty(data.accessMode)) {
                     self.accessMode[index + 1] = data.accessMode;
                     if (data.accessMode === "GetSet")
                        readonly |= false;
                     else
                        readonly |= true;
                  }

                  //it is the right device
                  if (self.capacity[index] === "event")
                     self.state()[index + 1] = 0;
                  else
                     self.state()[index + 1] = parseInt(data.value); // get the state. Same for dimmable
               }
            });
         }

         self.readonly(readonly);

         //knockout doesn't work witj bootstrap. So change values have to be done manually
         if (self.kind() === 'toggle') {
            self.update(true);
            if (self.readonly())
               this.widget.$content.find("input[type=checkbox]").prop('disabled', false);

            if (self.command() == 1)
               this.widget.$content.find("input[type=checkbox]").prop('checked', true).change();
            else
               this.widget.$content.find("input[type=checkbox]").prop('checked', false).change();

            self.update(false);
            this.widget.$content.find("input[type=checkbox]").prop('disabled', self.readonly());
         }
      };

      this.toggleCommand = function () {
         var self = this;
         var average = 0;

         if (self.command() == 0)
            self.commandClick(1);
         else
            self.commandClick(0);
      };
   };