/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function ConfigurationHelper(){}

ConfigurationHelper.createFormGroup = function(parameterHandler, controlToInsert) {

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
         "$(\"#" + parameterHandler.name + "\").popover({\"placement\" : \"right\", \"trigger\": \"manual\",});\n" +
         "$(\"button#" + parameterHandler.name + "-help\").click(function () {\n" +
         "$(\"#" + parameterHandler.name + "\").popover(\"toggle\");\n" +
         "setTimeout(function () {\n" +
         "$(\"#" + parameterHandler.name + "\").popover(\"hide\"); \n" +
         "}, 5000);\n" +
         "});\n" +
         "</script>";

   return s;
}