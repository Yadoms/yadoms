/**
 * Created by Nicolas on 01/03/14.
 */

function IntParameterHandler(objectToConfigure, name, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(name !== undefined, "name must be defined");
   assert(content !== undefined, "content must be defined");

   //if value is setted we use it else we use the default value else we use 0
   this.value = parseInt(currentValue);

   if (isNaN(this.value)) {
      this.value = parseInt(content.defaultValue);
      if (isNaN(this.value)) {
         console.warn("Unable to parse as int content.defaultValue of " + name + " parameter. Set to 0");
         this.value = 0;
      }
   }

   //we search min and max value
   this.minValue = parseInt(content.minimumValue);
   this.maxValue = parseInt(content.maximumValue);

   //we check if the param is double rounded. If yes we change the control by a spinEdit
   if ((!isNaN(this.minValue)) && (!isNaN(this.maxValue)))
      this.ValueRounded = true;
   else
      this.ValueRounded = false;

   this.name = name;
   this.objectToConfigure = objectToConfigure;
   this.content = content;
}

IntParameterHandler.prototype.getDOMObject = function () {
   //if (ValueRounded) {
      //we provide a spinEdit
   //}
   //else {
      //we provide a textBox
      return "<intput type=\"number\" id=\"" + this.name + "\" data-i18n=\"[placeholder]" + this.objectToConfigure +":configurationSchema." + this.name + ".name\" />" ;
   //}
}