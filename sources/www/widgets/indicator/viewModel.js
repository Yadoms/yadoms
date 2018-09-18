/**
 * Create a Indicator ViewModel
 * @constructor
 */
widgetViewModelCtor = function indicatorViewModel() {

    //observable data
    this.command = ko.observable(1);
    this.icon = ko.observable("");
    this.iconColor = ko.observable("");
	 this.readonly = ko.observable(true);

    this.capacity = null;

    /**
     * Initialization method
     */
    this.initialize = function () {
       var self = this;
       var arrayOfDeffered = [];
       var d = new $.Deferred();
        
       this.widgetApi.toolbar({
          activated: true,
          displayTitle: true,
          batteryItem: true
       });
        
       arrayOfDeffered.push(self.widgetApi.loadGzLibrary(
       ["libs/bootstrap-iconpicker-1.9.0/js/bootstrap-iconpicker-iconset-all.min.js.gz",
        "libs/bootstrap-iconpicker-1.9.0/js/bootstrap-iconpicker.min.js.gz"]));
       arrayOfDeffered.push(self.widgetApi.loadGzCss("libs/bootstrap-iconpicker-1.9.0/css/bootstrap-iconpicker.min.css.gz"));

       $.when.apply($, arrayOfDeffered).done(function () {
          d.resolve();
       })
       .fail(function () {
          d.reject();
       });
        
       return d.promise();        
    };

    this.indicatorClick = function () {
        var self = this;
        if ((!isNullOrUndefined(self.widget.configuration)) && (!isNullOrUndefined(self.widget.configuration.device))) {
            //the click event is taken under account only if the readOnly is not active
            if (!self.readOnly) {
                //we prevent from flooding command
                self.readOnly = true;
                setTimeout(function () {
                    self.readOnly = false;

                    if (self.command() === 0) {
                        self.command(1);
                        self.iconColor(self.activatedColor);
                    } else {
                        self.command(0);
                        self.iconColor(self.deactivatedColor);
                    }
                    //Send the command
                    KeywordManager.sendCommand(self.widget.configuration.device.keywordId, self.command().toString());
                }, 500);
            }
        }
    }

    this.configurationChanged = function () {
        var self = this;

        self.widgetApi.registerKeywordForNewAcquisitions(self.widget.configuration.device.keywordId);		
		  self.widgetApi.getLastValue(self.widget.configuration.device.keywordId);
        self.widgetApi.configureBatteryIcon(self.widget.configuration.device.deviceId);
        self.widgetApi.registerAdditionalInformation(["capacity", "accessMode"]);

        try {
            self.readOnly = parseBool(self.widget.configuration.readOnly);
        } catch (err) {
            self.readOnly = false;
        }


        try {
            self.icon("textfit textfit-in-parent glyphicon " + self.widget.configuration.icon + " " + (self.readOnly ? "" : "writable"));
        }
        catch (err) { }


        //we manage icon colors
        var defaultActivatedColor = "#2e6da4";
        var defaultDeactivatedColor = "#777";

        try {
            self.activatedColor = self.widget.configuration.customColors.checkbox ? self.widget.configuration.customColors.content.activated : defaultActivatedColor;
            self.deactivatedColor = self.widget.configuration.customColors.checkbox ? self.widget.configuration.customColors.content.deactivated : defaultDeactivatedColor;
        } catch (err) {
            self.activatedColor = defaultActivatedColor;
            self.deactivatedColor = defaultDeactivatedColor;
        }
/*
        var deffered;
        
        try {
            // Get the capacity of the keyword to display it correctly
            if (this.widget.configuration.device && this.widget.configuration.device.keywordId) {
                deffered = KeywordManager.get(this.widget.configuration.device.keywordId);
                
                deffered
                .done(function (keyword) {
                    self.capacity = keyword.capacityName;
					
				        if ( keyword.accessMode ==="GetSet" )
					        self.readonly ( false );
				        else
					        self.readonly ( true );
                });
            }
        }
        catch (err) { }
        
        deffered = deffered || new $.Deferred().resolve();
        return deffered.promise();*/
    };

    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        try {
            if (keywordId === self.widget.configuration.device.keywordId) {
               if (!isNullOrUndefinedOrEmpty(data.capacityName))
                  self.capacity[index+1] = data.capacityName;
          
               if (!isNullOrUndefinedOrEmpty(data.accessMode)){
                   if (data.accessMode === "GetSet")
                       self.readonly(false);
                   else
                      self.readonly(true);
               }               
               
                //it is the right device
                if (parseInt(data.value) !== 0) {
                    self.command(1);
                    self.iconColor(self.activatedColor);
                } else {
                    self.command(0);
                    self.iconColor(self.deactivatedColor);
                }
            }
        }
        catch (err) { }
    };
};