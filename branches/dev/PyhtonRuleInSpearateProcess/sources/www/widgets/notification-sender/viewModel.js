//TODO mettre à jour le contenu du select (champ "to") lorsque la liste des récipients change, sera
// possible lorsque le point suivant du Trello (https://trello.com/c/BXKNfMA3) sera résolu :
// "IHM, widgets : ajouter une méthode facilitant l'envoi de commande "KeywordManager.sendCommand = function(keyword, data, callback)" et masquant la requête REST (comme pour les autres requêtes REST)"

widgetViewModelCtor =

/**
 * Create a notification-sender ViewModel
 * @constructor
 */
function NotificationSenderViewModel() {
   var self = this;

   // Widget identifier
   self.widget = null;

   // Observable data
   self.toCaption = ko.observable($.t("notification-sender:to"));
   self.body = ko.observable();
   self.toSelected = ko.observable();
   
   // A recipient associated to a select entry
   var recipientTuple = function(data) {
      var self = this;
      self.id = ko.observable(data.id);
      self.name = ko.observable(data.name);
   };

   // Populate "to" select
   self.toList = ko.observableArray();
   
   // Disable the optionCaption "to" of the "to" select
   self.optionsAfterRender = function(option, item) {
      option.disabled = isNullOrUndefined(item);
   }

   self.send = function() {
      // Check that widget is configured, and to/body data are valid
      if ((!isNullOrUndefined(self.widget.configuration.device)) && (self.toSelected()) && (self.body())) {
         
         var notification = new Object();
         notification.to = self.toSelected();
         notification.body = self.body();
         notification.acknowledgment = 'true';
      
         KeywordManager.sendCommand(this.widget.configuration.device.keywordId, JSON.stringify(notification));
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
   
   /**
   * Function called when configuration change
   * @param widget widget class object
   */
   this.configurationChanged = function() {
      try {
         //we ask for keyword information, to filter recipients corresponding on keyword properties
         //(ie : recipients list contains only recipient having phone number for keywords supporting sms)
         KeywordManager.get(self.widget.configuration.device.keywordId, function (keyword) {
            if (isNullOrUndefined(keyword.typeInfo.associatedRecipientField)) {
               console.warning("typeInfo.associatedRecipientField not specified for keyword = " + self.widget.configuration.device.keywordId);
               return;
            }
            
            var recipientTuples = new Array();
            RecipientManager.getByField(keyword.typeInfo.associatedRecipientField, function (list) {
               $.each(list, function (recipientFieldKey, recipientField) {
                  RecipientManager.get(recipientField.idRecipient, function(recipient) {
                     self.toList.push(new recipientTuple({id: recipient.id, name: recipient}));
                  }, true);
               });
            }, true);
            
         });
      }
      catch(err) {}
   };

};
