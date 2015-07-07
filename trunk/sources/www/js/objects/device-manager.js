/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function DeviceManager(){}

DeviceManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.pluginId), "json.pluginId must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.friendlyName), "json.friendlyName must be defined");
   assert(!isNullOrUndefined(json.model), "json.model must be defined");

   return new Device(json.id, json.pluginId, decodeURIComponent(json.name), decodeURIComponent(json.friendlyName), json.model);
};

DeviceManager.get = function (deviceId, callback, sync) {
   assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "rest/device/" + deviceId,
      async: async
   })
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "Device with Id = " + deviceId}), JSON.stringify(data));
            return;
         }
         callback(DeviceManager.factory(data.data));
      })
      .fail(function() {notifyError($.t("objects.generic.errorGetting", {objectName : "Device with Id = " + deviceId}));});
};

DeviceManager.getAll = function (callback, sync) {
    assert(!isNullOrUndefined(callback), "callback must be defined");

    var async = true;
    if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
        async = !sync;

    $.ajax({
        dataType: "json",
        url: "/rest/device",
        async: async
    }).done(
        /**
         * Receive result from server
         * @param {{result:string}, {data: {device : object}}} data
         */
        function (data) {
        //we parse the json answer
        if (data.result != "true") {
            notifyError($.t("objects.generic.errorGetting", {objectName: "all devices"}), JSON.stringify(data));
            return;
        }

        var devices = [];
        //foreach result we append a <tr>
        $.each(data.data.device, function (index, value) {
            devices.push(DeviceManager.factory(value));
        });
        callback(devices);
    })
        .fail(function () {
            notifyError($.t("objects.generic.errorGetting", {objectName: "all devices"}));
        });

};

DeviceManager.getAllByInstanceId = function(pluginInstanceId, callback, sync) {
   assert(!isNullOrUndefined(pluginInstanceId), "pluginInstanceId must be defined");
   assert(!isNullOrUndefined(callback), "callback must be defined");

   var async = true;
   if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
      async = !sync;

   $.ajax({
      dataType: "json",
      url: "/rest/plugin/" + pluginInstanceId + "/devices",
      async: async
   })  .done(function (data) {
          //we parse the json answer
          if (data.result != "true") {
             notifyError($.t("objects.generic.errorGetting", {objectName: "Devices for pluginInstance  = " + pluginInstanceId}), JSON.stringify(data));
             return;
          }

          var devices = [];
          //foreach result we append a <tr>
          $.each(data.data.devices, function (index, value) {
             devices.push(DeviceManager.factory(value));
          });
          callback(devices);
       })
       .fail(function () {
          notifyError($.t("objects.generic.errorGetting", {objectName: "Device for pluginInstance Id = " + pluginInstanceId}));
       });

};

DeviceManager.getAllByInstance = function(pluginInstance, callback, sync) {

   if (isNullOrUndefined(pluginInstance.devices)) {

      DeviceManager.getAllByInstanceId(pluginInstance.id, function(list){
         pluginInstance.devices =list;
         callback(pluginInstance.devices);
      }, sync);
   }
   else {
      callback(pluginInstance.devices);
   }
};

DeviceManager.getAttachedPlugin = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   assert($.isFunction(callback), "callback must be a function");
   PluginInstanceManager.get(device.pluginId, function (pluginInstance) {
      device.attachedPlugin = pluginInstance;
      callback();
   });
};

DeviceManager.getKeywordsByDeviceId = function(deviceId, callback, sync) {
   assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
   assert(!isNullOrUndefined(callback), "callback must be defined");

      var async = true;
      if (!isNullOrUndefined(sync) && $.type( sync ) === "boolean")
         async = !sync;

      //we ask for the keywords of current device
      $.ajax({
         dataType: "json",
         url: "/rest/device/" + deviceId + "/keyword",
         async: async
      }).done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorGettingDeviceDetails", {deviceName : deviceId}), JSON.stringify(data));
            return;
         }

         //we save the keyword list into the object
         var list = [];
         $.each(data.data.keyword, function (index, value) {
            list.push(KeywordManager.factory(value));
         });

         if ($.isFunction(callback))
            callback(list);
      }).fail(function() { notifyError($.t("objects.deviceManager.errorGettingDeviceDetails", {deviceName : deviceId})); });
};

DeviceManager.getKeywords = function(device, callback, sync) {
   assert(!isNullOrUndefined(device), "device must be defined");

   if (isNullOrUndefined(device.keywords)) {

      DeviceManager.getKeywordsByDeviceId(device.id, function(list) {
         device.keywords =list;

         if ($.isFunction(callback))
            callback();
      }, sync);
   }
   else {
      //keyword has already been gotten
      callback();
   }
};

DeviceManager.deleteFromServer = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   $.ajax({
      type: "DELETE",
      url: "/rest/device/" + device.id,
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorDeleting", {deviceName : device.friendlyName}), JSON.stringify(data));
            return;
         }

         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.deviceManager.errorDeleting", {deviceName : device.friendlyName}));});
};

DeviceManager.updateToServer = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   $.ajax({
      type: "PUT",
      url: "/rest/device/" + device.id,
      data: JSON.stringify(device),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorUpdating", {deviceName : device.friendlyName}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }
         //it's okay
         //we update our information from the server
         device = DeviceManager.factory(data.data);

         //we call the callback with true as a ok result
         if ($.isFunction(callback))
            callback(true);
      })
      .fail(function() {
         notifyError($.t("objects.deviceManager.errorUpdating", {deviceName : device.friendlyName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};