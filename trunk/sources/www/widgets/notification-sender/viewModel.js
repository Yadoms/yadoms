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
   self.toList = ko.computed(function() {      
      var recipientTuples = new Array();
      RecipientManager.getAll(function (list) {
         $.each(list, function (recipientKey, recipient) {
            recipientTuples.push(new recipientTuple({id: recipient.id, name: (recipient.firstName + " " + recipient.lastName)}));
         });
      }, true);
      return recipientTuples;
   }, self);
   
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
      
         $.ajax({
            type: "POST",
            url: "/rest/device/keyword/" + self.widget.configuration.device.keywordId + "/command",
            data: JSON.stringify(notification),
            contentType: "application/json; charset=utf-8",
            dataType: "json"
         })
            .done(function(data) {
               //we parse the json answer
               if (data.result != "true")
               {
                  notifyError($.t("notification-sender:errorDuringSending"), JSON.stringify(data));
               }
            })
            .fail(function() {notifyError($.t("notification-sender:errorDuringSending")); });
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

};
