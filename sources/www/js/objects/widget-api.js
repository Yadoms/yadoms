/**
 * Created by nicolasHILAIRE on 09/02/2016.
 */
 
/**
 * Creates an instance of WidgetApi
 * @constructor
 */
function WidgetApi(widget) {
   assert(!isNullOrUndefined(widget), "widget must be defined");

   this.widget = widget;
   this.widgetBatteryClass = "widget-toolbar-battery";
}

/**
 * Jquery find method into widget instance
 */
WidgetApi.prototype.find = function (pattern) {
   return this.widget.$gridWidget.find(pattern);
}

/**
 * Change the state of a widget
 */
WidgetApi.prototype.setState = function (newState) {
   if (this.widget.getState() != newState) {
      this.widget.setState(newState);
      
      if (newState == widgetStateEnum.InvalidConfiguration)
      {
         this.widget.$gridWidget.find(".panel-widget-desactivated").removeClass("hidden");
         var message = $.t("objects.widgetManager.widgetDisabled", {widgetName: this.widget.title});
         this.widget.$gridWidget.find(".fa-exclamation-triangle").attr("title", message);
         notifyWarning(message);
      }
      else if (newState == widgetStateEnum.OK)
         this.widget.$gridWidget.find(".panel-widget-desactivated").addClass("hidden");
      else
      {}
   }
}

/**
 * Obtain information about a keyword
 * @param {} keywordId to query
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.prototype.getKeywordInformation = function (keywordId) {
   assert(!isNullOrUndefinedOrEmpty(keywordId), "keywordId must be defined");
   return KeywordManager.get(keywordId);
};

/**
 * @deprecated Register keywords to receive notifications when a new acquisition triggers
 * Obtain information about a device
 * @param {} deviceId to query
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.prototype.getDeviceInformation = function (deviceId) {
   assert(!isNullOrUndefinedOrEmpty(deviceId), "deviceId must be defined");
   return DeviceManager.get(deviceId);
};

/**
 * Obtain information about a plugin
 * @param {} deviceId to query
 * @returns {} a promise that's return done when information grabbed from server
 */
WidgetApi.prototype.getPluginInstanceInformation = function (pluginId) {
   assert(!isNullOrUndefinedOrEmpty(pluginId), "pluginId must be defined");
   return PluginInstanceManager.get(pluginId);
};

/**
 * @param {} keywordIds to register (can be a single value or an array of values)
 */
WidgetApi.prototype.registerKeywordAcquisitions = function (keywordIds) {
   assert(!isNullOrUndefinedOrEmpty(keywordIds), "keywordIds must be defined");
   
   var self = this;
   console.warn("this function is deprecated and will be removed soon");

   self.widget.viewModel.widgetApi.registerKeywordForNewAcquisitions (keywordIds);
   self.widget.viewModel.widgetApi.getLastValue(keywordIds);
};

/**
 * Register keywords to receive notifications when a new acquisition triggers (don't do a getLastValue)
 * @param {} keywordIds to register (can be a single value or an array of values)
 */
WidgetApi.prototype.registerKeywordForNewAcquisitions = function (keywordIds) {
   assert(!isNullOrUndefinedOrEmpty(keywordIds), "keywordIds must be defined");
   
   var self = this;
   if (!self.widget.listenedKeywords)
      self.widget.listenedKeywords = [];
   
   if (Array.isArray(keywordIds)) {
      $.each(keywordIds, function (index, value) {
         self.widget.listenedKeywords.push(value);
      });
   } else {
      self.widget.listenedKeywords.push(keywordIds);
   }
};

/**
 * Register keywords to get the last value of the keywordId when the page is loaded
 * @param {} keywordIds to register (can be a single value or an array of values)
 */
WidgetApi.prototype.getLastValue = function (keywordIds) {
   assert(!isNullOrUndefinedOrEmpty(keywordIds), "keywordIds must be defined");
   
   var self = this;
   if (!self.widget.getlastValue)
      self.widget.getlastValue = [];
   
   if (Array.isArray(keywordIds)) {
      $.each(keywordIds, function (index, value) {
         self.widget.getlastValue.push(value);
      });
   } else {
      self.widget.getlastValue.push(keywordIds);
   }
};

/**
 * Lazy load required libraries
 * @param {} librayNames to load (can be a single value or an array of values)
 * @returns {} a promise that's return done when libraries are loaded
 */
