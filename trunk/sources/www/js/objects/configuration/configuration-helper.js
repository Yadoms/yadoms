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
         "<button id=\"" + parameterHandler.uuid + "-help\" class=\"col-sm-1\" type=\"button\" class=\"btn btn-default\" data-container=\"body\">" +
            "<i class=\"fa fa-question\"></i>" +
         "</button>" +
         "<div class=\"controls col-sm-6\">" +
            controlToInsert +
         "</div>" +
      "</div>" +
      "<script>" +
         "$(\"#" + parameterHandler.uuid + "\").popover({\"placement\" : \"right\", \"trigger\": \"manual\"});\n" +
         "$(\"button#" + parameterHandler.uuid + "-help\").click(function () {\n" +
            "$(\"#" + parameterHandler.uuid + "\").popover(\"toggle\");\n" +
            "setTimeout(function () {\n" +
               "$(\"#" + parameterHandler.uuid + "\").popover(\"hide\"); \n" +
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
ConfigurationHelper.createParameterHandler = function (objectToConfigure, i18nContext, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain widget or plugin object");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined in " + paramName + " parameter");
   assert(content.type !== undefined, "type field must be found in " + paramName + " parameter");
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n " + paramName + " parameter");

   //objectToConfigure will be use later ton check special types (ie: serialPort only for plugins and not for widgets)

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

      case "device" :
         return new DeviceParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + paramName + " is unsupported");
         break;
   }
};
