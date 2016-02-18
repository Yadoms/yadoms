/**
 * Create a Moon ViewModel
 * @constructor
 */
widgetViewModelCtor = function moonViewModel() {

   //observable data
   this.illuminatedMoonValue = ko.observable(0).extend({ numeric: 1 });
   this.moonPicture = ko.observable("widgets/moon/images/new.png");

   /**
    * Initialization method
    * @param widget widget class object
    */
   this.initialize = function () {
   };

   /**
   * New acquisition handler
   * @param keywordId keywordId on which new acquisition was received
   * @param data Acquisition data
   */
   this.onNewAcquisition = function (keywordId, data) {
      var self = this;

      if (keywordId === self.widget.configuration.device.keywordId) {
         var obj = jQuery.parseJSON(data.value);
         if (obj) {
            //parse as integers
            var illuminatedMoon = parseInt(obj.IlluminatedMoon || 0);
            var dayOfMoon = parseInt(obj.DayOfMoon || 0);

            //update value binding
            self.illuminatedMoonValue(illuminatedMoon);

            //choose image and update binding
            if (illuminatedMoon <= 5) {
               self.moonPicture("widgets/moon/images/new.png");
            }
            else if (illuminatedMoon > 5 && illuminatedMoon <= 40) {
               if (dayOfMoon < 15)
                  self.moonPicture("widgets/moon/images/firstcrescent.png");
               else
                  self.moonPicture("widgets/moon/images/lastcrescent.png");
            }
            else if (illuminatedMoon > 40 && illuminatedMoon <= 60) {
               if (dayOfMoon < 15)
                  self.moonPicture("widgets/moon/images/firstquarter.png");
               else
                  self.moonPicture("widgets/moon/images/lastquarter.png");
            }
            else if (illuminatedMoon > 60 && illuminatedMoon <= 90) {
               if (dayOfMoon < 15)
                  self.moonPicture("widgets/moon/images/waninggibbous1.png");
               else
                  self.moonPicture("widgets/moon/images/waninggibbous2.png");
            }
            else
               self.moonPicture("widgets/moon/images/full.png");
         }
      }

      this.widgetApi.find(".moonPercentage").fitText();
   }


   this.configurationChanged = function () {
      //we register keyword new acquisition
      this.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);
   };
};