WidgetApi.prototype.loadLibrary = function (librayNames) {
   assert(!isNullOrUndefined(librayNames), "librayNames must be defined");

   if (Array.isArray(librayNames)) {
      return asyncLoadJSLibs(librayNames);
   } else {
      return asyncLoadJSLib(librayNames);
   }
};

/**
 * Lazy load required CSS stylesheet
 * @param {} librayNames to load (can be a single value or an array of values)
 * @returns {} a promise that's return done when libraries are loaded
 */
WidgetApi.prototype.loadCss = function (cssFiles) {
   assert(!isNullOrUndefined(cssFiles), "cssFiles must be defined");

   if (Array.isArray(cssFiles)) {
      return asyncLoadManyCss(cssFiles);
   } else {
      return asyncLoadCss(cssFiles);
   }
};

/**
 * Configure toolbar
 * @param {} options : json options (see wiki for help)
 */
WidgetApi.prototype.toolbar = function (options) {
    assert(!isNullOrUndefined(options), "options must be defined");

    var self = this;

    //we define default values
    options.activated = options.activated || false;
    options.displayTitle = options.displayTitle || true;
    options.items = options.items || [];
    options.batteryItem = options.batteryItem || false;

    //we manage options
    self.widget.toolbarActivated = options.activated;

    if (self.widget.toolbarActivated) {
        self.find(".panel-widget-header").removeClass("hidden");
        self.widget.displayTitle = options.displayTitle;

        //we manage title
        if (self.widget.displayTitle)
            self.find(".panel-widget-title").removeClass("hidden");
        else
            self.find(".panel-widget-title").addClass("hidden");

        //battery
        if (options.batteryItem) {
            self.widget.$toolbar.append("<div class=\"" + self.widgetBatteryClass + "\" deviceId=\"\"></div>");
        }

        //all other items
        $.each(options.items, function(index, value) {
            if (value.hasOwnProperty("custom")) {
                self.widget.$toolbar.append(value["custom"]);
            } else if (value.hasOwnProperty("separator")) {
                //for the moment value has no effect. Reserved for future use with options like margin, ...
                self.widget.$toolbar.append("<div class=\"widget-toolbar-separator\"></div>");

            } else {
                console.error("Unknown item type: " + index);
            }
        });
        
        //i18n of page tab
        self.widget.$toolbar.i18n();
    } else {
        self.find(".panel-widget-header").addClass("hidden");
    }
};

/**
 * Check if the widget use battery item and configure it
 */
WidgetApi.prototype.manageBatteryConfiguration = function () {

    var self = this;
	
	var d = new $.Deferred();
    var $battery = self.widget.$toolbar.find("." + self.widgetBatteryClass);
        $battery.empty();
        var deviceId = $battery.attr("deviceId");
        if (!isNullOrUndefinedOrEmpty(deviceId)) {
           //we check for the device to look if it has battery keyword
           DeviceManager.getKeywordsBydeviceIdAndCapacity(deviceId, "Get", "batteryLevel")
           .done(function (keyword) {
               // We assume that we have only 1 batteryLevel keyword for one device, it's the first one
               if (keyword.length>0) {
                 $battery.removeClass("hidden");
                 //it has capacity
                 $battery.append("<span class=\"\"/>");
                 $battery.attr("keywordId", keyword[0].id);
                 //we add it to the filter of keyword for websockets
                 self.widget.viewModel.widgetApi.registerKeywordForNewAcquisitions(keyword[0].id);

                 //we ask immediately for the battery value
                 AcquisitionManager.getLastValue(keyword[0].id)
                 .done(function (lastValue) {
                    if (lastValue.value !== "")
                        self.widget.viewModel.widgetApi.updateBatteryLevel(lastValue.value);
                     else 
                        $battery.addClass("hidden"); // if no value => we hide the icon
                     
                     d.resolve();
                 })
                 .fail(function (error) {
                    $battery.addClass("hidden");
                    notifyError($.t("objects.generic.errorGetting", { objectName: "Acquisition KeywordId = " + keyword[0].id }), error);
                    d.reject();
                 });
             }
             else {
               //we can hide the div to prevent margin spaces before the title
               $battery.addClass("hidden");
               d.resolve();
             }
         })
         .fail(function (error){
            $battery.addClass("hidden");
            notifyError($.t("objects.generic.errorGetting", { objectName: "keyword for device = " + deviceId }), error);
            d.reject();
         });
    }
    else
    {
       //we can hide the div to prevent margin spaces before the title
       $battery.addClass("hidden");
       d.resolve();
    }    
	
	 return d.promise();
}

