/**
 * Create a Weather ViewModel
 * @constructor
 */
widgetViewModelCtor = function weatherViewModel() {
    //observable data
    this.windspeed = ko.observable("-");
    this.direction = ko.observable(0);
    this.unit = "m/s";
    this.background = ko.observable("widgets/wind/images/Windrose-fr.png");
    this.correction = 0;

   this.displayNeedle = function(direction){
	   var self = this;
      var element = self.widgetApi.find(".wind-canvas");
      var mainDiv = self.widgetApi.find(".mainDiv");
      
		//get a reference to the canvas
      var ctx = element.get(0).getContext("2d");
		
		// Refresh the canvas, clear all existing information
		ctx.clearRect(0, 0, element.width(), element.height());
      
      self.direction(direction);
      
      var centerX = mainDiv.width()/2 + 2;
      var centerY = mainDiv.height()/2;
      var needleLenght = 0;
      
      if (self.widget.getHeight() == 200 && self.widget.getWidth() == 200)
         needleLenght = 70;
      else
         needleLenght = 110;
      
      var radius = direction*Math.PI/180;
      
		//Attributes of canvas could only be changed trough theses variables. In an other way the canvas is stretched.
		element.attr('width', mainDiv.width());
		element.attr('height', mainDiv.height());
		element.css('width', mainDiv.width());
		element.css('height', mainDiv.height());
		
      ctx.fillStyle = "rgb(128,0,0)";
      ctx.strokeStyle = '#ff0000';
      ctx.beginPath();
      ctx.lineWidth = 1;      
      ctx.moveTo(centerX,centerY);
      ctx.lineTo(centerX-needleLenght*Math.sin(radius-10*Math.PI/180)/3,centerY-needleLenght*Math.cos(radius-10*Math.PI/180)/3);
      ctx.lineTo(centerX-needleLenght*Math.sin(radius),centerY-needleLenght*Math.cos(radius));
      ctx.lineTo(centerX-needleLenght*Math.sin(radius+10*Math.PI/180)/3,centerY-needleLenght*Math.cos(radius+10*Math.PI/180)/3);
      ctx.lineTo(centerX,centerY);
      ctx.closePath();
      ctx.fill();
      ctx.stroke();
   };
   
   this.displayWindSpeed = function () {
      self = this;
      var divSpeed = self.widgetApi.find(".windspeed");
      var mainDiv = self.widgetApi.find(".mainDiv");
      
      divSpeed.css('top', mainDiv.height() - 22);
      divSpeed.css('left', mainDiv.width() - 60);
   };
    
    /**
     * Initialization method
     * @param widget widget class object
     */
    this.initialize = function () {
        var self = this;
    };
    
    this.mod = function(number, n) {
       var m = (( number % n) + n) % n;
       return m < 0 ? m + Math.abs(n) : m;
    };

    /**
     * New acquisition handler
     * @param keywordId keywordId on which new acquisition was received
     * @param data Acquisition data
     */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        
        if (parseBool(self.widget.configuration.speedDisplay.checkbox)){
           if (keywordId === self.widget.configuration.speedDisplay.content.windSpeed.keywordId) {
               self.windspeed(parseFloat(data.value).toFixed(1) + " " + self.unit);
           }
        }
        
        if (keywordId === self.widget.configuration.windDirection.keywordId) {
           console.log (parseFloat(data.value) + self.correction);
            self.displayNeedle(self.mod(parseFloat(data.value) + self.correction,360));
        }
    };
    
    this.resized = function(){
      var self = this;
      
      self.displayNeedle(self.direction());
      self.displayWindSpeed();
    };
    
    this.configurationChanged = function () {
        var self = this;

        if ((isNullOrUndefined(self.widget)) || (isNullOrUndefinedOrEmpty(self.widget.configuration)))
            return;
        
        // we register keyword new acquisition
        self.widgetApi.registerKeywordAcquisitions([self.widget.configuration.windDirection.keywordId]);
        
        // we add only the speed if we have ckeck it
        if (parseBool(self.widget.configuration.speedDisplay.checkbox)){
           self.widgetApi.registerKeywordAcquisitions([self.widget.configuration.speedDisplay.content.windSpeed.keywordId]);
           self.widgetApi.find(".windspeed").css("visibility","visible");
        }else
           self.widgetApi.find(".windspeed").css("visibility","hidden");
        
        if (parseBool(self.widget.configuration.options.checkbox)){
           self.correction = parseInt(self.widget.configuration.options.content.directionCorrection);
        }
        else
           self.correction = 0;
        
        self.displayNeedle("-");
    }
};