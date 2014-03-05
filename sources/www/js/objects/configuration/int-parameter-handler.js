/**
 * Created by Nicolas on 01/03/14.
 */

function IntParameterHandler(i18nContext, name, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
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

   this.name = name;
   this.i18nContext = i18nContext;
   this.content = content;
}

function createFormGroup(parameterHandler, controlToInsert) {

   assert(parameterHandler !== undefined, "parameterHandler must be defined");
   assert(controlToInsert !== undefined, "controlToInsert must be defined");

   var s =
   "<div class=\"form-group\">" +
      "<label for=\"" + parameterHandler.name + "\" data-i18n=\"" + parameterHandler.i18nContext + parameterHandler.name + ".name\" class=\"control-label col-sm-3\"></label>" +
      "<button id=\"" + parameterHandler.name + "-help\" class=\"col-sm-1\" type=\"button\" class=\"btn btn-default\" data-container=\"body\">" +
         "<i class=\"fa fa-question\"></i>" +
      "</button>" +
      "<div class=\"col-sm-8\">" +
         controlToInsert +
      "</div>" +
   "</div>" +
   "<script>" +
      "$(\"#" + parameterHandler.name + "\").popover({\"placement\" : \"right\"});\n" +
      "$(\"button#" + parameterHandler.name + "-help\").click(function () {\n" +
         "$(\"#" + parameterHandler.name + "\").popover(\"toggle\");\n" +
         "setTimeout(function () {\n" +
            "$(\"#" + parameterHandler.name + "\").popover(\"hide\"); \n" +
         "}, 5000);\n" +
      "});\n" +
   "</script>";

   return s;
}

IntParameterHandler.prototype.getDOMObject = function () {
   //we provide a SpinEdit
   var input = "<input " +
                        "type=\"number\" " +
                        "class=\"form-control\" " +
                        "id=\"" + this.name + "\" " +
                        "data-i18n=\"[data-content]" + this.i18nContext + this.name + ".description\" ";
   if (!isNaN(this.maxValue)) {
      input += "max=\"" + this.maxValue + "\" ";
      input += "data-validation-max-message=\"" + "Valeur maximum depassée" + "\""
   }
   if (!isNaN(this.minValue))
      input += "min=\"" + this.minValue + "\" ";
   "aria-invalid=\"true\"" +
   ">";

   var self = this;

   return createFormGroup(self, input);
}