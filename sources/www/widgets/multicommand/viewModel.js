widgetViewModelCtor =

   /**
    * Create a Multicommand ViewModel
    * @constructor
    */
      function MulticommandViewModel() {
          /**
           * method to send command to Yadoms
           * @param keywordId    The targetted keyword
           * @param newState     The value to set
           */        
          this.commandClick = function (keywordId, newState) {
             debugger;
             KeywordManager.sendCommand(keywordId, newState.toString())
             .fail(function(error) {
                self.widgetApi.notify(error, "error");
             });             
          };

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
             //we configure the toolbar
             this.widgetApi.toolbar({
                 activated: true,
                 displayTitle: true,
                 batteryItem: false
             });
             
          };

          this.createDomButton = function(keywordId, value, icon)  {
             return '<button class="btn btn-default multicommandButton multicommandButton-' + keywordId + '" data-bind="event: {click: function() {commandClick(' + keywordId + ', \'' + value + '\')}}"><span class="glyphicon ' + icon + '"></span></button>';
          };
          
          /**
           * ConfigurationChanged method
           * @param none
           */          
          this.configurationChanged = function () {
              var self = this;
              var readOnlyMode=false;
              var arrayOfDeffered = [];

              if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
                  return;

              if ((!isNullOrUndefined(self.widget.configuration.commands))) {
                  $.each(self.widget.configuration.commands, function (index, command) {
                     var btn = self.createDomButton(command.content.target.keywordId, command.content.target.value, command.content.icon);
                     self.widget.$content.find(".commands-container").append(btn);
                     
                     //force KnockOut to apply binding on newly created button
                     ko.applyBindings(self, $(".multicommandButton-" + command.content.target.keywordId)[0]);
                  });
              }
              
          };
      };