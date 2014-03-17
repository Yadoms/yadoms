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

/**
 * Factory of parameter Handler
 * can be recursive on special types
 * @param name
 * @param content
 */
ConfigurationHelper.createParameterHandler = function (objectToConfigure, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(content.type !== undefined, "type field must be found in " + paramName + " parameter");

   //we build the i18n context
   var i18nContext = objectToConfigure.name + ":configurationSchema.";

   switch (content.type.toLowerCase()) {
      case "int" :
         return new IntParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "decimal" :
         return new DecimalParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "enum" :
         return new EnumParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "string" :
         return new StringParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "bool" :
         return new BoolParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "section" :
         return new SectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + paramName + " is unsupported");
         break;
   }
};
