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
   //we provide a textBox
   return "<div class=\"form-group\">" +
               "<label for=\"" + this.name + "\" data-i18n=\"" + this.objectToConfigure.name +":configurationSchema." + this.name + ".name\"></label>" +
               "<div class=\"input-group\">" +
                  "<input type=\"number\" class=\"form-control\" id=\"" + this.name + "\" data-i18n=\"[title]" + this.objectToConfigure.name +":configurationSchema." + this.name + ".description\">" +
                  "<span class=\"input-group-btn\">" +
                     "<button id=\"" + this.name + "-help\" type=\"button\" class=\"btn btn-default\" data-container=\"body\" data-toggle=\"popover\" data-placement=\"right\" data-i18n=\"[data-content]" + this.objectToConfigure.name +":configurationSchema." + this.name + ".description\">" +
                        "<i class=\"fa fa-question\"></i>" +
                     "</button>" +
                  "</span>" +
               "</div>" +
          "</div>" +
          "<script>" +
               "$(\"button#" + this.name + "-help\").popover().click(function () {\n\r" +
                  "setTimeout(function () {\n\r" +
                     "$(\"button#" + this.name + "-help\").popover(\"hide\"); \n\r" +
                  "}, 5000);\n\r" +
               "});" +
          "</script>";
}