//TODO renommer le widget complet ?
//TODO mettre à jour le contenu du select (champ "to") lorsque la liste des récipients change
widgetViewModelCtor =

/**
 * Create a sms-sender ViewModel
 * @constructor
 */
function SmsSenderViewModel() {
   var self = this;

   //widget identifier
   this.widget = null;

   //observable data
   this.body = ko.observable();
   this.smsSenderText = ko.observable();
   self.toSelected = ko.observable();
   
   //a recipient associated to a select entry
   var recipientTuple = function(data) {
      var self = this;
      self.id = ko.observable(data.id);
      self.name = ko.observable(data.name);
   };

   //populate "to" select
   this.toList = ko.computed(function() {      
      var recipientTuples = new Array();
      RecipientManager.getAll(function (list) {
         $.each(list, function (recipientKey, recipient) {
            recipientTuples.push(new recipientTuple({id: recipient.id, name: (recipient.firstName + " " + recipient.lastName)}));
         });
      }, true);
      return recipientTuples;
   }, this);

   this.send = function() {
      // Check that widget is configured, and to/body data are valid
      if ((!isNullOrUndefined(this.widget.configuration.device)) && (this.toSelected()) && (this.body())) {
         
         var sms = new Object();
         sms.to = self.toSelected();
         sms.body = this.body();
         sms.acknowledgment = 'true';
      
         $.ajax({
            type: "POST",
            url: "/rest/device/keyword/" + this.widget.configuration.device.keywordId + "/command",
            data: JSON.stringify(sms),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("sms-sender:errorDuringSending"), JSON.stringify(data));
               }
            })
            .fail(function() {notifyError($.t("sms-sender:errorDuringSending")); });
      }
   };

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function(widget) {
      this.widget = widget;
      var self = this;
   };

   this.configurationChanged = function() {
      //we update the kind observable property
      if ((isNullOrUndefined(this.widget)) || (isNullOrUndefined(this.widget.configuration)))
         return;

      if (!isNullOrUndefined(this.widget.configuration.kind)) {
         this.kind(this.widget.configuration.kind);
      }

      if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
         this.showDeviceName(this.widget.configuration.showDeviceName);
      }

      //we ask for device information
      if (!isNullOrUndefined(this.widget.configuration.device)) {
         var self = this;
         $.getJSON("rest/device/" + this.widget.configuration.device.deviceId)
            .done(function( data ) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("widgets.errors.errorDuringGettingDeviceInformation"), JSON.stringify(data));
                  return;
               }
               self.smsSenderText(data.data.name);
            })
            .fail(function() {notifyError($.t("widgets.errors.errorDuringGettingDeviceInformation"));});
      }
   };

   /**
    * Dispatch the data to the viewModel
    * @deviceId device identifier which make the values
    * @param data data to dispatch
    * @param device
    */
   this.dispatch = function(device, data) {
      /* Nothing to do here, this widget doesn't display data */
   };

   this.getDevicesToListen = function() {
      var result = [];

      if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
         result.push(this.widget.configuration.device);
      }
      return result;
   }

};
