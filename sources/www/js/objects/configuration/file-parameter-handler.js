/**
 * Created by JMDECORET on 07/02/17.
 */

/**
 * Create a file parameter handler
 * @param i18nContext
 * @param paramName
 * @param content
 * @param currentValue
 * @constructor
 */
function FileParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");

   this.value = "";

   //we look if the field mustn't be blank
   if (!isNullOrUndefined(content.required))
       this.required = parseBool(content.required);
   else
       this.required = false;

   this.name = content.name;
   this.uuid = createUUID();
   this.paramName = paramName;
   this.description = isNullOrUndefined(content.description)?"":content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

/**
 * Get the DOM Object to insert
 * @returns {string}
 */
FileParameterHandler.prototype.getDOMObject = function () {
    var input = "<input type=\"file\" ";
    input +=            "class=\"form-control\" " +
                        "id=\"" + this.uuid + "\" " +
                        "name=\"" + this.uuid + "\" " +
                        "data-content=\"" + this.description + "\" ";
   if (this.required)
    input +=            "required aria-required ";
   var dataI18n = "data-i18n=\"";
   dataI18n += "[data-content]" + this.i18nContext + this.paramName + ".description";
   dataI18n += "\"";

   input += dataI18n + " >";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

FileParameterHandler.prototype.locateInDOM = function () {
   return $("input#" + this.uuid);
};

/**
 * Get the param name
 * @returns {string}
 */
FileParameterHandler.prototype.getParamName = function() {
   return this.paramName;
};

/**
 * Enable / Disbale the content of the configuration item
 */
FileParameterHandler.prototype.setEnabled = function (enabled) {
    var self = this;

    if (enabled) {
            $("#" + self.uuid).addClass("enable-validation");
    } else {
            $("#" + self.uuid).removeClass("enable-validation");
    }
}

  function handleFileSelect()
  {               
    
  }

/**
 * Get the current configuration in the form
 * @returns {string}
 */
FileParameterHandler.prototype.getCurrentConfiguration = function () {
   var d = new $.Deferred();
   
   var myFile = $("input#" + this.uuid).prop('files');
   
   if (!window.File || !window.FileReader || !window.FileList || !window.Blob) {
      d.reject("The File APIs are not fully supported in this browser.");
    }   

    else if (!myFile) {
      d.reject("This browser doesn't seem to support the `files` property of file inputs.");
    }
    else if (!myFile[0]) {
      d.reject("Please select a file before clicking 'Load'");
    }
    else {
      file = input.files[0];
      var ready = false;
      var result = null;
      
      var fr = new FileReader();
      fr.onload = function(evt) {
          // file is loaded
          d.resolve(evt.target.result);
      };
      
      fr.onabort = function(evt) { d.reject(evt); }
      fr.onerror = function(evt) { d.reject(evt); }
      
      fr.readAsDataURL(file);
      
      
    }   
    return d.promise();
};