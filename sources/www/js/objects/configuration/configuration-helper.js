/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function ConfigurationHelper(){}

ConfigurationHelper.createControlGroup = function (parameterHandler, controlToInsert, placeInsideLabel, classOfControlGroup) {

   assert(parameterHandler !== undefined, "parameterHandler must be defined");
   assert(controlToInsert !== undefined, "controlToInsert must be defined");

   //if we don't ask to place the label inside the div we place it outside
   if (isNullOrUndefined(placeInsideLabel))
      placeInsideLabel = false;

   var s =
      "<div class=\"control-group";
   if (!isNullOrUndefined(classOfControlGroup))
      s += " " + classOfControlGroup;
   s += "\">" +
      "<div class=\"configuration-label\">" +
      "<label for=\"" + parameterHandler.paramName + "\" class=\"control-label";
   if (placeInsideLabel)
      s += " col-md-offset-6 ";
   s += " col-sm-6\">";
   if (placeInsideLabel)
      s += controlToInsert;

   s += "<span class=\"configuration-label-content configuration-label-name\" data-i18n=\"" + parameterHandler.i18nContext + parameterHandler.paramName + ".name\">" + parameterHandler.name + "</span>" +
      "<span class=\"configuration-label-content configuration-label-description\" data-i18n=\"" + parameterHandler.i18nContext + parameterHandler.paramName + ".description\">" + parameterHandler.description + "</span>" +
      "</label>" +
      "</div>";
   if (!placeInsideLabel) {
      s += "<div class=\"controls col-sm-6 configuration-control\">" +
         controlToInsert +
         "</div>";
   }

   s += "</div>";

   return s;
};

/**
 * Factory of parameter Handler
 * @param objectToConfigure
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @returns {*}
 */
ConfigurationHelper.createParameterHandler = function (objectToConfigure, i18nContext, paramName, content, currentValue) {
   assert(objectToConfigure !== undefined, "objectToConfigure must contain object to configure");
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
         return new SectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue, undefined, undefined);
         break;

      case "radiosection" :
         return new RadioSectionParameterHandler(objectToConfigure, i18nContext, paramName, content, currentValue);
         break;

      case "device" :
         return new DeviceParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "serialport" :
         return new SerialParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "color" :
         return new ColorParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + paramName + " is unsupported");
         break;
   }
};
