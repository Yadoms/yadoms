/**
 * Created by nhilaire on 15/06/2015.
 */

/**
 * This extenders allow you to display numbers with precision
 * For example :
 * this.data = ko.observable(0).extend({ numeric: 1 });
 * this.data(12.5895);
 * will display 12.6
  */
ko.extenders.numeric = function(target, precision) {
   //create a writeable computed observable to intercept writes to our observable
   var result = ko.computed({
      read: target,  //always return the original observables value
      write: function(newValue) {
         var current = target(),
             roundingMultiplier = Math.pow(10, precision),
             newValueAsNum = isNaN(newValue) ? 0 : parseFloat(+newValue),
             valueToWrite = Math.round(newValueAsNum * roundingMultiplier) / roundingMultiplier;

         //only write if it changed
         if (valueToWrite !== current) {
            target(valueToWrite);
         } else {
            //if the rounded value is the same, but a different value was written, force a notification for the current field
            if (newValue !== current) {
               target.notifySubscribers(valueToWrite);
            }
         }
      }
   }).extend({ notify: 'always' });

   //initialize with current value to make sure it is rounded appropriately
   result(target());

   //return the new computed observable
   return result;
};

/**
 * This extenders allow you to used 'translatedText' instead of 'test' item to automatically manage i18n
 * <div class="numericDisplayWidgetUnit" data-bind="text: unit"></div>
 * should become
 * <div class="numericDisplayWidgetUnit" data-bind="translatedText: unit"></div>
 * and the self.unit(keyword.units) will automatically call the $t() method on tokeyword.units string
 * @type {{update: Function}}
 */
ko.bindingHandlers['translatedText'] = {
   update: function(element, valueAccessor, allBindings){
      var key = ko.unwrap(valueAccessor());
      var options = ko.toJS(allBindings.get('translationOptions') || {});
      var translation = i18n.t(key, options);
      element.innerText = translation;
   }
};