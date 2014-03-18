/**
 * Created by Nicolas on 01/03/14.
 */

function BoolParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   //we set the current value of the default value or false
   if ((currentValue !== undefined) && (currentValue != null))
      this.value = parseBool(currentValue);
   else if ((content.defaultValue !== undefined) && (content.defaultValue != null))
      this.value = content.defaultValue;
   else
      this.value = false;

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

BoolParameterHandler.prototype.getDOMObject = function () {

   var self = this;

   var s =
      "<div class=\"control-group\">" +
         "<span class=\"col-sm-5\"></span>" +
         "<button id=\"" + self.uuid + "-help\" class=\"col-sm-1\" type=\"button\" class=\"btn btn-default\" data-container=\"body\">" +
            "<i class=\"fa fa-question\"></i>" +
         "</button>" +

            "<label id=\"" + self.uuid + "\" class=\"controls col-sm-6\" data-content=\"" + self.description + "\" " +
               "data-i18n=\"[data-content]" + self.i18nContext + self.paramName + ".description" + "\"" +
            "\">" +
               "<input type=\"checkbox\" ";
   if (this.value)
      s += "checked ";
   s +=        "></input>&nbsp;&nbsp;" +
               "<span data-i18n=\"" + self.i18nContext + self.paramName + ".name\" >" +
                  self.name +
               "</span>" +
            "</label>"+
      "</div>" +
      "<script>" +
         "$(\"#" + self.uuid + "\").popover({\"placement\" : \"right\", \"trigger\": \"manual\"});\n" +
         "$(\"button#" + self.uuid + "-help\").click(function () {\n" +
         "$(\"#" + self.uuid + "\").popover(\"toggle\");\n" +
         "setTimeout(function () {\n" +
         "$(\"#" + self.uuid + "\").popover(\"hide\"); \n" +
         "}, 5000);\n" +
         "});\n" +
      "</script>";

   return s;
};

BoolParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

BoolParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = parseBool($("label#" + this.uuid).find("input").prop("checked"));
   return this.value;
};