/**
 * Created by nicolasHILAIRE on 06/03/14.
 */
function ConfigurationHelper(){}

ConfigurationHelper.loadConfigurationLibs = function() {
    var d = new $.Deferred();
    asyncLoadJSLibs([
        "js/objects/configuration/configuration-control-manager.js",
        "js/objects/configuration/int-parameter-handler.js",
        "js/objects/configuration/decimal-parameter-handler.js",
        "js/objects/configuration/enum-parameter-handler.js",
        "js/objects/configuration/string-parameter-handler.js",
        "js/objects/configuration/file-parameter-handler.js",
        "js/objects/configuration/bool-parameter-handler.js",
        "js/objects/configuration/section-parameter-handler.js",
        "js/objects/configuration/keyword-parameter-handler.js",
        "js/objects/configuration/keywordValue-parameter-handler.js",
        "js/objects/configuration/radio-section-parameter-handler.js",
        "js/objects/configuration/combo-section-parameter-handler.js",
        "js/objects/configuration/color-parameter-handler.js",
        "js/objects/configuration/icon-parameter-handler.js",
        "js/objects/configuration/list-parameter-handler.js"
    ]).done(function () {
        d.resolve();
    });

    return d.promise();
}

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

   // Convert markdown for the designation field.
   var result = "";
   if ( !isNullOrUndefined ( parameterHandler ))
      result = ConfigurationHelper.makdownIt( $.t(parameterHandler.i18nContext + parameterHandler.paramName + ".description", {defaultValue: parameterHandler.description}) );
   
   s += "<span class=\"configuration-label-content configuration-label-name\" data-i18n=\"" + parameterHandler.i18nContext + parameterHandler.paramName + ".name\">" + parameterHandler.name + "</span>" +
      "<span class=\"configuration-label-content configuration-label-description\"\">" + result + "</span>" +
      "</label>" +
      "</div>";
   if (!placeInsideLabel) {
      s += "<div class=\"col-sm-6 control-label\" style=\"text-align: left;\">" +
         controlToInsert +
         "</div>";
   }

   s += "</div>";

   return s;
};

/**
 * Factory of parameter Handler
  * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @param parentRadioButtonSectionName: used only for containers to add a radio button at the beginning for radiosection handler
 * @param parentRadioSectionActive: used only for containers to add a radio button at the beginning for radiosection handler
 * @returns {*}
 */
ConfigurationHelper.createParameterHandler = function (i18nCtxt, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive) {
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined in " + paramName + " parameter");
   assert(content.type !== undefined, "type field must be found in " + paramName + " parameter");
   assert(i18nCtxt !== undefined, "i18nCtxt must contain path of i18n " + paramName + " parameter");
   
   if (content.show !== undefined && content.show.result === "false")
      return null;

   var i18nContext = i18nCtxt;
   if(content.i18nBasePath)
      i18nContext = content.i18nBasePath;
   
   switch (content.type.toLowerCase()) {
      case "int" :
         return new IntParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "decimal" :
         return new DecimalParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "enum" :
         return new EnumParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "string" :
         return new StringParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "file" :
         return new FileParameterHandler(i18nContext, paramName, content, currentValue);
         break;

      case "bool" :
         return new BoolParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "section" :
         return new SectionParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive);
         break;

      case "radiosection" :
         return new RadioSectionParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive);
         break;

      case "combosection" :
         return new ComboSectionParameterHandler(i18nContext, i18nKey, paramName, content, currentValue, parentRadioButtonSectionName, parentRadioSectionActive);
         break;

      case "keyword" :
         return new KeywordParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "keywordvalue" :
         return new KeywordValueParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "color" :
         return new ColorParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "icon" :
         return new IconParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      case "list" :
         return new ListParameterHandler(i18nContext, i18nKey, paramName, content, currentValue);
         break;

      default :
         throw Error("type " + content.type + " of parameter " + paramName + " is unsupported");
         break;
   }
};



/**
 * Factory of parameter Handler
  * @param i18NContext
 * @param paramName
 * @param keyword
 * @param currentValue
 * @returns {*}
 */
ConfigurationHelper.createKeywordValueParameterHandler = function (i18NContext, i18nKey, paramName, keyword, currentValue, pluginInstance) {
   assert(paramName !== undefined, "paramName must be defined");
   assert(keyword !== undefined, "keyword must be defined in " + paramName + " parameter");
   assert(keyword.type !== undefined, "type field must be found in " + paramName + " parameter");
   assert(i18NContext !== undefined, "i18nContext must contain path of i18n " + paramName + " parameter");

   var obj  = {
      name: keyword.friendlyName,
      description: $.t(keyword.units, { defaultValue: keyword.units }),
      type: keyword.type,
      values : keyword.typeInfo.values,
      minimumValue : keyword.typeInfo.min,
      maximumValue : keyword.typeInfo.max,
      precision : keyword.typeInfo.precision,
   }
   
   switch (keyword.type.toLowerCase()) {
      case "numeric":
         return new DecimalParameterHandler(i18NContext, i18nKey, paramName, obj, currentValue);

      case "enum":
         var enumValues = {};
         if (obj.values) {
            for(var i in obj.values) { // don't use for(.. of obj.values) because it fails when values is less than 2 elements
               if (obj.values.hasOwnProperty(i))
               {
                  var item = obj.values[i]; 
                  enumValues[item]=item;
               }
            }
         }
         obj.values = enumValues;
         return new EnumParameterHandler("plugins." + pluginInstance.type + ":enumerations.", i18nKey, keyword.typeInfo.name, obj, currentValue);

      case "string":
         return new StringParameterHandler(i18NContext, i18nKey, paramName, obj, currentValue);

      case "bool":
         return new BoolParameterHandler(i18NContext, i18nKey, paramName, obj, currentValue, true);

      default:
         return null;
   }
};

/**
 * Check if content item is a container (section,  radiosection, ...)
 * @param content
 * @returns { boolean : true if the item is a container}
 */
ConfigurationHelper.isContainer = function (content) {
   assert(content !== undefined, "content must be defined");
   assert((content.type !== undefined), "Type of the content must be defined");

   return ((content.type.toLowerCase() == "section") || (content.type.toLowerCase() == "radiosection") || (content.type.toLowerCase() == "combosection"))
};

/**
 * Render Markdown text
 * @param str Text to render
 * @returns Rendered text (HTML format)
 */
ConfigurationHelper.makdownIt = function(str) {
   var iterator = window.markdownitForInline;
   var md = window.markdownit({
     breaks:true,
     linkify:true
   })
   .use(iterator, 'url_new_win', 'link_open', function (tokens, idx) {
     tokens[idx].attrPush([ 'target', '_blank' ]);
   });    
   
   return md.renderInline(str);
}
