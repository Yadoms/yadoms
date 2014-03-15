/**
 * Created by Nicolas on 01/03/14.
 */

function SectionParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   this.configurationHandlers = new Array();

   var self = this;
   //for each key in package
   $.each(self.configurationSchema, function (key, value) {
      var v = currentValue[key];

      var handler = ConfigurationHelper.createParameterHandler(self.objectToConfigure, key, value, v);
      self.configurationHandlers.push(handler);
   });

   //we get max length value
   this.maximumLength = parseInt(content.maximumLength);

   //we cut the actual value in the maximumLength allowed if greater
   if (!isNaN(this.maximumLength))
      this.value =  this.value.substr(0,this.maximumLength);

   this.name = content.name;
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

SectionParameterHandler.prototype.getDOMObject = function () {
   var input = "<input " +
                        "type=\"text\" " +
                        "class=\"form-control\" " +
                        "id=\"" + this.paramName + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   if (!isNaN(this.maximumLength))
      input += "maxlength=\"" + this.maximumLength + "\" ";

   input += " value =\"" + this.value + "\" >";
   input += "</input>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

SectionParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

SectionParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = $("input#" + this.paramName).val();
   return this.value;
};