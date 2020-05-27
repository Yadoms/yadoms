widgetViewModelCtor =

   /**
    * Create a colorPicker ViewModel
    * @constructor
    */
   function RGBcolorPickerViewModel() {
      this.colorpicker = null;
      this.WidgetWidth = 156;
      this.WidgetHeight = 120;

      /**
       * Initialization method
       * @param widget widget class object
       */
      this.initialize = function () {
         var self = this;
         var arrayOfDeffered = [];
         var d = new $.Deferred();

         arrayOfDeffered.push(self.widgetApi.loadGzCss("libs/bootstrap-colorpicker/css/bootstrap-colorpicker.min.css.gz"));
         arrayOfDeffered.push(self.widgetApi.loadGzLibrary("libs/bootstrap-colorpicker/js/bootstrap-colorpicker.min.js.gz"));

         $.when.apply($, arrayOfDeffered).done(function () {
               self.createWidgetPickerStyle(self.widget.id);

               //we configure the toolbar
               self.widgetApi.toolbar({
                  activated: true,
                  displayTitle: true,
                  batteryItem: false
               });

               d.resolve();
            })
            .fail(d.reject);

         return d.promise();
      };

      this.createWidgetPickerStyle = function (widgetId) {
         $('head').append(
            $('<style/>', {
               id: 'widget-picker-' + widgetId,
               html: '.colorpicker-size-' + widgetId + ' .colorpicker-saturation { width: 150px; height: 150px; background-size: 150px 135px;}'
            })
         );
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-hue, .colorpicker-size .colorpicker-alpha {width: 15px;height: 150px;background-size: 15px 135px;} ');
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-color, .colorpicker-size .colorpicker-color div {height: 14px;background-size: 14px 150px;} ');
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-selectors{display: none;height: 35px;width: 150px;margin-top: 4px;clear: both;} ');
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-selectors i{cursor: pointer;float: left;height: 30px;width: 30px;} ');
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-selectors i + i{margin-left: 3px;} ');
         $('#widget-picker-' + widgetId).append('.colorpicker-size-' + widgetId + ' .colorpicker-selectors-color{height: 30px;width: 30px;background-size: 30px 30px;} ');
      };

      this.createPicker = function (preselectedColor) {
         var self = this;
         self.colorpickerCanvas = $('.picker-canvas');
         self.colorpicker = self.colorpickerCanvas.colorpicker({
            customClass: 'colorpicker-size-' + self.widget.id,
            hexNumberSignPrefix: false,
            sliders: {
               saturation: {
                  maxLeft: 156,
                  maxTop: 120
               },
               hue: {
                  maxTop: 120
               },
               alpha: {
                  maxTop: 120
               }
            },
            color: '#000000',
            format: 'rgb',

            colorSelectors: preselectedColor,
            // remove the dropdown-menu class to disable the border
            template: "<div class='colorpicker'><div class='colorpicker-saturation'><i><b></b></i></div><div class='colorpicker-hue'><i></i></div><div class='colorpicker-alpha'><i></i></div><div class='colorpicker-color'><div /></div><div class='colorpicker-selectors'></div></div>",
            container: true,
            inline: true
         });

         // capture the event changeColor
         self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
         self.colorpickerCanvas.attr("style", "pointer-events:none;");
         KeywordManager.get(parseInt(this.widget.configuration.device.keywordId))
            .done(function (keyword) {
               if (keyword.accessMode === "GetSet") {
                  self.colorpickerCanvas.removeAttr("style");
               }
            });
      };

      // function called when the color changed
      this.changeColorButtonClick = function () {
         var self = this;
         return function (e) {
            self.colorpicker.unbind('changeColor')
            KeywordManager.sendCommand(parseInt(self.widget.configuration.device.keywordId), e.color.toHex().toString());
            self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
         };
      };

      this.configurationChanged = function () {
         var self = this;
         var preselectedColor = null;

         if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
            return;

         //we register keyword new acquisition
         self.widgetApi.registerKeywordForNewAcquisitions(parseInt(self.widget.configuration.device.keywordId));

         //we register keyword for get last value at web client startup
         self.widgetApi.getLastValue(parseInt(self.widget.configuration.device.keywordId));

         // destroy the precedent colorPicker if any
         // it's the only solution, to create/delete preselected colors
         if (!isNullOrUndefined(self.colorpicker)) self.colorpicker.colorpicker('destroy');

         if (!(isNullOrUndefinedOrEmpty(self.widget.configuration.colorsSelection))) {
            if (self.widget.configuration.colorsSelection.length > 0) preselectedColor = {};
            self.widget.configuration.colorsSelection.forEach(function (item) {
               var newProperties = {};
               newProperties[item.content.name] = item.content.color;
               $.extend(preselectedColor, newProperties);
            });
         }

         self.createPicker(preselectedColor);
         self.resized();
      };

      this.changeCss = function (width, height) {
         var self = this;
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-saturation").css('height', height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-saturation").css('width', width + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-saturation").css('background-size', width + 'px ' + height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-hue").css('height', height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-hue").css('background-size', '15px ' + height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-alpha").css('height', height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-alpha").css('background-size', '14px ' + height + 'px');

         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-color").css('background-size', '14px ' + height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-color div").css('background-size', '14px ' + height + 'px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-color div").css('height', '14px');
         this.widgetApi.find(".colorpicker-size-" + self.widget.id + " .colorpicker-selectors").css('width', width + 15 + 'px');
      };

      this.resized = function () {
         var self = this;
         self.WidgetWidth = this.widget.getWidth() - 34;

         // If no colors preselected, we use all the space
         if (isNullOrUndefined(self.colorpicker.data('colorpicker').options.colorSelectors))
            self.WidgetHeight = this.widget.getHeight() - 57;
         else
            self.WidgetHeight = this.widget.getHeight() - 90;

         // Update sliders values
         self.colorpicker.data('colorpicker').options.sliders.saturation.maxLeft = self.WidgetWidth;
         self.colorpicker.data('colorpicker').options.sliders.saturation.maxTop = self.WidgetHeight;

         self.colorpicker.data('colorpicker').options.sliders.hue.maxTop = self.WidgetHeight;
         self.colorpicker.data('colorpicker').options.sliders.alpha.maxTop = self.WidgetHeight;

         self.changeCss(self.WidgetWidth, self.WidgetHeight);

         self.colorpicker.colorpicker('update');
      };

      /**
       * New acquisition handler
       * @param keywordId keywordId on which new acquisition was received
       * @param data Acquisition data
       */
      this.onNewAcquisition = function (keywordId, data) {
         var self = this;

         if (keywordId === parseInt(self.widget.configuration.device.keywordId)) {
            self.colorpicker.unbind('changeColor');
            self.colorpicker.colorpicker('setValue', data.value);
            self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
         }
      };
   };