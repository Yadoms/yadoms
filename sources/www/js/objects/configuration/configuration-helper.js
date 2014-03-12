/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function ConfigurationHelper(){}

ConfigurationHelper.createControlGroup = function(parameterHandler, controlToInsert) {

   assert(parameterHandler !== undefined, "parameterHandler must be defined");
   assert(controlToInsert !== undefined, "controlToInsert must be defined");

   var s =
      "<div class=\"control-group\">" +
         "<label for=\"" + parameterHandler.paramName + "\" data-i18n=\"" + parameterHandler.i18nContext + parameterHandler.paramName + ".name\" class=\"control-label col-sm-5\">" + parameterHandler.name + "</label>" +
         "<button id=\"" + parameterHandler.paramName + "-help\" class=\"col-sm-1\" type=\"button\" class=\"btn btn-default\" data-container=\"body\">" +
            "<i class=\"fa fa-question\"></i>" +
         "</button>" +
         "<div class=\"controls col-sm-6\">" +
            controlToInsert +
         "</div>" +
      "</div>" +
      "<script>" +
         "$(\"#" + parameterHandler.paramName + "\").popover({\"placement\" : \"right\", \"trigger\": \"manual\"});\n" +
         "$(\"button#" + parameterHandler.paramName + "-help\").click(function () {\n" +
            "$(\"#" + parameterHandler.paramName + "\").popover(\"toggle\");\n" +
            "setTimeout(function () {\n" +
               "$(\"#" + parameterHandler.paramName + "\").popover(\"hide\"); \n" +
            "}, 5000);\n" +
         "});\n" +
      "</script>";

   return s;
}