/**
 * Apply batterylevel onto the icon of the toolbar
 */
WidgetApi.prototype.updateBatteryLevel = function (batteryLevel) {
    assert(!isNullOrUndefined(batteryLevel), "batteryLevel must be defined");
    var self = this;

    //we compute the battery fill
    var fill;
    var lvl = parseInt(batteryLevel);
    if (lvl < 20.0)
        fill = 0;
    else if (lvl < 40.0)
        fill = 1;
    else if (lvl < 60.0)
        fill = 2;
    else if (lvl < 80.0)
        fill = 3;
    else
        fill = 4;

    this.widget.$toolbar.find("div." + self.widgetBatteryClass + " span").removeClass().addClass("fa fa-battery-" + fill);
};

/**
 * Define the keyword Id of the battery toolbar icon. 
 */
WidgetApi.prototype.configureBatteryIcon = function (deviceId) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
    var $batteryIcon = this.widget.$toolbar.find("." + this.widgetBatteryClass);
    if ($batteryIcon) {
        $batteryIcon.attr("deviceId", deviceId);
    }
}

/**
 * Make the text which has class "textfit" fit into the widget
 */
WidgetApi.prototype.fitText = function () {
    var $texts = this.find(".textfit");
    if ($texts)
        $texts.fitText();
}

/**
 * Notify a message to the window using noty library
 * @param message message to display
 * @param gravity gravity of the message
 * @returns {noty}
 * @param timeout timeout of the noty
 */
WidgetApi.prototype.notify = function(message, gravity, timeout) {
    assert(message != undefined, "Message must be defined");
    gravity = gravity || "information";
    timeout = timeout || defaultNotyTimeout;
    
    return noty({ text: message, timeout: timeout, layout: defaultNotyLayout, type: gravity });
}

/**
 * Configure/Modify the Rolling Title for widgets
 */
 
WidgetApi.prototype.manageRollingTitle = function () {
	var self = this;
   
   //console.log ("widget !", self.widget);
   
	if (self.widget.displayTitle && self.widget.toolbarActivated){
		if (self.widget.$toolbar[0].scrollWidth <= 3) // Round size of the padding-right of the panel-widget-title-toolbar
			toolbarSize = 0;
		else
			toolbarSize = self.widget.$toolbar[0].scrollWidth;
      
		//Calculate the overflow ! Theses values could be obtain, only after the drawing of all elements !
		var overflow = toolbarSize +
					   self.widget.$header.find(".panel-widget-title")[0].scrollWidth -
					   self.widget.$header[0].scrollWidth;
      
		if (overflow > 0) {
			if (self.widget.$header.find(".panel-widget-title-" + self.widget.id).length !== 0){ 
				 rule = getRule ( "panel-widget-title-marquee-" + self.widget.id );
                 
				 //Delete the rule containing the overflow to scroll
				 rule[0].deleteRule("50%");
				 
				 //Append the new overflow
				 rule[0].appendRule("50% { text-indent: " + -(overflow+5) + "px;}"); // Add 5 pixels to see properly the last letter of the title
			}
			else{
				// Create completly the css rule
				$("<style type='text/css'> .panel-widget-title-" + self.widget.id + "{margin: 0 auto; overflow: hidden; white-space: nowrap; box-sizing: border-box; animation: panel-widget-title-marquee-" + self.widget.id +
				  " 10s steps(150) infinite;-webkit-animation-play-state: running; animation-play-state: running;}</style>").appendTo("head");	//html > //ease-in-out
				$("<style type='text/css'> @keyframes panel-widget-title-marquee-" + self.widget.id + " { 0%   { text-indent: 0px; } 50% { text-indent: " + -(overflow+5) + "px;}  100%  { text-indent: 0px; } }</style>").appendTo("head");
				self.widget.$header.find(".panel-widget-title").addClass("panel-widget-title-" + self.widget.id);
			}
		}
		else{
			// If exist, remove the class associated to the div
			self.widget.$header.find(".panel-widget-title").removeClass("panel-widget-title-" + self.widget.id);
		}
	}
}

WidgetApi.prototype.askServerLocalTime = function (callback) {
   assert(typeof callback === 'function', "callback must be defined");
   
   var d = $.Deferred();
   
   YadomsTimeManager.getCurrentLocalTime()
   .done(function(data) {
      callback(data.now);
      d.resolve(data.now);
   })
   .fail(function(error) {
      var message = "Fail to get server local time " + error;
      console.warn(message);
      d.reject(message);
   });
   
   return d.promise();
}