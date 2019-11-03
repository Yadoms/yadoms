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

          this.createDomButton = function(keywordId, value, icon, counter)  {
             return '<button class="btn btn-default multicommandButton multicommandButton-' + counter + '" data-bind="event: {click: function() {commandClick(' + keywordId + ', \'' + value + '\')}}"><span class="glyphicon ' + icon + '"></span></button>';
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

              //remove all buttons
              $(".multicommandButton").remove();
              
              //create all buttons
              var counter = 0;
              if ((!isNullOrUndefined(self.widget.configuration.commands))) {
                  $.each(self.widget.configuration.commands, function (index, command) {
                     var btn = self.createDomButton(parseInt(command.content.target.keywordId), command.content.target.value, command.content.icon, counter);
                     self.widget.$content.find(".commands-container").append(btn);
                     
                     //force KnockOut to apply binding on newly created button
                     ko.applyBindings(self, $(".multicommandButton-" + counter)[0]);
                     
                     counter++;
                  });
              }
              
          };
      };